#include "add_widgets.h"

AddWidgetField::AddWidgetField(QWidget *parent)
    : QWidget{parent}
{

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(this);
    label->setText("");
    lineEdit = new QLineEdit(this);

    layout->addWidget(label);
    layout->addWidget(lineEdit);
    setLayout(layout);

}

AddWidgetDateFields::AddWidgetDateFields(QWidget *parent)
    : QWidget{parent}
{

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    startTimeLabel = new QLabel(this);
    startTimeLabel->setText("Начальное время: ");
    startTimeLabel->setAlignment(Qt::AlignCenter);

    endTimeLabel = new QLabel(this);
    endTimeLabel->setText("Конечное время: ");
    endTimeLabel->setAlignment(Qt::AlignCenter);

    startDateEdit = new QDateTimeEdit(this);
    endDateEdit = new QDateTimeEdit(this);

    layout->addWidget(startTimeLabel);
    layout->addWidget(startDateEdit);

    layout->addWidget(endTimeLabel);
    layout->addWidget(endDateEdit);
    setLayout(layout);

}
