#ifndef QTCALC_H
#define QTCALC_H

#include <QWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class QtCalc; }
QT_END_NAMESPACE

class QtCalc : public QWidget
{
    Q_OBJECT

public:
    QtCalc(QWidget *parent = nullptr);
    ~QtCalc();

private slots:
    void addnumber();

    void addsign();

    void expressions();

    void display_cleaning();

    void on_btnDot_clicked();

    void on_btnClearAll_clicked();

    void on_btnEquals_clicked();

    void on_btnPercent_clicked();

private:
    Ui::QtCalc *ui;
    QString number;
    QString sign;
    double first_number{};

    void buttons();
};
#endif // QTCALC_H
