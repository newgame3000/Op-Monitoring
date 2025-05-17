#ifndef EXPANDABLE_ITEM_DELEGATE_H
#define EXPANDABLE_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QTextDocument>

class ExpandableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ExpandableItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        QTextDocument doc;
        doc.setHtml(opt.text);
        doc.setTextWidth(opt.rect.width());

        painter->save();
        painter->translate(opt.rect.topLeft());

        // Рассчитываем смещение по Y для вертикального выравнивания по центру
        QRect textRect = opt.rect;
        textRect.moveTo(0, 0);

        qreal textHeight = doc.size().height();
        qreal yOffset = 0;

        // Если текст помещается в ячейку, выравниваем по центру
        if (textHeight <= opt.rect.height()) {
            yOffset = (opt.rect.height() - textHeight) / 2;
        }

        painter->translate(0, yOffset);
        doc.drawContents(painter, textRect);

        painter->restore();
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override
    {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                QString text = model->data(index, Qt::DisplayRole).toString();

                QTextEdit *textEdit = new QTextEdit;
                textEdit->setText(text);
                textEdit->setReadOnly(true);

                textEdit->setStyleSheet(R"(
                    QTextEdit {
                        background-color: #FFFFFF;  // Белый фон
                        color: #000000;  // Черный текст
                        font-size: 12px;
                        border: 2px solid #80CBC4;  // Рамка
                        padding: 10px;
                    }
                )");

                QFontMetrics metrics(textEdit->font());
                int textWidth = metrics.horizontalAdvance(text) + 40;  // Ширина текста + отступы
                int textHeight = metrics.height() * (text.count('\n') + 1) + 50;  // Высота текста + отступы

                // textWidth = std::max(textWidth, 400);  // Максимальная ширина
                // textHeight = std::max(textHeight, 100);  // Максимальная высота

                textEdit->setWindowTitle("Содержимое ячейки");
                textEdit->resize(textWidth, textHeight);
                textEdit->show();

                return true;
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
};
#endif // EXPANDABLE_ITEM_DELEGATE_H
