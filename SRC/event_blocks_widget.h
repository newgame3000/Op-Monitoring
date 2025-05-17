#ifndef EVENTBLOCKSWIDGET_H
#define EVENTBLOCKSWIDGET_H

#include <QWidget>
#include <QColor>
#include <QStringList>

class EventBlocksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventBlocksWidget(QWidget *parent = nullptr);

    void setBlockColor(int index, const QColor& color);
    void setBlockText(int index, const QString& text);
    void setWindowText(int index, const QString& text);
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    static const int BLOCK_COUNT = 6;
    QColor blockColors[BLOCK_COUNT];
    QStringList blockTexts;
    QStringList wrappedTexts;
    QStringList windowTexts;

    void updateWrappedTexts();
    void showBlockInfo(int index);
};

#endif // EVENTBLOCKSWIDGET_H
