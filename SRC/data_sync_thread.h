#ifndef DATA_SYNC_THREAD_H
#define DATA_SYNC_THREAD_H

#include <QThread>
#include <QMutex>
#include <QSqlDatabase>
#include <QWaitCondition>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

#include "functions.h"

enum EventType {
    // ИМП events
    IMP_DB_MCI_CONNECTION = 0,            // Подключение к базе данных МЦИ при формировании ИМП
    IMP_DB_TOP_CONNECTION = 1,            // Подключение к базе данных ТОП при формировании ИМП
    IMP_DB_BI_CONNECTION = 2,             // Подключение к базе данных БИ при формировании ИМП
    IMP_OPERATOR_CREATE_OPEN = 3,         // Оператор создал/открыл текст ИМП
    IMP_OPERATOR_SAVE = 4,                // Оператор сохранил текст ИМП
    IMP_OPERATOR_SEND_TRANSLATION = 5,    // Оператор отправил текст ИМП на трансляцию
    IMP_TOP_READ = 6,                     // Чтение ТОП при формировании ИМП
    IMP_BI_READ = 7,                      // Чтение БИ при формировании ИМП
    IMP_TRANSLATION_COMPLETED = 8,        // Трансляция ИМП завершена
    IMP_OPERATOR_CONFIRM_DB_WRITE = 9,    // Оператор подтвердил запись ИМП в базу данных
    IMP_MCI_DB_WRITE = 10,                // Запись ИМП в базу данных МЦИ

    // МЦИ events
    MCI_DB_TOP_CONNECTION = 11,           // Подключение к базе данных ТОП при формировании МЦИ
    MCI_DB_BI_CONNECTION = 12,            // Подключение к базе данных БИ при формировании МЦИ
    MCI_DB_CONNECTION = 13,               // Произошло подключение к базе данных МЦИ
    MCI_OPERATOR_ATTR_INPUT = 14,         // Оператор ввел атрибуты при формировании МЦИ
    MCI_BI_READ_FOR_ATTR = 15,            // Чтение БИ при формировании атрибутов МЦИ
    MCI_ATTR_VALIDATION = 16,             // Проверка корректности атрибутов при формировании МЦИ
    MCI_OPERATOR_CREATE_OPEN = 17,        // Оператор создал/открыл текст МЦИ
    MCI_OPERATOR_SAVE = 18,               // Оператор сохранил текст МЦИ
    MCI_OPERATOR_SEND_TRANSLATION = 19,   // Оператор отправил текст МЦИ на трансляцию
    MCI_TOP_READ = 20,                    // Чтение ТОП при формировании МЦИ
    MCI_BI_READ = 21,                     // Чтение БИ при формировании МЦИ
    MCI_IMP_READ = 22,                    // Чтение ИМП при формировании МЦИ
    MCI_TRANSLATION_COMPLETED = 23,       // Трансляция МЦИ завершена
    MCI_OPERATOR_SEND_PROCESSING = 24,    // Оператор отправил МЦИ на отработку
    MCI_PROCESSING_COMPLETED = 25,        // Отработка МЦИ завершена
    MCI_OPERATOR_CONFIRM_DB_WRITE = 26,   // Оператор подтвердил запись МЦИ в базу данных
    MCI_DB_WRITE = 27,                    // Запись МЦИ в базу данных

    // ПСС events
    PSS_DB_CONNECTION = 28,               // Подключение к базе данных ПСС
    PSS_DB_BI_CONNECTION = 29,            // Подключение к базе данных БИ при формировании ПСС
    PSS_DB_MCI_CONNECTION = 30,           // Подключение к базе данных МЦИ при формировании ПСС
    PSS_OPERATOR_ATTR_INPUT = 31,         // Оператор ввел атрибуты при формировании ПСС
    PSS_BI_READ_FOR_ATTR = 32,            // Чтение БИ при формировании атрибутов ПСС
    PSS_ATTR_VALIDATION = 33,             // Проверка корректности атрибутов при формировании ПСС
    PSS_OPERATOR_CREATE_OPEN = 34,        // Оператор создал/открыл текст ПСС
    PSS_OPERATOR_SAVE = 35,               // Оператор сохранил текст ПСС
    PSS_OPERATOR_SEND_TRANSLATION = 36,   // Оператор отправил текст ПСС на трансляцию
    PSS_MCI_DU_KU_SK_FK_READ = 37,        // Чтение МЦИ, ДУ, КУ, СК, ФК
    PSS_TRANSLATION_COMPLETED = 38,       // Трансляция ПСС завершена
    PSS_OPERATOR_CONFIRM_DB_WRITE = 39,   // Оператор подтвердил запись ПСС в базу данных
    PSS_DB_WRITE = 40,                    // Запись ПСС в базу данных

    // МЦИ/ПСС receipt events
    MCI_PSS_SEND_PROCESSING = 41,         // Оператор отправил МЦИ/ПСС на отработку
    MCI_PSS_TRANSPORT_RECEIPT = 42,       // Получена транспортная квитанция об отправке МЦИ/ПСС на отработку
    MCI_PSS_USER_RECEIPT = 43,            // Получена пользовательская квитанция о результате отработки МЦИ/ПСС

    // КИС events
    KIS_DB_PSS_CONNECTION = 44,           // Подключение к базе данных ПСС при выдаче КПИ
    KIS_DB_MCI_CONNECTION = 45,           // Подключение к базе данных МЦИ при выдаче КПИ
    KIS_OPERATOR_RESERVE = 46,            // Оператор зарезервировал КИС
    KIS_OPERATOR_SELECT_COMMAND = 47,     // Оператор выбрал команду/ПСС
    KIS_OPERATOR_ISSUE_COMMAND = 48,      // Оператор выдал команду
    KIS_COMMAND_RESULT = 49               // Получение результата выполнения команды
};

enum Severity {
    INFO = 0, //Информация
    WARNING = 1, //Предупреждение
    ERROR = 2 //Ошибка
};

class DataSyncThread : public QThread {
    Q_OBJECT

private:
    QSqlDatabase sourceDb;
    QMutex mutex;
    QWaitCondition condition;
    bool isRunning;

    QString config;
    QDateTime lastUpdateInnerDb;

    int lastImpId = -1;
    int lastMciId = -1;
    int lastPssId = -1;
    int lastMciIdTest = -1;
    int lastSendIdTest = -1;
    QString typeObjSend = "";


    void createRes(bool res, QString description, int id, QString table, QString field,  QString error_code = "");
    bool checkStatus(QSqlQuery eventQuery, int &idx, QString table, QString field);
    // void procQuery(QSqlQuery &eventQuery, QString execText, bool next = true);
    int checkOperator(QSqlQuery & spanQuery, int span_id, int session_id);
    QString jsonArrToString(QJsonArray arr);


public:
    explicit DataSyncThread(QObject *parent = nullptr);

    void stop() {
        mutex.lock();
        isRunning = false;
        condition.wakeOne();
        mutex.unlock();
    }

protected:
    void run() override;
};


#endif // DATA_SYNC_THREAD_H
