#include "tree_widget.h"
#include <QDebug>

TreeWidget::TreeWidget(QWidget *parent)
    : QWidget{parent}
{
    db = QSqlDatabase::database("view_connection");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базы данных";
        qDebug() << "Ошибка источника:" << db.lastError().text();
        return;
    }

    QVBoxLayout * mainVLayout = new QVBoxLayout;

    searchBox = new QComboBox(this);
    for (int i = 0; i < searchNames.size(); ++i) {
        searchBox->addItem(searchNames[i]);
    }

    QHBoxLayout * hLayt = new QHBoxLayout;
    hLayt->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(this);
    label->setText("Имя: ");
    searchEdit = new QLineEdit(this);
    but = new QPushButton(this);
    but->setText("Выполнить");

    hLayt->addWidget(label);
    hLayt->addWidget(searchEdit);
    hLayt->addWidget(but);

    hLayt->setStretch(0, 1);
    hLayt->setStretch(1, 15);
    hLayt->setStretch(2, 3);

    QFrame * frame = new QFrame(this);
    frame->setFrameStyle(QFrame::Panel);
    QHBoxLayout * hFrameLayt = new QHBoxLayout;
    eventBlock = new EventBlocksWidget(this);
    hFrameLayt->addWidget(eventBlock);
    frame->setLayout(hFrameLayt);

    variantsBox = new QComboBox(this);

    mainVLayout->addWidget(searchBox);
    mainVLayout->addLayout(hLayt);
    mainVLayout->addWidget(variantsBox);
    mainVLayout->addWidget(frame);

    variantsBox->hide();

    setLayout(mainVLayout);

    connect(but, &QPushButton::clicked, this, &TreeWidget::executeReq);
    connect(searchBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &TreeWidget::changedSearchBox);
}

