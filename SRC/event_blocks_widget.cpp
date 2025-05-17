#include "event_blocks_widget.h"
#include <QPainter>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QMessageBox>

EventBlocksWidget::EventBlocksWidget(QWidget *parent) : QWidget(parent)
{
    // Инициализация цветов (серый по умолчанию)
    for (int i = 0; i < BLOCK_COUNT; ++i) {
        blockColors[i] = Qt::gray;
    }

    // Тексты для каждого блока
    blockTexts << "Формирование ИМП"
               << "Формирование МЦИ"
               << "Отработка МЦИ на моделирующих стендах"
               << "Формирование ПСС"
               << "Отработка ПСС на моделирующих стендах"
               << "Выдача КПИ";

    windowTexts << ""
                << ""
                << ""
                << ""
                << ""
                << "";

    updateWrappedTexts();
    setMinimumSize(1200, 300); // Ещё больше минимальный размер
}

void EventBlocksWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Размеры элементов
    const int blockWidth = 220;
    const int blockHeight = 100;
    const int arrowLength = 50;
    const int verticalSpacing = 50;

    // Рассчитываем общие размеры схемы
    int totalTopRowWidth = 2 * (blockWidth + arrowLength) + blockWidth;
    int totalBottomRowWidth = blockWidth + arrowLength + blockWidth;
    int totalWidth = std::max(totalTopRowWidth, totalBottomRowWidth);
    int totalHeight = 2 * blockHeight + verticalSpacing;

    // Центровка с смещением влево (уменьшаем startX на 20%)
    int startX = (width() - totalWidth) / 2 - totalWidth * 0.2;
    int startY = (height() - totalHeight) / 2;

    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    // Координаты рядов
    int topRowY = startY;
    int bottomRowY = startY + blockHeight + verticalSpacing;

    // 1. Рисуем верхний ряд (блоки 1, 2, 3)
    for (int i = 0; i < 3; ++i) {
        QRect blockRect(startX + i * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);
        painter.setBrush(blockColors[i]);
        painter.drawRoundedRect(blockRect, 10, 10);
        painter.drawText(blockRect, wrappedTexts[i], QTextOption(Qt::AlignCenter));

        // Стрелки между верхними блоками
        if (i < 2) {
            int arrowStart = blockRect.right();
            int arrowEnd = arrowStart + arrowLength;
            int arrowY = blockRect.center().y();

            painter.setPen(QPen(Qt::black, 2));
            painter.drawLine(arrowStart, arrowY, arrowEnd, arrowY);

            // Наконечник стрелки
            QPolygon arrowHead;
            arrowHead << QPoint(arrowEnd, arrowY);
            arrowHead << QPoint(arrowEnd - 10, arrowY - 5);
            arrowHead << QPoint(arrowEnd - 10, arrowY + 5);
            painter.setBrush(Qt::black);
            painter.drawPolygon(arrowHead);
        }
    }

    // 2. Блок 4 (под блоком 2)
    QRect block2Rect(startX + 1 * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);
    QRect block4Rect(block2Rect.center().x() - blockWidth/2, bottomRowY, blockWidth, blockHeight);
    painter.setBrush(blockColors[3]);
    painter.drawRoundedRect(block4Rect, 10, 10);
    painter.drawText(block4Rect, wrappedTexts[3], QTextOption(Qt::AlignCenter));

    // Стрелка от 2 к 4
    painter.setPen(QPen(Qt::black, 2));
    int arrow2to4X = block2Rect.center().x();
    painter.drawLine(arrow2to4X, block2Rect.bottom(), arrow2to4X, block4Rect.top());

    // Наконечник стрелки 2→4 (направлен вниз)
    QPolygon arrowHead2to4;
    arrowHead2to4 << QPoint(arrow2to4X, block4Rect.top());
    arrowHead2to4 << QPoint(arrow2to4X - 5, block4Rect.top() - 10);
    arrowHead2to4 << QPoint(arrow2to4X + 5, block4Rect.top() - 10);
    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead2to4);

    // 3. Блок 5 (справа от 4)
    QRect block5Rect(block4Rect.right() + arrowLength, bottomRowY, blockWidth, blockHeight);
    painter.setBrush(blockColors[4]);
    painter.drawRoundedRect(block5Rect, 10, 10);
    painter.drawText(block5Rect, wrappedTexts[4], QTextOption(Qt::AlignCenter));

    // Стрелка от 4 к 5
    painter.setPen(QPen(Qt::black, 2));
    int arrowY = block4Rect.center().y();
    painter.drawLine(block4Rect.right(), arrowY, block5Rect.left(), arrowY);

    // Наконечник стрелки 4→5
    QPolygon arrowHead4to5;
    arrowHead4to5 << QPoint(block5Rect.left(), arrowY);
    arrowHead4to5 << QPoint(block5Rect.left() - 10, arrowY - 5);
    arrowHead4to5 << QPoint(block5Rect.left() - 10, arrowY + 5);
    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead4to5);

    // 4. Блок 6 (центрированный справа)
    QRect block3Rect(startX + 2 * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);
    int block6X = std::max(block3Rect.right(), block5Rect.right()) + arrowLength;
    // Опускаем блок значительно ниже (на 60px от центра)
    int block6Y = (topRowY + bottomRowY) / 2 - blockHeight/2 + 60;  // Было +25, теперь +60
    QRect block6Rect(block6X, block6Y, blockWidth, blockHeight);
    painter.setBrush(blockColors[5]);
    painter.drawRoundedRect(block6Rect, 10, 10);
    painter.drawText(block6Rect, wrappedTexts[5], QTextOption(Qt::AlignCenter));

    // 5. Стрелка от 3 к 6 (с изгибом)
    int arrow3to6StartX = block3Rect.right();
    int arrow3to6EndX = block6Rect.left();
    int arrow3to6StartY = block3Rect.center().y();
    int arrow3to6EndY = block6Rect.center().y();

    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(arrow3to6StartX, arrow3to6StartY,
                     (arrow3to6StartX + arrow3to6EndX)/2, arrow3to6StartY);
    painter.drawLine((arrow3to6StartX + arrow3to6EndX)/2, arrow3to6StartY,
                     (arrow3to6StartX + arrow3to6EndX)/2, arrow3to6EndY);
    painter.drawLine((arrow3to6StartX + arrow3to6EndX)/2, arrow3to6EndY,
                     arrow3to6EndX, arrow3to6EndY);

    // Наконечник стрелки 3→6
    QPolygon arrowHead3to6;
    arrowHead3to6 << QPoint(arrow3to6EndX, arrow3to6EndY);
    arrowHead3to6 << QPoint(arrow3to6EndX - 10, arrow3to6EndY - 5);
    arrowHead3to6 << QPoint(arrow3to6EndX - 10, arrow3to6EndY + 5);
    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead3to6);

    // 6. Стрелка от 5 к 6 (аналогично стрелке 3→6)
    int arrow5to6StartX = block5Rect.right();
    int arrow5to6EndX = block6Rect.left();
    int arrow5to6StartY = block5Rect.center().y();
    int arrow5to6EndY = block6Rect.center().y();

    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(arrow5to6StartX, arrow5to6StartY,
                     (arrow5to6StartX + arrow5to6EndX)/2, arrow5to6StartY);
    painter.drawLine((arrow5to6StartX + arrow5to6EndX)/2, arrow5to6StartY,
                     (arrow5to6StartX + arrow5to6EndX)/2, arrow5to6EndY);
    painter.drawLine((arrow5to6StartX + arrow5to6EndX)/2, arrow5to6EndY,
                     arrow5to6EndX, arrow5to6EndY);

    // Наконечник стрелки 5→6
    QPolygon arrowHead5to6;
    arrowHead5to6 << QPoint(arrow5to6EndX, arrow5to6EndY);
    arrowHead5to6 << QPoint(arrow5to6EndX - 10, arrow5to6EndY - 5);
    arrowHead5to6 << QPoint(arrow5to6EndX - 10, arrow5to6EndY + 5);
    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead5to6);
}

