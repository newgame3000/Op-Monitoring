#include "search_widget.h"
#include "expandable_item_delegate.h"
#include "functions.h"


SearchWidget::SearchWidget(QWidget *parent)
    : QWidget{parent}
{

    db = QSqlDatabase::database("view_connection");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базы данных";
        qDebug() << "Ошибка источника:" << db.lastError().text();
        return;
    }

    QVBoxLayout * searchlayout = new QVBoxLayout;
    searchlayout->setSpacing(5);

    searchComboBox = new QComboBox(this);

    for (int i = 0; i < searchComboNames.size(); ++i) {
        searchComboBox->addItem(searchComboNames[i]);
    }

    QHBoxLayout * addLayout = new QHBoxLayout;
    addLayout->setMargin(0);

    addWidgetField = new AddWidgetField(this);
    addWidgetField->setText("Виток: ");

    addWidgetDateFields = new AddWidgetDateFields(this);

    QPushButton * but = new QPushButton(this);
    but->setText("Выполнить");

    addLayout->addWidget(addWidgetField);
    addLayout->addWidget(addWidgetDateFields);
    addLayout->addWidget(but);
    addLayout->setStretch(0, 5);
    addLayout->setStretch(1, 5);
    addLayout->setStretch(2, 1);

    addWidgetDateFields->hide();

    //Инициализация отображения таблиц
    tableLabels = QVector<TableLabel *>(modelsCount);

    QSplitter * verticalSplitter = new QSplitter(this);
    QSplitter * horizontalSplitter1 = new QSplitter(this);
    QSplitter * horizontalSplitter2 = new QSplitter(this);
    verticalSplitter->setOrientation(Qt::Vertical);
    horizontalSplitter1->setOrientation(Qt::Horizontal);
    horizontalSplitter2->setOrientation(Qt::Horizontal);

    verticalSplitter->addWidget(horizontalSplitter1);
    verticalSplitter->addWidget(horizontalSplitter2);

    for (int i = 0; i < modelsCount; ++i) {

        tableLabels[i] = new TableLabel(this);

        if (i % 2 == 0) {
            horizontalSplitter1->addWidget(tableLabels[i]);
        } else {
            horizontalSplitter2->addWidget(tableLabels[i]);
        }

    }


    searchlayout->addWidget(searchComboBox);
    searchlayout->addLayout(addLayout);
    searchlayout->addWidget(verticalSplitter);

    setLayout(searchlayout);

    connect(but, &QPushButton::clicked, this, &SearchWidget::executeReq);
    connect(searchComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &SearchWidget::searchComboChanged);
}

