#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QComboBox>

#include "data_sync_thread.h"
#include "search_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void traceComboChanged(int id);
    void innerComboChanged(int id);

private:
    QSqlTableModel *traceTableModel;
    QSqlTableModel *innerTableModel;

    QComboBox *innerComboBox;
    QComboBox *traceComboBox;

    QVector<QString> traceNames = {"session", "span", "event"};
    QVector<QString> innerNames = {"operator", "imp", "mci", "pss", "command", "send", "result"};

    DataSyncThread *syncThread;

    SearchWidget * searchWidget;

};
#endif // MAINWINDOW_H