void EventBlocksWidget::setBlockColor(int index, const QColor& color)
{
    if (index >= 0 && index < BLOCK_COUNT) {
        blockColors[index] = color;
        update();
    }
}

void EventBlocksWidget::mousePressEvent(QMouseEvent *event)
{
    const int blockWidth = 220;
    const int blockHeight = 100;
    const int arrowLength = 50;
    const int verticalSpacing = 50;

    // Рассчитываем общие размеры схемы (как в paintEvent)
    int totalTopRowWidth = 2 * (blockWidth + arrowLength) + blockWidth;
    int totalBottomRowWidth = blockWidth + arrowLength + blockWidth;
    int totalWidth = std::max(totalTopRowWidth, totalBottomRowWidth);
    int totalHeight = 2 * blockHeight + verticalSpacing;

    // Центровка с смещением влево (как в paintEvent)
    int startX = (width() - totalWidth) / 2 - totalWidth * 0.2;
    int startY = (height() - totalHeight) / 2;

    int topRowY = startY;
    int bottomRowY = startY + blockHeight + verticalSpacing;

    // Проверяем клик в верхнем ряду (блоки 0,1,2)
    for (int i = 0; i < 3; ++i) {
        QRect blockRect(startX + i * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);
        if (blockRect.contains(event->pos())) {
            showBlockInfo(i);
            return;
        }
    }

    // Блок 1 (центральный верхний)
    QRect block1Rect(startX + 1 * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);

    // Блок 3 (под блоком 1)
    QRect block3Rect(block1Rect.center().x() - blockWidth/2, bottomRowY, blockWidth, blockHeight);
    if (block3Rect.contains(event->pos())) {
        showBlockInfo(3);
        return;
    }

    // Блок 4 (справа от блока 3)
    QRect block4Rect(block3Rect.right() + arrowLength, bottomRowY, blockWidth, blockHeight);
    if (block4Rect.contains(event->pos())) {
        showBlockInfo(4);
        return;
    }

    // Блок 2 (правый верхний)
    QRect block2Rect(startX + 2 * (blockWidth + arrowLength), topRowY, blockWidth, blockHeight);

    // Блок 5 (правый нижний, индекс 5)
    int block5X = std::max(block2Rect.right(), block4Rect.right()) + arrowLength;
    int block5Y = (topRowY + bottomRowY) / 2 - blockHeight/2 + 60;  // То же смещение, что и в paintEvent
    QRect block5Rect(block5X, block5Y, blockWidth, blockHeight);

    if (block5Rect.contains(event->pos())) {
        showBlockInfo(5);
        return;
    }
}