void SearchWidget::executeReq() {

    int boxIndex = searchComboBox->currentIndex();
    clearAndHideTable();

    switch (boxIndex) {

        case 0: {

            addWidgetField->setText("Виток: ");

            QVector<QString> req;

            req.push_back(QString(
                              "SELECT "
                              "    mci.name, "
                              "    mci.type, "
                              "    mci.turn, "
                              "    send.send_time, "
                              "    result.res, "
                              "    result.description, "
                              "    send.kis, "
                              "    send.krl, "
                              "    send.object, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.mci "
                              "LEFT JOIN "
                              "    inner_db.send ON mci.id_send = send.id "
                              "LEFT JOIN "
                              "    inner_db.result ON send.id_res_send = result.id "
                              "JOIN "
                              "    inner_db.operator ON mci.id_operator = operator.id "
                              "WHERE "
                              "    mci.turn = %1"
                              ).arg(addWidgetField->text().toInt()));

            req.push_back(QString(
                              "SELECT "
                              "    command.num, "
                              "    command.type, "
                              "    command.turn, "
                              "    send.send_time, "
                              "    send.object, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.command "
                              "LEFT JOIN "
                              "    inner_db.send ON send.id = command.id_send "
                              "LEFT JOIN "
                              "    inner_db.operator ON operator.id = send.id_operator "
                              "WHERE "
                              "    command.turn = %1"
                              ).arg(addWidgetField->text().toInt()));

            req.push_back(QString(
                              "SELECT "
                              "    pss.name, "
                              "    pss.type, "
                              "    pss.turn_nums, "
                              "    send.send_time, "
                              "    result.res, "
                              "    result.description, "
                              "    send.kis, "
                              "    send.krl, "
                              "    send.object, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.pss "
                              "LEFT JOIN "
                              "    inner_db.send ON pss.id_send = send.id "
                              "LEFT JOIN "
                              "    inner_db.result ON send.id_res_send = result.id "
                              "JOIN "
                              "    inner_db.operator ON pss.id_operator = operator.id "
                              "WHERE "
                              "    %1 = ANY(pss.turn_nums)"
                              ).arg(addWidgetField->text().toInt()));

            QVector<QString> names = { "МЦИ", "Команды", "ПСС"};

            bool found = false;

            for (int i = 0; i < req.size(); ++i) {
                QSqlQuery searchTestQuery(db);
                procQuery(searchTestQuery, req[i], false);

                if (searchTestQuery.next()) {
                    found = true;
                    tableLabels[i]->setQuery(searchTestQuery);
                    tableLabels[i]->setText(names[i]);
                    tableLabels[i]->show();
                }
            }

            if (!found) {
                clearAndShowTable();
            }

            break;
        }

        case 1: {

            QString startTime = addWidgetDateFields->getStartDateEdit().toString("yyyy-MM-dd HH:mm:ss");
            QString endTime = addWidgetDateFields->getEndDateEdit().toString("yyyy-MM-dd HH:mm:ss");;

            QVector<QString> req;

            req.push_back(QString(
                              "SELECT "
                              "    imp.id, "
                              "    imp.name, "
                              "    imp.type, "
                              "    imp.abonent, "
                              "    imp.last_save_source_time, "
                              "    imp.translation_time, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.imp "
                              "JOIN "
                              "    inner_db.operator ON imp.id_operator = operator.id "
                              "WHERE "
                              "    (imp.translation_time BETWEEN '%1' AND '%2') "
                              "ORDER BY "
                              "    imp.translation_time;"
                              ).arg(startTime, endTime));

            req.push_back(QString(
                              "SELECT "
                              "    mci.id, "
                              "    mci.name, "
                              "    mci.type, "
                              "    mci.abonent, "
                              "    mci.turn, "
                              "    mci.krl, "
                              "    mci.object, "
                              "    operator.login AS operator_login, "
                              "    mci.last_save_source_time, "
                              "    mci.translation_time "
                              "FROM "
                              "    inner_db.mci "
                              "JOIN "
                              "    inner_db.operator ON mci.id_operator = operator.id "
                              "WHERE "
                              "    (mci.translation_time BETWEEN '%1' AND '%2') "
                              "ORDER BY "
                              "     mci.translation_time;"
                              ).arg(startTime, endTime));

            req.push_back(QString(
                                   "SELECT "
                                   "    pss.id, "
                                   "    pss.name, "
                                   "    pss.type, "
                                   "    pss.kis_nums, "
                                   "    pss.turn_nums, "
                                   "    pss.krl, "
                                   "    pss.object, "
                                   "    operator.login AS operator_login, "
                                   "    pss.last_save_source_time, "
                                   "    pss.translation_time "
                                   "FROM "
                                   "    inner_db.pss "
                                   "JOIN "
                                   "    inner_db.operator ON pss.id_operator = operator.id "
                                   "WHERE "
                                   "    (pss.translation_time BETWEEN '%1' AND '%2') "
                                   "ORDER BY "
                                   "    pss.translation_time;"
                              ).arg(startTime, endTime));

            QVector<QString> names = {"ИМП", "МЦИ", "ПСС"};

            bool found = false;

            for (int i = 0; i < req.size(); ++i) {
                QSqlQuery searchTestQuery(db);
                procQuery(searchTestQuery, req[i], false);

                if (searchTestQuery.next()) {
                    found = true;
                    tableLabels[i]->setQuery(searchTestQuery);
                    tableLabels[i]->setText(names[i]);
                    tableLabels[i]->show();
                }
            }

            if (!found) {
                clearAndShowTable();
            }

            break;
        }

        case 2: {

            QString startTime = addWidgetDateFields->getStartDateEdit().toString("yyyy-MM-dd HH:mm:ss");
            QString endTime = addWidgetDateFields->getEndDateEdit().toString("yyyy-MM-dd HH:mm:ss");;

            QVector<QString> req;

            req.push_back(QString(
                              "SELECT "
                              "    mci.id, "
                              "    mci.name, "
                              "    mci.type, "
                              "    mci.abonent, "
                              "    mci.turn, "
                              "    mci.krl, "
                              "    mci.object, "
                              "    mci.sbi, "
                              "    mci.last_save_source_time, "
                              "    mci.translation_time, "
                              "    operator.login AS operator_login, "
                              "    send.send_time "
                              "FROM "
                              "    inner_db.mci "
                              "JOIN "
                              "    inner_db.operator ON mci.id_operator = operator.id "
                              "JOIN "
                              "    inner_db.send ON mci.id_send = send.id "
                              "WHERE "
                              "    (send.send_time BETWEEN '%1' AND '%2') "
                              "ORDER BY "
                              "    send.send_time;"
                              ).arg(startTime, endTime));

            req.push_back(QString(
                              "SELECT "
                              "    command.id, "
                              "    command.num, "
                              "    command.turn, "
                              "    command.type, "
                              "    operator.login AS operator_login, "
                              "    send.send_time, "
                              "    send.object, "
                              "    send.krl, "
                              "    send.kis "
                              "FROM "
                              "    inner_db.command "
                              "JOIN "
                              "    inner_db.send ON command.id_send = send.id "
                              "JOIN "
                              "    inner_db.operator ON send.id_operator = operator.id "
                              "WHERE "
                              "    (send.send_time BETWEEN '%1' AND '%2') "
                              "ORDER BY "
                              "    send.send_time;"
                              ).arg(startTime, endTime));

            req.push_back(QString(
                              "SELECT "
                              "    pss.id, "
                              "    pss.name, "
                              "    pss.type, "
                              "    pss.krl, "
                              "    pss.object, "
                              "    pss.sbi, "
                              "    pss.last_save_source_time, "
                              "    pss.translation_time, "
                              "    operator.login AS operator_login, "
                              "    send.send_time "
                              "FROM "
                              "    inner_db.pss "
                              "JOIN "
                              "    inner_db.operator ON pss.id_operator = operator.id "
                              "JOIN "
                              "    inner_db.send ON pss.id_send = send.id "
                              "WHERE "
                              "    (send.send_time BETWEEN '%1' AND '%2') "
                              "ORDER BY "
                              "    send.send_time;"
                              ).arg(startTime, endTime));

            QVector<QString> names = { "МЦИ", "Команды", "ПСС"};

            bool found = false;

            for (int i = 0; i < req.size(); ++i) {
                QSqlQuery searchTestQuery(db);
                procQuery(searchTestQuery, req[i], false);

                if (searchTestQuery.next()) {
                    found = true;
                    tableLabels[i]->setQuery(searchTestQuery);
                    tableLabels[i]->setText(names[i]);
                    tableLabels[i]->show();
                }
            }

            if (!found) {
                clearAndShowTable();
            }

            break;
        }

        case 3: {

            QVector<QString> req;

            req.push_back(QString(
                              "SELECT "
                              "    mci.id, "
                              "    mci.name, "
                              "    mci.type, "
                              "    mci.abonent, "
                              "    mci.turn, "
                              "    mci.krl, "
                              "    mci.object, "
                              "    mci.sbi, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.mci "
                              "JOIN "
                              "    inner_db.operator ON mci.id_operator = operator.id "
                              "WHERE "
                              "    mci.object = '%1' "
                              ).arg(addWidgetField->text().toInt()));

            req.push_back(QString(
                              "SELECT "
                              "    command.id, "
                              "    command.num, "
                              "    command.turn, "
                              "    command.type, "
                              "    operator.login AS operator_login, "
                              "    send.object "
                              "FROM "
                              "    inner_db.command "
                              "JOIN "
                              "    inner_db.send ON command.id_send = send.id "
                              "JOIN "
                              "    inner_db.operator ON send.id_operator = operator.id "
                              "WHERE "
                              "    send.object = '%1' "
                              ).arg(addWidgetField->text().toInt()));

            req.push_back(QString(
                              "SELECT "
                              "    pss.id, "
                              "    pss.name, "
                              "    pss.type, "
                              "    pss.krl, "
                              "    pss.object, "
                              "    pss.sbi, "
                              "    pss.last_save_source_time, "
                              "    pss.translation_time, "
                              "    operator.login AS operator_login "
                              "FROM "
                              "    inner_db.pss "
                              "JOIN "
                              "    inner_db.operator ON pss.id_operator = operator.id "
                              "WHERE "
                              "    pss.object = '%1' "
                              ).arg(addWidgetField->text().toInt()));

            QVector<QString> names = { "МЦИ", "Команды", "ПСС"};

            bool found = false;

            for (int i = 0; i < req.size(); ++i) {
                QSqlQuery searchTestQuery(db);
                procQuery(searchTestQuery, req[i], false);

                if (searchTestQuery.next()) {
                    found = true;
                    tableLabels[i]->setQuery(searchTestQuery);
                    tableLabels[i]->setText(names[i]);
                    tableLabels[i]->show();
                }
            }

            if (!found) {
                clearAndShowTable();
            }

            break;
        }
    }
}

void SearchWidget::searchComboChanged(int index) {
    clearAndShowTable();

    qDebug() << index;

    switch(index) {
        case 0: {
            addWidgetField->setText("Виток: ");
            addWidgetField->show();
            addWidgetDateFields->hide();
            break;
        }

        case 1:
        case 2: {
            addWidgetField->hide();
            addWidgetDateFields->show();
            break;
        }

        case 3: {
            addWidgetField->setText("КА: ");
            addWidgetField->show();
            addWidgetDateFields->hide();
            break;
        }
    }

}

void SearchWidget::clearAndShowTable() {
    for (int i = 0; i < modelsCount; ++i) {
        tableLabels[i]->clear();
        tableLabels[i]->show();
    }
}

void SearchWidget::clearAndHideTable() {
    for (int i = 0; i < modelsCount; ++i) {
        tableLabels[i]->clear();
        tableLabels[i]->hide();
    }
}