void TreeWidget::executeReq() {

    blocksStatus.clear();
    eventBlock->clear();
    variantsBox->clear();
    variantsBox->show();

    switch(searchBox->currentIndex()) {

        qDebug() << searchBox->currentIndex();

        //ИМП
        case 0: {
            QString impName = searchEdit->text();

            QVector<queryRes> resImp = impSearchImpName(impName);
            qDebug() << "sizeImp" << resImp.size();

            for (int i = 0; i < resImp.size(); ++i) {

                QColor color = Qt::green;
                if (!resImp[i].res_create) {
                    color = Qt::red;
                }

                QString text = "\nИмя ИМП: " + resImp[i].name + "\nТип: " +
                               resImp[i].type + "\n" + resImp[i].description_create +
                               "\nВремя трансляции: " + resImp[i].create_time;

                blockStatus impBlockStatus = {color, text};

                QVector<queryRes> resMci = mciSearchImpId(resImp[i].id);
                qDebug() << "sizeMci" << resMci.size();

                //Если не нашлось дальше МЦИ, в который входит ИМП
                if (resMci.size() == 0) {
                    blocksStatus.push_back(QVector<blockStatus>());
                    blockStatus empty = {Qt::gray, ""};
                    blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);

                    variantsBox->addItem("ИМП: " + resImp[i].name);
                }

                for (int j = 0; j < resMci.size(); ++j) {


                    color = Qt::green;
                    if (!resMci[j].res_create || (resMci[j].description_test != "" && !resMci[j].res_test)) {
                        color = Qt::red;
                    }

                    text = "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_create +
                           "\nВремя трансляции: " + resMci[j].create_time;

                    blockStatus mciBlockStatus = {color, text};
                    blockStatus mciTestBlockStatus = {Qt::gray, ""};

                    if (resMci[j].description_test != "") {

                        color = Qt::green;
                        if (!resMci[j].res_test) {
                            color = Qt::red;
                        }

                        text += "\n" + resMci[j].description_test;
                        mciTestBlockStatus = {color, text};
                    }

                    QVector<queryRes> resPss = pssSearchMciId(resMci[j].id);

                    //Если не нашлось дальше ПСС, в который входит МЦИ
                    if (resPss.size() == 0) {
                        blocksStatus.push_back(QVector<blockStatus>());
                        blockStatus empty = {Qt::gray, ""};
                        blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);

                        variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name);
                    }

                    for (int k = 0; k < resPss.size(); ++k) {
                        color = Qt::green;
                        if ((!resPss[k].res_create || (resPss[k].description_test != "" && !resPss[k].res_test))) {
                            color = Qt::red;
                        }

                        text = "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_create +
                            "\nВремя трансляции: " + resPss[k].create_time;;

                        blockStatus pssBlockStatus = {color, text};

                        blockStatus pssTestBlockStatus = {Qt::gray, ""};

                        if (resPss[k].description_test != "") {

                            color = Qt::green;
                            if (!resPss[k].res_test) {
                                color = Qt::red;
                            }

                            text += "\n" + resPss[k].description_test;
                            pssTestBlockStatus = {color, text};
                        }


                        if (resMci[j].description_send == "" && resPss[k].description_send == "") {
                            blocksStatus.push_back(QVector<blockStatus>());
                            blockStatus empty = {Qt::gray, ""};
                            blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(empty);

                            variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                            break;
                        }

                        color = Qt::green;
                        if ((resMci[j].description_send != "" && !resMci[j].res_send) ||
                            (resPss[k].description_send != "" && !resPss[k].res_send)) {
                            color = Qt::red;
                        }

                        text = "";

                        if (resMci[j].description_send != "") {
                            text += "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_send +
                                "\nВремя отправки: " + resMci[j].send_time;
                        }

                        if (resPss[k].description_send != "") {
                            text += "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_send +
                                "\nВремя отправки: " + resPss[k].send_time;;
                        }

                        blocksStatus.push_back(QVector<blockStatus>());
                        blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back({color, text});

                        variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                    }


                    qDebug() << "sizePss" << resPss.size();
                }
            }

            drawBlocks(0);
            break;
        }

        //МЦИ
        case 1: {
            QString mciName = searchEdit->text();

            QVector<queryRes> resMci = mciSearchMciName(mciName);
            qDebug() << "sizeMci" << resMci.size();

            for (int j = 0; j < resMci.size(); ++j) {

                QColor color = Qt::green;
                if (!resMci[j].res_create || (resMci[j].description_test != "" && !resMci[j].res_test)) {
                    color = Qt::red;
                }

                QString text = "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_create +
                       "\nВремя трансляции: " + resMci[j].create_time;

                blockStatus mciBlockStatus = {color, text};
                blockStatus mciTestBlockStatus = {Qt::gray, ""};

                if (resMci[j].description_test != "") {

                    color = Qt::green;
                    if (!resMci[j].res_test) {
                        color = Qt::red;
                    }

                    text += "\n" + resMci[j].description_test;
                    mciTestBlockStatus = {color, text};
                }

                color = Qt::green;
                if ((resMci[j].description_send != "" && !resMci[j].res_send)) {
                    color = Qt::red;
                }

                text = "";

                if (resMci[j].description_send != "") {
                    text += "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_send +
                            "\nВремя отправки: " + resMci[j].send_time;
                }

                blockStatus sendStatus = {color, text};

                QVector<queryRes> resImp = impSearchMciId(resMci[j].id);
                QVector<queryRes> resPss = pssSearchMciId(resMci[j].id);

                qDebug() << "resImp" << resImp.size();
                qDebug() << "resPss" << resPss.size();

                if (resPss.size() == 0 && resImp.size() == 0) {
                    blocksStatus.push_back(QVector<blockStatus>());
                    blockStatus empty = {Qt::gray, ""};
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                    blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(empty);
                    blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                    variantsBox->addItem("МЦИ: " + resMci[j].name);
                    break;
                } else if (resPss.size() == 0) {
                    for (int i = 0; i < resImp.size(); ++i) {

                        QColor color = Qt::green;
                        if (!resImp[i].res_create) {
                            color = Qt::red;
                        }

                        QString text = "\nИмя ИМП: " + resImp[i].name + "\nТип: " +
                                       resImp[i].type + "\n" + resImp[i].description_create +
                                       "\nВремя трансляции: " + resImp[i].create_time;

                        blockStatus impBlockStatus = {color, text};

                        blocksStatus.push_back(QVector<blockStatus>());
                        blockStatus empty = {Qt::gray, ""};
                        blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);
                        blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                        variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name);
                        break;
                    }
                } else if (resImp.size() == 0) {
                    for (int k = 0; k < resPss.size(); ++k) {
                        color = Qt::green;
                        if ((!resPss[k].res_create || (resPss[k].description_test != "" && !resPss[k].res_test))) {
                            color = Qt::red;
                        }

                        text = "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_create +
                               "\nВремя трансляции: " + resPss[k].create_time;;

                        blockStatus pssBlockStatus = {color, text};

                        blockStatus pssTestBlockStatus = {Qt::gray, ""};

                        if (resPss[k].description_test != "") {

                            color = Qt::green;
                            if (!resPss[k].res_test) {
                                color = Qt::red;
                            }

                            text += "\n" + resPss[k].description_test;
                            pssTestBlockStatus = {color, text};
                        }

                        if ((resPss[k].description_send != "" && !resPss[k].res_send)) {
                            sendStatus.color = Qt::red;
                        }

                        if (resPss[k].description_send != "") {
                            sendStatus.text += "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_send +
                                    "\nВремя отправки: " + resPss[k].send_time;;
                        }

                        blocksStatus.push_back(QVector<blockStatus>());
                        blockStatus empty = {Qt::gray, ""};
                        blocksStatus[blocksStatus.size() - 1].push_back(empty);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                        blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                        variantsBox->addItem("МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                        break;
                    }
                } else {

                    for (int i = 0; i < resImp.size(); ++i) {
                        for (int k = 0; k < resPss.size(); ++k) {

                            QColor color = Qt::green;
                            if (!resImp[i].res_create) {
                                color = Qt::red;
                            }

                            QString text = "\nИмя ИМП: " + resImp[i].name + "\nТип: " +
                                           resImp[i].type + "\n" + resImp[i].description_create +
                                           "\nВремя трансляции: " + resImp[i].create_time;

                            blockStatus impBlockStatus = {color, text};

                            color = Qt::green;
                            if ((!resPss[k].res_create || (resPss[k].description_test != "" && !resPss[k].res_test))) {
                                color = Qt::red;
                            }

                            text = "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_create +
                                   "\nВремя трансляции: " + resPss[k].create_time;;

                            blockStatus pssBlockStatus = {color, text};

                            blockStatus pssTestBlockStatus = {Qt::gray, ""};

                            if (resPss[k].description_test != "") {

                                color = Qt::green;
                                if (!resPss[k].res_test) {
                                    color = Qt::red;
                                }

                                text += "\n" + resPss[k].description_test;
                                pssTestBlockStatus = {color, text};
                            }

                            if ((resPss[k].description_send != "" && !resPss[k].res_send)) {
                                sendStatus.color = Qt::red;
                            }

                            if (resPss[k].description_send != "") {
                                sendStatus.text += "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_send +
                                                   "\nВремя отправки: " + resPss[k].send_time;;
                            }

                            blocksStatus.push_back(QVector<blockStatus>());
                            blockStatus empty = {Qt::gray, ""};
                            blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                            blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                            variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                            break;

                        }
                    }
                }
            }

            drawBlocks(0);
            break;
        }

        case 2: {

            QString pssName = searchEdit->text();

            QVector<queryRes> resPss = pssSearchPssName(pssName);
            qDebug() << resPss.size();


            for (int k = 0; k < resPss.size(); ++k) {
                QColor color = Qt::green;
                if ((!resPss[k].res_create || (resPss[k].description_test != "" && !resPss[k].res_test))) {
                    color = Qt::red;
                }

                QString text = "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_create +
                       "\nВремя трансляции: " + resPss[k].create_time;;

                blockStatus pssBlockStatus = {color, text};
                blockStatus pssTestBlockStatus = {Qt::gray, ""};

                if (resPss[k].description_test != "") {

                    color = Qt::green;
                    if (!resPss[k].res_test) {
                        color = Qt::red;
                    }

                    text += "\n" + resPss[k].description_test;
                    pssTestBlockStatus = {color, text};
                }

                blockStatus sendStatusPss = {Qt::gray, ""};

                if ((resPss[k].description_send != "" && !resPss[k].res_send)) {
                    sendStatusPss.color = Qt::red;
                }

                if (resPss[k].description_send != "") {
                    sendStatusPss.text += "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_send +
                                       "\nВремя отправки: " + resPss[k].send_time;;
                }

                QJsonDocument jsonDoc = QJsonDocument::fromJson(resPss[k].json.toUtf8());
                QJsonObject jsonObj = jsonDoc.object();

                auto commArray = jsonObj["commands"].toArray();

                foreach (const QJsonValue & value, commArray) {
                    QJsonObject commJson = value.toObject();


                    if (commJson.value("type").toString() == "mci") {
                        QVector<queryRes> resMci = mciSearchMciName(commJson.value("name").toString());

                        for (int j = 0; j < resMci.size(); ++j) {
                            color = Qt::green;
                            if (!resMci[j].res_create || (resMci[j].description_test != "" && !resMci[j].res_test)) {
                                color = Qt::red;
                            }

                            QString text = "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_create +
                                           "\nВремя трансляции: " + resMci[j].create_time;

                            blockStatus mciBlockStatus = {color, text};
                            blockStatus mciTestBlockStatus = {Qt::gray, ""};

                            if (resMci[j].description_test != "") {

                                color = Qt::green;
                                if (!resMci[j].res_test) {
                                    color = Qt::red;
                                }

                                text += "\n" + resMci[j].description_test;
                                mciTestBlockStatus = {color, text};
                            }

                            if ((resMci[j].description_send != "" && !resMci[j].res_send)) {
                                color = Qt::red;
                            }

                            text = "";

                            if (resMci[j].description_send != "") {
                                text += "\nИмя МЦИ: " + resMci[j].name + "\nТип: " + resMci[j].type + "\n" + resMci[j].description_send +
                                        "\nВремя отправки: " + resMci[j].send_time;
                            }

                            if (color != Qt::red && sendStatusPss.color == Qt::red) {
                                color = Qt::red;
                            }

                            blockStatus sendStatus = {color, text + sendStatusPss.text};

                            QVector<queryRes> resImp = impSearchMciId(resMci[j].id);

                            if (resImp.size() == 0) {
                                blocksStatus.push_back(QVector<blockStatus>());
                                blockStatus empty = {Qt::gray, ""};
                                blocksStatus[blocksStatus.size() - 1].push_back(empty);
                                blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                                blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                                blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                                blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                                blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                                variantsBox->addItem("МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                                break;
                            } else {
                                for (int i = 0; i < resImp.size(); ++i) {

                                    QColor color = Qt::green;
                                    if (!resImp[i].res_create) {
                                        color = Qt::red;
                                    }

                                    QString text = "\nИмя ИМП: " + resImp[i].name + "\nТип: " +
                                                   resImp[i].type + "\n" + resImp[i].description_create +
                                                   "\nВремя трансляции: " + resImp[i].create_time;

                                    blockStatus impBlockStatus = {color, text};

                                    color = Qt::green;
                                    if ((!resPss[k].res_create || (resPss[k].description_test != "" && !resPss[k].res_test))) {
                                        color = Qt::red;
                                    }

                                    text = "\nИмя ПСС: " + resPss[k].name + "\nТип: " + resPss[k].type + "\n" + resPss[k].description_create +
                                           "\nВремя трансляции: " + resPss[k].create_time;;

                                    blocksStatus.push_back(QVector<blockStatus>());
                                    blockStatus empty = {Qt::gray, ""};
                                    blocksStatus[blocksStatus.size() - 1].push_back(impBlockStatus);
                                    blocksStatus[blocksStatus.size() - 1].push_back(mciBlockStatus);
                                    blocksStatus[blocksStatus.size() - 1].push_back(mciTestBlockStatus);
                                    blocksStatus[blocksStatus.size() - 1].push_back(pssBlockStatus);
                                    blocksStatus[blocksStatus.size() - 1].push_back(pssTestBlockStatus);
                                    blocksStatus[blocksStatus.size() - 1].push_back(sendStatus);

                                    variantsBox->addItem("ИМП: " + resImp[i].name + ", МЦИ: " + resMci[j].name + ", ПСС: " + resPss[k].name);
                                }
                            }
                        }
                    }
                }
            }
            drawBlocks(0);
            break;
        }
    }
}

