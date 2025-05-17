#ifndef SEARCH_WIDGET_H
#define SEARCH_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSqlTableModel>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QSplitter>

#include "gui/table_label.h"
#include "gui/add_widgets.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

private:
    QComboBox *searchComboBox;
    QSqlDatabase db;

    AddWidgetField * addWidgetField;
    AddWidgetDateFields * addWidgetDateFields;


    int modelsCount = 4;

    // QSqlQueryModel *mciQueryModel;
    // QSqlQueryModel *impQueryModel;
    // QSqlQueryModel *commandQueryModel;
    // QSqlQueryModel *operatorQueryModel;
    // QSqlQueryModel *pssQueryModel;

    QVector<TableLabel *> tableLabels;

    QVector<QString> searchComboNames = {
        "Информация о КПИ на определенном витке",
        "Информация о формировании КПИ в определенный промежуток времени",
        "Информация о выдаче КПИ в определенный промежуток времени",
        "Информация о КПИ, выданной на определенный космический аппарат",


    };

public slots:
    void executeReq();
    void searchComboChanged(int index);
    void clearAndShowTable();
    void clearAndHideTable();

};

#endif // SEARCH_WIDGET_H
