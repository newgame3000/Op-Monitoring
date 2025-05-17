#include "data_sync_thread.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <stdlib.h>


void DataSyncThread::createRes(bool res, QString description, int id, QString table, QString field, QString error_code) {

    QSqlQuery resInsertQuery(sourceDb);
    QString resStr;
    if (!res) {
        resStr = "False";
        procQuery(resInsertQuery, QString("INSERT INTO inner_db.result (res, error_code, description) VALUES (%1, %2, \'%3\')")
                                      .arg(resStr, error_code, description));
    } else {
        resStr = "True";
        procQuery(resInsertQuery, QString("INSERT INTO inner_db.result (res, description) VALUES (%1, \'%2\')")
                                      .arg(resStr, description));
    }


    QSqlQuery impInsertQuery(sourceDb);
    procQuery(impInsertQuery, QString("UPDATE inner_db.%1 SET %2 = %3 WHERE inner_db.%1.id = %4")
                                  .arg(table, field, resInsertQuery.lastInsertId().toString(), QString::number(id)));
}

bool DataSyncThread::checkStatus(QSqlQuery eventQuery, int &idx, QString table, QString field) {
    //Проверка "на всякий случай"
    if (idx == -1) {
        return false;
    }

    //Ошибка, записываем результат, обнуляем индекс формирования ИМП
    if (eventQuery.value("severity").toInt() == ERROR) {
        qDebug() << "Обнаружена ошибка";
        QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
        QJsonObject addInfoJson = addInfoJsonDoc.object();
        createRes(false, eventQuery.value("description").toString(), idx, table, field, addInfoJson["error_code"].toString());
        idx = -1;
        return false;
    }

    return true;
}


int DataSyncThread::checkOperator(QSqlQuery &spanQuery, int span_id, int session_id) {
    procQuery(spanQuery, QString("SELECT context FROM trace_2025.span WHERE trace_2025.span.id = %1").arg(span_id));

    QSqlQuery sessionQuery(sourceDb);
    procQuery(sessionQuery, QString("SELECT * FROM trace_2025.session WHERE trace_2025.session.id = %1").arg(session_id));

    //Ищем индекс оператора, если его нет - создаем
    QSqlQuery operatorQuery(sourceDb);
    procQuery(operatorQuery, QString("SELECT * FROM inner_db.operator WHERE inner_db.operator.login = \'%1\' AND inner_db.operator.server = \'%2\' AND inner_db.operator.start_time = \'%3\'")
                                 .arg(sessionQuery.value("login").toString(), sessionQuery.value("server").toString(), sessionQuery.value("start_time").toString()), false);

    int inner_operator_id = -1;
    if (!operatorQuery.next()) {
        qDebug() << "Оператор во внутренней БД не найден. Добавляем";
        QSqlQuery insertOperatorQuery(sourceDb);

        procQuery(operatorQuery, QString("INSERT INTO inner_db.operator (login, server, start_time) VALUES (\'%1\', \'%2\', \'%3\')")
                                     .arg(sessionQuery.value("login").toString(), sessionQuery.value("server").toString(), sessionQuery.value("start_time").toString()));

        inner_operator_id = insertOperatorQuery.lastInsertId().toInt();
    } else {
        inner_operator_id = operatorQuery.value("id").toInt();
    }

    return inner_operator_id;
}

QString DataSyncThread::jsonArrToString(QJsonArray arr) {
    QString jsonArrStr = "{ ";
    for (int i = 0; i < arr.size(); ++i) {
        jsonArrStr += arr[i].toString();
        if (i != arr.size() - 1) {
            jsonArrStr += ", ";
        } else {
            jsonArrStr += " }";
        }
    }
    return jsonArrStr;
}

DataSyncThread::DataSyncThread(QObject *parent) : QThread(parent), isRunning(true) {

    // Настройка подключений к базам данных PostgreSQL
    sourceDb = QSqlDatabase::addDatabase("QPSQL", "source_connection");
    sourceDb.setHostName("localhost");     // Хост базы данных
    sourceDb.setDatabaseName("postgres"); // Имя базы источника
    sourceDb.setUserName("postgres");  // Имя пользователя
    sourceDb.setPassword("vkm");      // Пароль
    sourceDb.setPort(5432);           // Порт PostgreSQL по умолчанию


    // QString config = getenv("CONFIG");
    config = "config.json";
    QFile file(config);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        config = "";
        qDebug() << "Не удалось открыть файл с настройками. Настройки установлены по умолчанию.";
        lastUpdateInnerDb = QDateTime::currentDateTime();
    } else {
        QString timeFormat = "yyyy-MM-dd HH:mm:ss";
        QString val = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject jsonObj = jsonDoc.object();
        lastUpdateInnerDb = QDateTime::fromString(jsonObj.value("last_update").toString(),timeFormat);
    }
}

