#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

static void procQuery(QSqlQuery &eventQuery, QString execText, bool next = true) {
    if (!eventQuery.exec(execText)) {
        qDebug() << "Ошибка при выполнении запроса к базе данных";
        qDebug() << "Текст запроса: " << execText;
        qDebug() << "Ошибка: " << eventQuery.lastError();
        return;
    }

    if (next) {
        eventQuery.next();
    }
    return;
}


#endif // FUNCTIONS_H