void TreeWidget::drawBlocks(int id) {

    if (blocksStatus.size() == 0) {
        eventBlock->clear();
        return;
    }

    for (int i = 0; i < 6; ++i) {
        eventBlock->setBlockColor(i, blocksStatus[id][i].color);
        eventBlock->setWindowText(i, blocksStatus[id][i].text);
    }

}

void TreeWidget::changedSearchBox()
{
    blocksStatus.clear();
    eventBlock->clear();
    variantsBox->clear();
    variantsBox->hide();
    // searchEdit->clear();
}

QVector<queryRes> TreeWidget::impSearchImpName(QString impName) {
    QString req = QString("SELECT "
                          "imp.id, "
                          "imp.name, "
                          "imp.type, "
                          "imp.translation_time, "
                          "result.res, "
                          "result.description "
                          "FROM "
                          "   inner_db.imp "
                          "JOIN "
                          "    inner_db.result ON imp.id_res_create = result.id "
                          "WHERE "
                          "   imp.name = '%1'")
                      .arg(impName);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("res").toBool(),
                       searchTestQuery.value("description").toString(), searchTestQuery.value("translation_time").toString()});
    }

    return res;
}

QVector<queryRes> TreeWidget::impSearchMciId(int mciId) {

    QString req = QString("SELECT "
                          "imp.id, "
                          "imp.name, "
                          "imp.type, "
                          "imp.translation_time, "
                          "result.res, "
                          "result.description "
                          "FROM "
                          "   inner_db.imp "
                          "JOIN "
                          "    inner_db.mci ON imp.id = mci.id_imp "
                          "JOIN "
                          "    inner_db.result ON imp.id_res_create = result.id "
                          "WHERE "
                          "   mci.id = %1")
                      .arg(mciId);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("res").toBool(),
                       searchTestQuery.value("description").toString(), searchTestQuery.value("translation_time").toString()});
    }

    return res;

}

