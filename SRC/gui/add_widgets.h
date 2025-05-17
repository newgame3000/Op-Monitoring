#ifndef ADD_WIDGETS_H
#define ADD_WIDGETS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDateTimeEdit>

class AddWidgetField : public QWidget
{
    Q_OBJECT
public:
    explicit AddWidgetField(QWidget *parent = nullptr);

    QLabel *label;
    QLineEdit *lineEdit;

    void setText(QString text) {
        label->setText(text);
    }

    QString text() {
        return lineEdit->text();
    }
};



class AddWidgetDateFields : public QWidget
{
    Q_OBJECT
public:
    explicit AddWidgetDateFields(QWidget *parent = nullptr);

    QDateTime getStartDateEdit() {
        return startDateEdit->dateTime();
    }

    QDateTime getEndDateEdit() {
        return endDateEdit->dateTime();
    }

private:
    QLabel *startTimeLabel;
    QLabel *endTimeLabel;

    QDateTimeEdit * startDateEdit;
    QDateTimeEdit * endDateEdit;

};

#endif // ADD_WIDGETS_H
