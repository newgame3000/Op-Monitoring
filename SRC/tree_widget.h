#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "functions.h"
#include "event_blocks_widget.h"

struct queryRes
{
    int id;
    QString name;
    QString type;
    bool res_create;
    QString description_create;
    QString create_time;

    bool res_test = true;
    QString description_test = "";

    bool res_send = true;
    QString description_send = "";
    QString send_time = "";

    QString json = "";

};

struct blockStatus {
    QColor color;
    QString text;
};

class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);

    QVector<QString> searchNames = {
        "ИМП",
        "МЦИ",
        "ПСС"
    };

    QComboBox * searchBox;
    QLineEdit * searchEdit;
    QLabel * label;
    QComboBox * variantsBox;
    QPushButton * but;
    EventBlocksWidget * eventBlock;
    QSqlDatabase db;

    QVector<QVector<blockStatus>> blocksStatus;
    // QVector<QVector<QString>> variantNames;

    // QVector<blockStatus> mciBlocksStatus;
    // QVector<blockStatus> testBlocksStatus;
    // QVector<blockStatus> sendBlocksStatus;

public slots:
    void executeReq();
    void drawBlocks(int id);
    void changedSearchBox();

signals:

private:
    QVector<queryRes> impSearchImpName(QString impName);
    QVector<queryRes> mciSearchMciName(QString mciName);
    QVector<queryRes> pssSearchPssName(QString pssName);

    QVector<queryRes> impSearchMciId(int mciId);
    QVector<queryRes> mciSearchImpId(int impId);
    QVector<queryRes> pssSearchMciId(int mciId);

};

#endif // TREE_WIDGET_H