QVector<queryRes> TreeWidget::mciSearchMciName(QString mciName)
{
    QString req = QString("SELECT "
                          "mci.id, "
                          "mci.name, "
                          "mci.type, "
                          "mci.translation_time, "
                          "send.send_time, "
                          "create_res.res AS create_res, "
                          "create_res.description AS create_description, "
                          "test_res.res AS test_res, "
                          "test_res.description AS test_description, "
                          "send_result.res AS send_res, "
                          "send_result.description AS send_description "
                          "FROM "
                          "   inner_db.mci "
                          "JOIN "
                          "    inner_db.result create_res ON mci.id_res_create = create_res.id "
                          "LEFT JOIN "
                          "    inner_db.result test_res ON mci.id_res_test = test_res.id "
                          "LEFT JOIN "
                          "    inner_db.send ON mci.id_send = send.id "
                          "LEFT JOIN "
                          "    inner_db.result send_result ON send.id_res_send = send_result.id "
                          "WHERE "
                          "   mci.name = '%1'")
                      .arg(mciName);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("create_res").toBool(),
                       searchTestQuery.value("create_description").toString(),
                       searchTestQuery.value("translation_time").toString(),
                       searchTestQuery.value("test_res").toBool(),
                       searchTestQuery.value("test_description").toString(),
                       searchTestQuery.value("send_res").toBool(),
                       searchTestQuery.value("send_description").toString(),
                       searchTestQuery.value("send_time").toString()});
    }

    return res;
}

