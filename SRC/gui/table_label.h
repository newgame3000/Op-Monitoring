#ifndef TABLE_LABEL_H
#define TABLE_LABEL_H

#include <QWidget>
#include <QTableView>
#include <QLabel>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QSqlQuery>

class TableLabel : public QWidget
{
    Q_OBJECT
public:
    explicit TableLabel(QWidget *parent = nullptr);

    void setQuery(QSqlQuery searchTestQuery) {
        model->setQuery(searchTestQuery);
    }

    void setText(QString text) {
        label->setText(text);
    }

    void clear() {
        label->setText("");
        model->clear();
    }

private:
    QTableView *table;
    QLabel * label;
    QSqlQueryModel * model;

signals:
};

#endif // TABLE_LABEL_H