void EventBlocksWidget::setBlockText(int index, const QString& text)
{
    if (index >= 0 && index < BLOCK_COUNT) {
        blockTexts[index] = text;
        updateWrappedTexts();
        update();
    }
}

void EventBlocksWidget::setWindowText(int index, const QString &text)
{
    if (index >= 0 && index < BLOCK_COUNT) {
        windowTexts[index] = text;
    }
}

void EventBlocksWidget::clear() {
    for (int i = 0; i < BLOCK_COUNT; ++i) {
        blockColors[i] = Qt::gray;
        windowTexts[i] = "";
        updateWrappedTexts();
        update();
    }
}

void EventBlocksWidget::showBlockInfo(int index)
{
    if (index >= 0 && index < BLOCK_COUNT) {
        QString message;
        QString status;

        // Определяем текущий статус блока по цвету
        if (blockColors[index] == Qt::green) {
            status = "Результат: ";
        } else if (blockColors[index] == Qt::red) {
            status = "Ошибка: ";
        } else {
            status = "Неизвестно";
        }

        QMessageBox::information(this, "Информация", status + windowTexts[index]);
    }
}

void EventBlocksWidget::updateWrappedTexts()
{
    wrappedTexts.clear();
    QFontMetrics metrics(this->font());
    const int maxWidth = 200; // Максимальная ширина текста в блоке

    for (const QString& text : blockTexts) {
        QString wrappedText;
        QStringList words = text.split(' ');
        QString currentLine;

        for (const QString& word : words) {
            if (metrics.horizontalAdvance(currentLine + word) <= maxWidth) {
                currentLine += (currentLine.isEmpty() ? "" : " ") + word;
            } else {
                wrappedText += (wrappedText.isEmpty() ? "" : "\n") + currentLine;
                currentLine = word;
            }
        }

        if (!currentLine.isEmpty()) {
            wrappedText += (wrappedText.isEmpty() ? "" : "\n") + currentLine;
        }

        wrappedTexts << wrappedText;
    }
}

