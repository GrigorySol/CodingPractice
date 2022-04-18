#include "qtcalc.h"
#include "ui_qtcalc.h"

QtCalc::QtCalc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtCalc)
{
    ui->setupUi(this);
    buttons();
}

QtCalc::~QtCalc()
{
    delete ui;
}

void QtCalc::addnumber()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    number.append(btn->text());
    ui->lcdNumber->display(number);
}

void QtCalc::addsign()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    if(!first_number && !number.size()) return; // Is there anything to calculate

    if(sign.size())
    {
        expressions();
    }
    first_number = ui->lcdNumber->value();
    number.clear();
    sign = btn->text();
}

void QtCalc::expressions()      // Simple actions for "+-*/" signs
{
    if(!first_number && !sign.size()) return;

    double second_number {ui->lcdNumber->value()};

    if(sign=="+")
    {
        first_number += second_number;
    }
    else if(sign=="-")
    {
        first_number -= second_number;
    }
    else if(sign=="x")
    {
        first_number *= second_number;
    }
    else if(sign=="÷")
    {
        first_number /= second_number;
    }
    number.clear();
    ui->lcdNumber->display(first_number);
}


void QtCalc::on_btnPercent_clicked()    // Calculation for "%" sign
{
    if(!first_number && !number.size()) return;

    double current_number {ui->lcdNumber->value()};

    if(!first_number)
    {
        ui->lcdNumber->display(current_number / 100);
    }
    else
    {
        ui->lcdNumber->display(first_number / 100 * current_number);
    }
}

void QtCalc::buttons()
{
    connect(ui->btnOne, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnTwo, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnThree, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnFour, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnFive, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnSix, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnSeven, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnEight, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnNine, &QPushButton::clicked, this, &QtCalc::addnumber);
    connect(ui->btnZero, &QPushButton::clicked, this, &QtCalc::addnumber);

    connect(ui->btnPlus, &QPushButton::clicked, this, &QtCalc::addsign);
    connect(ui->btnMinus, &QPushButton::clicked, this, &QtCalc::addsign);
    connect(ui->btnMult, &QPushButton::clicked, this, &QtCalc::addsign);
    connect(ui->btnDiv, &QPushButton::clicked, this, &QtCalc::addsign);

    connect(ui->btnClear, &QPushButton::clicked, this, &QtCalc::display_cleaning);
}


void QtCalc::on_btnDot_clicked()
{
    if(!number.contains("."))   // Check existing point
    {
        if(number.size())
        {
            number.append(".");
        }
        else
        {
            number = "0.";
        }
        ui->lcdNumber->display(number);
    }
}

void QtCalc::display_cleaning()
{
    number.clear();
    ui->lcdNumber->display("0");
}

void QtCalc::on_btnClearAll_clicked()
{
    display_cleaning();
    first_number = 0;
    sign.clear();
}


void QtCalc::on_btnEquals_clicked()
{
    expressions();
    sign.clear();
}