QVector<queryRes> TreeWidget::pssSearchPssName(QString pssName) {
    QString req = QString("SELECT "
                          "pss.id, "
                          "pss.name, "
                          "pss.type, "
                          "pss.structure, "
                          "pss.translation_time, "
                          "send.send_time, "
                          "create_res.res AS create_res, "
                          "create_res.description AS create_description, "
                          "test_res.res AS test_res, "
                          "test_res.description AS test_description, "
                          "send_result.res AS send_res, "
                          "send_result.description AS send_description "
                          "FROM"
                          "   inner_db.pss "
                          "JOIN "
                          "    inner_db.result create_res ON pss.id_res_create = create_res.id "
                          "LEFT JOIN "
                          "    inner_db.result test_res ON pss.id_res_test = test_res.id "
                          "LEFT JOIN "
                          "    inner_db.send ON pss.id_send = send.id "
                          "LEFT JOIN "
                          "    inner_db.result send_result ON send.id_res_send = send_result.id "
                          "WHERE "
                          "pss.name = '%1'")
                      .arg(pssName);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("create_res").toBool(),
                       searchTestQuery.value("create_description").toString(),
                       searchTestQuery.value("translation_time").toString(),
                       searchTestQuery.value("test_res").toBool(),
                       searchTestQuery.value("test_description").toString(),
                       searchTestQuery.value("send_res").toBool(),
                       searchTestQuery.value("send_description").toString(),
                       searchTestQuery.value("send_time").toString(),
                       searchTestQuery.value("structure").toString()});
    }

    return res;
}

