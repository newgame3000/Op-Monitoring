#include "table_label.h"
#include "../expandable_item_delegate.h"

TableLabel::TableLabel(QWidget *parent)
    : QWidget{parent}
{
    model = new QSqlQueryModel(this);

    table = new QTableView(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setMinimumSectionSize(200);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setItemDelegate(new ExpandableItemDelegate(table));
    table->setModel(model);

    QVBoxLayout * tableVLayt = new QVBoxLayout;
    tableVLayt->setSpacing(1);
    tableVLayt->setMargin(0);

    QFrame * frame = new QFrame(this);
    // frame->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout * frameLayout = new QHBoxLayout;
    frameLayout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(this);
    label->setContentsMargins(0, 0, 0, 0);
    label->setAlignment(Qt::AlignCenter);

    frameLayout->addWidget(label);
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::Panel);

    tableVLayt->addWidget(frame);
    tableVLayt->addWidget(table);

    setLayout(tableVLayt);
}
