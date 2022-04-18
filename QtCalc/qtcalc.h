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

private:
    Ui::QtCalc *ui;
};
#endif // QTCALC_H
