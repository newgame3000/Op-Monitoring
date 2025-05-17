#include "mainwindow.h"
#include "expandable_item_delegate.h"
#include "tree_widget.h"

#include <QDebug>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHeaderView>
#include <QObject>

#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "view_connection");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("vkm");
    db.setPort(5432);


    if (!db.open()) {
        qDebug() << "Не удалось открыть базы данных";
        qDebug() << "Ошибка источника:" << db.lastError().text();
        return;//???
    }

    qDebug() << "Подключено к БД главнй поток";

    QTabWidget * tabs = new QTabWidget(this);

    QFrame *traceFrame = new QFrame(this);
    QVBoxLayout * tracelayout = new QVBoxLayout;
    tracelayout->setSpacing(5);

    traceComboBox = new QComboBox(this);
    traceComboBox->setContentsMargins(10, 10, 10, 10);
    for (int i = 0; i < traceNames.size(); ++i) {
        traceComboBox->addItem(traceNames[i]);
    }

    traceTableModel = new QSqlTableModel(this, db);
    traceTableModel->setTable("trace_2025." + traceNames[0]);
    traceTableModel->select();

    QTableView * traceTableView = new QTableView(this);
    traceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    traceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    traceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    traceTableView->setSelectionMode(QAbstractItemView::NoSelection);
    traceTableView->setItemDelegate(new ExpandableItemDelegate(traceTableView));
    traceTableView->setModel(traceTableModel);

    tracelayout->addWidget(traceComboBox);
    tracelayout->addWidget(traceTableView);

    traceFrame->setLayout(tracelayout);

    QFrame *innerFrame = new QFrame(this);
    QVBoxLayout * innerlayout = new QVBoxLayout;
    innerlayout->setSpacing(5);

    innerComboBox = new QComboBox(this);
    innerComboBox->setContentsMargins(10, 10, 10, 10);
    for (int i = 0; i < innerNames.size(); ++i) {
        innerComboBox->addItem(innerNames[i]);
    }

    innerTableModel = new QSqlTableModel(this, db);
    innerTableModel->setTable("inner_db." + innerNames[0]);
    innerTableModel->select();

    QTableView * innerTableView = new QTableView(this);
    innerTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    innerTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    innerTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    innerTableView->horizontalHeader()->setMinimumSectionSize(200);
    innerTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    innerTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    innerTableView->setSelectionMode(QAbstractItemView::NoSelection);
    innerTableView->setItemDelegate(new ExpandableItemDelegate(innerTableView));
    innerTableView->setModel(innerTableModel);

    innerTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    innerlayout->addWidget(innerComboBox);
    innerlayout->addWidget(innerTableView);

    innerFrame->setLayout(innerlayout);

    searchWidget = new SearchWidget(this);



    TreeWidget *treeWidget = new TreeWidget(this);


    tabs->addTab(traceFrame, "База данных журналов СПО");
    tabs->addTab(innerFrame, "Внутренняя база данных");
    tabs->addTab(searchWidget, "Анализ данных о действиях операторов");
    tabs->addTab(treeWidget, "Дерево событий");

    this->setCentralWidget(tabs);

    QObject::connect(traceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::traceComboChanged);
    QObject::connect(innerComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::innerComboChanged);
    connect(tabs, &QTabWidget::currentChanged, this, [tabs, this](int id) {
        if (id == 0) {
            traceComboChanged(traceComboBox->currentIndex());
        }

        if (id == 1) {
            innerComboChanged(innerComboBox->currentIndex());
        }
    });
    // Запуск потока синхронизации
    syncThread = new DataSyncThread(this);
    syncThread->start();
}

void MainWindow::traceComboChanged(int id) {
    traceTableModel->setTable("trace_2025." + traceNames[id]);
    traceTableModel->select();
}

void MainWindow::innerComboChanged(int id) {
    innerTableModel->setTable("inner_db." + innerNames[id]);
    innerTableModel->select();
}


MainWindow::~MainWindow() {




}