QVector<queryRes> TreeWidget::mciSearchImpId(int impId)
{
    QString req = QString("SELECT "
                          "mci.id, "
                          "mci.name, "
                          "mci.type, "
                          "mci.translation_time, "
                          "send.send_time, "
                          "create_res.res AS create_res, "
                          "create_res.description AS create_description, "
                          "test_res.res AS test_res, "
                          "test_res.description AS test_description, "
                          "send_result.res AS send_res, "
                          "send_result.description AS send_description "
                          "FROM "
                          "   inner_db.mci "
                          "JOIN "
                          "    inner_db.result create_res ON mci.id_res_create = create_res.id "
                          "LEFT JOIN "
                          "    inner_db.result test_res ON mci.id_res_test = test_res.id "
                          "LEFT JOIN "
                          "    inner_db.send ON mci.id_send = send.id "
                          "LEFT JOIN "
                          "    inner_db.result send_result ON send.id_res_send = send_result.id "
                          "WHERE "
                          "   mci.id_imp = %1")
                      .arg(impId);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("create_res").toBool(),
                       searchTestQuery.value("create_description").toString(),
                       searchTestQuery.value("translation_time").toString(),
                       searchTestQuery.value("test_res").toBool(),
                       searchTestQuery.value("test_description").toString(),
                       searchTestQuery.value("send_res").toBool(),
                       searchTestQuery.value("send_description").toString(),
                       searchTestQuery.value("send_time").toString()});
    }

    return res;
}

QVector<queryRes> TreeWidget::pssSearchMciId(int mciId) {
    QString req = QString("SELECT "
                          "pss.id, "
                          "pss.name, "
                          "pss.type, "
                          "pss.translation_time, "
                          "send.send_time, "
                          "create_res.res AS create_res, "
                          "create_res.description AS create_description, "
                          "test_res.res AS test_res, "
                          "test_res.description AS test_description, "
                          "send_result.res AS send_res, "
                          "send_result.description AS send_description "
                          "FROM"
                          "   inner_db.pss "
                          "JOIN "
                          "    inner_db.result create_res ON pss.id_res_create = create_res.id "
                          "LEFT JOIN "
                          "    inner_db.result test_res ON pss.id_res_test = test_res.id "
                          "LEFT JOIN "
                          "    inner_db.send ON pss.id_send = send.id "
                          "LEFT JOIN "
                          "    inner_db.result send_result ON send.id_res_send = send_result.id "
                          "WHERE "
                          "jsonb_path_exists(pss.structure::jsonb, '$.commands[*] ? (@.type == \"mci\" && @.name == $mci_name)'::jsonpath,"
                          "jsonb_build_object('mci_name', (SELECT name FROM inner_db.mci WHERE id = %1)))")
                      .arg(mciId);

    QSqlQuery searchTestQuery(db);
    procQuery(searchTestQuery, req, false);

    QVector<queryRes> res;

    while (searchTestQuery.next()) {
        res.push_back({searchTestQuery.value("id").toInt(), searchTestQuery.value("name").toString(),
                       searchTestQuery.value("type").toString(), searchTestQuery.value("create_res").toBool(),
                       searchTestQuery.value("create_description").toString(),
                       searchTestQuery.value("translation_time").toString(),
                       searchTestQuery.value("test_res").toBool(),
                       searchTestQuery.value("test_description").toString(),
                       searchTestQuery.value("send_res").toBool(),
                       searchTestQuery.value("send_description").toString(),
                       searchTestQuery.value("send_time").toString()});
    }

    return res;
}
