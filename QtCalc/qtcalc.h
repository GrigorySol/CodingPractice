#ifndef QTCALC_H
#define QTCALC_H

#include <QWidget>

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
    void on_btnEquals_clicked();
    void on_btnClear_clicked();

private:
    Ui::QtCalc *ui;

    QString number;

    void buttons();
};
#endif // QTCALC_H