void DataSyncThread::run() {
    if (!sourceDb.open()) {
        qDebug() << "Не удалось открыть базы данных";
        qDebug() << "Ошибка источника:" << sourceDb.lastError().text();
        return;
    }

    qDebug() << "Подключено к БД поток поток синхронизации";

    while (isRunning) {
        // Проверка наличия новых
        QSqlQuery eventQuery(sourceDb);
        QString time = lastUpdateInnerDb.toString("yyyy-MM-dd HH:mm:ss");
        procQuery(eventQuery, QString("SELECT * FROM trace_2025.event WHERE event_time > \'%1\'").arg(time), false);

        while (eventQuery.next()) {

            int span_id = eventQuery.value("span_id").toInt();
            int session_id = eventQuery.value("session_id").toInt();

            qDebug() << eventQuery.value("description");

            switch (eventQuery.value("type").toInt()) {
                // ИМП events
                case IMP_DB_MCI_CONNECTION: {
                    // Подключение к базе данных МЦИ при формировании ИМП

                    QSqlQuery spanQuery(sourceDb);
                    int inner_operator_id = checkOperator(spanQuery, span_id, session_id);
                    qDebug() << "Id текущего оператора формирования ИМП: " << inner_operator_id;

                    QJsonDocument contextJsonDoc = QJsonDocument::fromJson(spanQuery.value("context").toString().toUtf8());
                    QJsonObject contextJson = contextJsonDoc.object();

                    QSqlQuery impInsertQuery(sourceDb);
                    procQuery(impInsertQuery, QString("INSERT INTO inner_db.imp (name, type, id_operator) VALUES (\'%1\', \'%2\', %3)")
                                                  .arg(contextJson["imp_name"].toString(), contextJson["imp_type"].toString(), QString::number(inner_operator_id)));

                    lastImpId = impInsertQuery.lastInsertId().toInt();


                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");

                    break;
                }
                case IMP_DB_TOP_CONNECTION: {
                    // Подключение к базе данных ТОП при формировании ИМП
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_DB_BI_CONNECTION: {
                    // Подключение к базе данных БИ при формировании ИМП
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_OPERATOR_CREATE_OPEN: {
                    // Оператор создал/открыл текст ИМП

                    if (!checkStatus(eventQuery, lastImpId, "imp", "id_res_create")) {
                        break;
                    }

                    //Надо ли брать абонента?
                    QSqlQuery impInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();
                    procQuery(impInsertQuery, QString("UPDATE inner_db.imp SET abonent = \'%1\', last_save_source_time = \'%2\' WHERE inner_db.imp.id = %3")
                                                .arg(addInfoJson["ab"].toString(), eventQuery.value("event_time").toString(), QString::number(lastImpId)));

                    break;
                }
                case IMP_OPERATOR_SAVE: {
                    // Оператор сохранил текст ИМП
                    if (!checkStatus(eventQuery, lastImpId, "imp", "id_res_create")) {
                        break;
                    }

                    QSqlQuery impInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();
                    procQuery(impInsertQuery, QString("UPDATE inner_db.imp SET abonent = \'%1\', last_save_source_time = \'%2\' WHERE inner_db.imp.id = %3")
                                                  .arg(addInfoJson["ab"].toString(), eventQuery.value("event_time").toString(), QString::number(lastImpId)));

                    break;
                }
                case IMP_OPERATOR_SEND_TRANSLATION: {
                    // Оператор отправил текст ИМП на трансляцию
                    if (!checkStatus(eventQuery, lastImpId, "imp", "id_res_create")) {
                        break;
                    }

                    QSqlQuery impInsertQuery(sourceDb);
                    procQuery(impInsertQuery, QString("UPDATE inner_db.imp SET translation_time = \'%1\' WHERE inner_db.imp.id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastImpId)));


                    break;
                }
                case IMP_TOP_READ: {
                    // Чтение ТОП при формировании ИМП
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_BI_READ: {
                    // Чтение БИ при формировании ИМП
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_TRANSLATION_COMPLETED: {
                    // Трансляция ИМП завершена
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_OPERATOR_CONFIRM_DB_WRITE: {
                    // Оператор подтвердил запись ИМП в базу данных
                    checkStatus(eventQuery, lastImpId, "imp", "id_res_create");
                    break;
                }
                case IMP_MCI_DB_WRITE: {
                    // Запись ИМП в базу данных МЦИ
                    if (!checkStatus(eventQuery, lastImpId, "imp", "id_res_create")) {
                        break;
                    }

                    createRes(true, eventQuery.value("description").toString(), lastImpId, "imp", "id_res_create");
                    break;
                }

                // МЦИ events
                case MCI_DB_TOP_CONNECTION: {
                    // Подключение к базе данных ТОП при формировании МЦИ
                    QSqlQuery spanQuery(sourceDb);
                    int inner_operator_id = checkOperator(spanQuery, span_id, session_id);
                    qDebug() << "Id текущего оператора формирования МЦИ: " << inner_operator_id;

                    QJsonDocument contextJsonDoc = QJsonDocument::fromJson(spanQuery.value("context").toString().toUtf8());
                    QJsonObject contextJson = contextJsonDoc.object();

                    QSqlQuery mciInsertQuery(sourceDb);
                    procQuery(mciInsertQuery, QString("INSERT INTO inner_db.mci (name, type, id_operator) VALUES (\'%1\', \'%2\', %3)")
                                                  .arg(contextJson["mci_name"].toString(), contextJson["mci_type"].toString(), QString::number(inner_operator_id)));

                    lastMciId = mciInsertQuery.lastInsertId().toInt();

                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");

                    break;
                }
                case MCI_DB_BI_CONNECTION: {
                    // Подключение к базе данных БИ при формировании МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_DB_CONNECTION: {
                    // Произошло подключение к базе данных МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_OPERATOR_ATTR_INPUT: {
                    // Оператор ввел атрибуты при формировании МЦИ
                    if (!checkStatus(eventQuery, lastImpId, "mci", "id_res_create")) {
                        break;
                    }

                    QSqlQuery mciInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    procQuery(mciInsertQuery, QString("UPDATE inner_db.mci SET abonent = \'%1\', turn = %2, krl = \'%3\', object = \'%4\', sbi = %5 WHERE inner_db.mci.id = %6")
                                                  .arg(addInfoJson["ab"].toString(), addInfoJson["turn"].toString(), addInfoJson["krl"].toString(), addInfoJson["obj"].toString(), addInfoJson["sbi"].toString(), QString::number(lastMciId)));


                    break;
                }
                case MCI_BI_READ_FOR_ATTR: {
                    // Чтение БИ при формировании атрибутов МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_ATTR_VALIDATION: {
                    // Проверка корректности атрибутов при формировании МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_OPERATOR_CREATE_OPEN: {
                    // Оператор создал/открыл текст МЦИ

                    if (!checkStatus(eventQuery, lastMciId, "mci", "id_res_create")) {
                        break;
                    }

                    QSqlQuery mciInsertQuery(sourceDb);
                    procQuery(mciInsertQuery, QString("UPDATE inner_db.mci SET last_save_source_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));


                    break;
                }
                case MCI_OPERATOR_SAVE: {
                    // Оператор сохранил текст МЦИ

                    if (!checkStatus(eventQuery, lastMciId, "mci", "id_res_create")) {
                        break;
                    }

                    QSqlQuery mciInsertQuery(sourceDb);
                    procQuery(mciInsertQuery, QString("UPDATE inner_db.mci SET last_save_source_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));

                    break;
                }
                case MCI_OPERATOR_SEND_TRANSLATION: {
                    // Оператор отправил текст МЦИ на трансляцию

                    if (!checkStatus(eventQuery, lastMciId, "mci", "id_res_create")) {
                        break;
                    }

                    QSqlQuery mciInsertQuery(sourceDb);
                    procQuery(mciInsertQuery, QString("UPDATE inner_db.mci SET translation_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));


                    break;
                }
                case MCI_TOP_READ: {
                    // Чтение ТОП при формировании МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_BI_READ: {
                    // Чтение БИ при формировании МЦИ
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_IMP_READ: {
                    // Чтение ИМП при формировании МЦИ
                    if (!checkStatus(eventQuery, lastMciId, "mci", "id_res_create")) {
                        break;
                    }

                    QSqlQuery impInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    procQuery(impInsertQuery, QString("SELECT id FROM inner_db.imp WHERE name = \'%1\'")
                                                  .arg(addInfoJson["imp_name"].toString()), false);

                    if (impInsertQuery.next()) {
                        QSqlQuery mciInsertQuery(sourceDb);
                        procQuery(mciInsertQuery, QString("UPDATE inner_db.mci SET id_imp = %1")
                                                      .arg(impInsertQuery.value("id").toString()));
                    } else {
                        qDebug() << "При формировании МЦИ не найден ИМП с именем" << addInfoJson["imp_name"].toString();
                    }

                    break;
                }
                case MCI_TRANSLATION_COMPLETED: {
                    // Трансляция МЦИ завершена
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_OPERATOR_SEND_PROCESSING: {
                    // Оператор отправил МЦИ на отработку
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_PROCESSING_COMPLETED: {
                    // Отработка МЦИ завершена
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_OPERATOR_CONFIRM_DB_WRITE: {
                    // Оператор подтвердил запись МЦИ в базу данных
                    checkStatus(eventQuery, lastMciId, "mci", "id_res_create");
                    break;
                }
                case MCI_DB_WRITE: {
                    // Запись МЦИ в базу данных

                    if (!checkStatus(eventQuery, lastMciId, "mci", "id_res_create")) {
                        break;
                    }

                    createRes(true, eventQuery.value("description").toString(), lastMciId, "mci", "id_res_create");
                    break;
                }

                // ПСС events
                case PSS_DB_CONNECTION: {
                    // Подключение к базе данных ПСС

                    QSqlQuery spanQuery(sourceDb);
                    int inner_operator_id = checkOperator(spanQuery, span_id, session_id);
                    qDebug() << "Id текущего оператора формирования ПСС: " << inner_operator_id;

                    QJsonDocument contextJsonDoc = QJsonDocument::fromJson(spanQuery.value("context").toString().toUtf8());
                    QJsonObject contextJson = contextJsonDoc.object();

                    QSqlQuery pssInsertQuery(sourceDb);
                    procQuery(pssInsertQuery, QString("INSERT INTO inner_db.pss (name, type, id_operator) VALUES (\'%1\', \'%2\', %3)")
                                                  .arg(contextJson["pss_name"].toString(), contextJson["pss_type"].toString(), QString::number(inner_operator_id)));

                    lastPssId = pssInsertQuery.lastInsertId().toInt();

                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");

                    break;
                }
                case PSS_DB_BI_CONNECTION: {
                    // Подключение к базе данных БИ при формировании ПСС
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_DB_MCI_CONNECTION: {
                    // Подключение к базе данных МЦИ при формировании ПСС
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_OPERATOR_ATTR_INPUT: {
                    // Оператор ввел атрибуты при формировании ПСС

                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    QSqlQuery pssInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    auto arrKisNum = addInfoJson["kis_nums"].toArray();
                    QString kisNumsStr = jsonArrToString(arrKisNum);

                    auto arrTurnNum = addInfoJson["turn_nums"].toArray();
                    QString turnNumsStr =jsonArrToString(arrTurnNum);

                    procQuery(pssInsertQuery, QString("UPDATE inner_db.pss SET kis_nums = \'%1\', turn_nums = \'%2\', krl = \'%3\', object = \'%4\', sbi = %5 WHERE inner_db.pss.id = %6")
                                                  .arg(kisNumsStr, turnNumsStr, addInfoJson["krl"].toString(), addInfoJson["obj"].toString(), addInfoJson["sbi"].toString(), QString::number(lastPssId)));



                    break;
                }
                case PSS_BI_READ_FOR_ATTR: {
                    // Чтение БИ при формировании атрибутов ПСС
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_ATTR_VALIDATION: {
                    // Проверка корректности атрибутов при формировании ПСС
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_OPERATOR_CREATE_OPEN: {
                    // Оператор создал/открыл текст ПСС

                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    QSqlQuery pssInsertQuery(sourceDb);
                    procQuery(pssInsertQuery, QString("UPDATE inner_db.pss SET last_save_source_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));


                    break;
                }
                case PSS_OPERATOR_SAVE: {
                    // Оператор сохранил текст ПСС

                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    QSqlQuery pssInsertQuery(sourceDb);
                    procQuery(pssInsertQuery, QString("UPDATE inner_db.pss SET last_save_source_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));


                    break;
                }
                case PSS_OPERATOR_SEND_TRANSLATION: {
                    // Оператор отправил текст ПСС на трансляцию

                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    QSqlQuery pssInsertQuery(sourceDb);
                    procQuery(pssInsertQuery, QString("UPDATE inner_db.pss SET translation_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));


                    break;
                }
                case PSS_MCI_DU_KU_SK_FK_READ: {
                    // Чтение МЦИ, ДУ, КУ, СК, ФК

                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    QSqlQuery pssInsertQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();



                    procQuery(pssInsertQuery, QString("UPDATE inner_db.pss SET structure = \'%1\' WHERE inner_db.pss.id = %2")
                                                  .arg(addInfoJsonDoc.toJson(QJsonDocument::Compact), QString::number(lastPssId)));

                    auto commArray = addInfoJson.value("commands").toArray();

                    foreach (const QJsonValue & value, commArray) {
                        QJsonObject commJson = value.toObject();

                        if (commJson.value("type").toString() != "mci") {

                            QSqlQuery commInsertQuery(sourceDb);
                            procQuery(commInsertQuery, QString("INSERT INTO inner_db.command (type, num, turn) VALUES (\'%1\', %2, %3)")
                                                          .arg(commJson.value("type").toString(), commJson.value("name").toString(), commJson.value("turn").toString()));

                        }
                    }

                    break;
                }
                case PSS_TRANSLATION_COMPLETED: {
                    // Трансляция ПСС завершена
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_OPERATOR_CONFIRM_DB_WRITE: {
                    // Оператор подтвердил запись ПСС в базу данных
                    checkStatus(eventQuery, lastPssId, "pss", "id_res_create");
                    break;
                }
                case PSS_DB_WRITE: {
                    // Запись ПСС в базу данных
                    if (!checkStatus(eventQuery, lastMciId, "pss", "id_res_create")) {
                        break;
                    }

                    createRes(true, eventQuery.value("description").toString(), lastPssId, "pss", "id_res_create");

                    break;
                }

                // МЦИ receipt events
                case MCI_PSS_SEND_PROCESSING: {
                    // Оператор отправил МЦИ на отработку
                    QSqlQuery mciPssTestQuery(sourceDb);
                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    typeObjSend = addInfoJson.value("type_obj").toString();

                    procQuery(mciPssTestQuery, QString("SELECT id FROM inner_db.%1 WHERE name = \'%2\' AND type = \'%3\' AND (SELECT inner_db.result.res FROM inner_db.result WHERE id = id_res_create) = True")
                                                  .arg(typeObjSend, addInfoJson["name"].toString(), addInfoJson["type"].toString()), false);

                    if (!mciPssTestQuery.next()) {
                        qDebug() << "Отсутствует запись об МЦИ/ПСС, отправвленного на отработку";
                        break;
                    }

                    lastMciIdTest = mciPssTestQuery.value("id").toInt();

                    checkStatus(eventQuery, lastMciIdTest, typeObjSend, "id_res_test");
                    break;
                }
                case MCI_PSS_TRANSPORT_RECEIPT: {

                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    typeObjSend = addInfoJson.value("type_obj").toString();

                    // Получена транспортная квитанция об отправке МЦИ на отработку
                    checkStatus(eventQuery, lastMciIdTest, typeObjSend, "id_res_test");
                    break;
                }
                case MCI_PSS_USER_RECEIPT: {
                    // Получена пользовательская квитанция о результате отработки МЦИ
                    if (!checkStatus(eventQuery, lastMciIdTest, typeObjSend, "id_res_test")) {
                        break;
                    }

                    createRes(true, eventQuery.value("description").toString(), lastMciIdTest, typeObjSend, "id_res_test");

                    break;
                }

                // КИС events
                case KIS_DB_PSS_CONNECTION: {
                    // Подключение к базе данных ПСС при выдаче КПИ

                    QSqlQuery spanQuery(sourceDb);
                    int inner_operator_id = checkOperator(spanQuery, span_id, session_id);
                    qDebug() << "Id текущего оператора выдачи КПИ: " << inner_operator_id;

                    QJsonDocument contextJsonDoc = QJsonDocument::fromJson(spanQuery.value("context").toString().toUtf8());
                    QJsonObject contextJson = contextJsonDoc.object();

                    QSqlQuery sendInsertQuery(sourceDb);
                    procQuery(sendInsertQuery, QString("INSERT INTO inner_db.send (krl, object, id_operator) VALUES (\'%1\', \'%2\', %3)")
                                                  .arg(contextJson["krl"].toString(), contextJson["object"].toString(), QString::number(inner_operator_id)));

                    lastSendIdTest = sendInsertQuery.lastInsertId().toInt();

                    qDebug() << lastSendIdTest;

                    checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send");

                    break;
                }
                case KIS_DB_MCI_CONNECTION: {
                    // Подключение к базе данных МЦИ при выдаче КПИ
                    checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send");
                    break;
                }
                case KIS_OPERATOR_RESERVE: {
                    // Оператор зарезервировал КИС

                    if (!checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send")) {
                        break;
                    }

                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    QSqlQuery sendQuery(sourceDb);
                    procQuery(sendQuery, QString("UPDATE inner_db.send SET kis = %1 WHERE id = %2")
                                            .arg(addInfoJson.value("kis_num").toString(), QString::number(lastSendIdTest)));


                    break;
                }
                case KIS_OPERATOR_SELECT_COMMAND: {
                    // Оператор выбрал команду/ПСС
                    if (!checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send")) {
                        break;
                    }

                    QJsonDocument addInfoJsonDoc = QJsonDocument::fromJson(eventQuery.value("add_info").toString().toUtf8());
                    QJsonObject addInfoJson = addInfoJsonDoc.object();

                    typeObjSend = addInfoJson.value("type_obj").toString();

                    QSqlQuery mciPssTestQuery(sourceDb);
                    procQuery(mciPssTestQuery, QString("SELECT id FROM inner_db.%1 WHERE name = \'%2\' AND type = \'%3\' AND (SELECT inner_db.result.res FROM inner_db.result WHERE id = id_res_create) = True")
                                                  .arg(typeObjSend, addInfoJson["name"].toString(), addInfoJson["type"].toString()), false);

                    if (!mciPssTestQuery.next()) {
                        qDebug() << "Отсутствует запись об МЦИ/ПСС, отправвленного на отработку";
                        break;
                    }

                    QString id = mciPssTestQuery.value("id").toString();
                    procQuery(mciPssTestQuery, QString("UPDATE inner_db.%1 SET id_send = %2 WHERE id = %3")
                                                  .arg(typeObjSend, QString::number(lastSendIdTest), id));

                    break;
                }
                case KIS_OPERATOR_ISSUE_COMMAND: {
                    // Оператор выдал команду

                    if (!checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send")) {
                        break;
                    }

                    QSqlQuery sendInsertQuery(sourceDb);
                    procQuery(sendInsertQuery, QString("UPDATE inner_db.send SET send_time = \'%1\' WHERE id = %2")
                                                  .arg(eventQuery.value("event_time").toString(), QString::number(lastMciId)));

                    break;
                }
                case KIS_COMMAND_RESULT: {
                    // Получение результата выполнения команды

                    if (!checkStatus(eventQuery, lastSendIdTest, "send", "id_res_send")) {
                        break;
                    }

                    createRes(true, eventQuery.value("description").toString(), lastSendIdTest, "send", "id_res_send");

                    break;
                }

                default: {
                    // Неизвестное событие
                    break;
                }
            }

            lastUpdateInnerDb = QDateTime::currentDateTime();

            if (config != "") {
                QFile file(config);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    config = "";
                    qDebug() << "Не удалось открыть файл с настройками для записи.";
                } else {
                    QTextStream out(&file);
                    out << QString("{ \"last_update\":\"%1\" }").arg(lastUpdateInnerDb.toString("yyyy-MM-dd HH:mm:ss"));
                    file.close();
                }
            }

        }

        QThread::sleep(5);
    }
}
