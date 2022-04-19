#include "qtcalc.h"
#include "ui_qtcalc.h"

QtCalc::QtCalc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtCalc)
{
    ui->setupUi(this);
    buttons();
    keyshortcuts();
}

QtCalc::~QtCalc()
{
    delete ui;
}

void QtCalc::addnumber()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    typing_number.append(btn->text());
    ui->lcdNumber->display(typing_number);
}

void QtCalc::addsign()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    if(!first_number && !typing_number.size()) return; // Is there anything to calculate

    if(sign.size())
    {
        expressions();
    }
    first_number = ui->lcdNumber->value();          // Save displaing number
    lcd_line.append(typing_number);
    typing_number.clear();
    sign = btn->text();
    //lcd_line.append(QString::number(first_number)); // Append number to string line
    lcd_line.append(sign);
    // qDebug() << lcd_line << " " << number << " " << sign;
    ui->lcdString->setText(lcd_line);
}

void QtCalc::expressions()      // Simple actions for "+-*/" signs
{
    if(!first_number && !sign.size()) return;

    double second_number {ui->lcdNumber->value()};
    lcd_line.append(QString::number(second_number));

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
    typing_number.clear();
    ui->lcdNumber->display(first_number);
}


void QtCalc::on_btnPercent_clicked()    // Calculation for "%" sign
{
    if(!first_number && !typing_number.size()) return;

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

void QtCalc::keyshortcuts()
{
    ui->btnOne->setShortcut(QKeySequence("1"));
    ui->btnTwo->setShortcut(QKeySequence("2"));
    ui->btnThree->setShortcut(QKeySequence("3"));
    ui->btnFour->setShortcut(QKeySequence("4"));
    ui->btnFive->setShortcut(QKeySequence("5"));
    ui->btnSix->setShortcut(QKeySequence("6"));
    ui->btnSeven->setShortcut(QKeySequence("7"));
    ui->btnEight->setShortcut(QKeySequence("8"));
    ui->btnNine->setShortcut(QKeySequence("9"));
    ui->btnZero->setShortcut(QKeySequence("0"));
    ui->btnDot->setShortcut(QKeySequence("."));
    ui->btnPlus->setShortcut(QKeySequence("+"));
    ui->btnMinus->setShortcut(QKeySequence("-"));
    ui->btnMult->setShortcut(QKeySequence("*"));
    ui->btnDiv->setShortcut(QKeySequence(":"));
    ui->btnPercent->setShortcut(QKeySequence("%"));
    ui->btnClear->setShortcut(QKeySequence(Qt::Key_Backspace));

    QShortcut* equal = new QShortcut(QKeySequence("="), this);
    connect(equal, &QShortcut::activated, ui->btnEquals, &QPushButton::animateClick);
    QShortcut* ret = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(ret, &QShortcut::activated, ui->btnEquals, &QPushButton::animateClick);


    QShortcut* del = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(del, &QShortcut::activated, ui->btnClearAll, &QPushButton::animateClick);
    QShortcut* cmd_x = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_X), this);
    connect(cmd_x, &QShortcut::activated, ui->btnClearAll, &QPushButton::animateClick);
}

void QtCalc::on_btnDot_clicked()
{
    if(!typing_number.contains("."))   // Check existing point
    {
        if(typing_number.size())
        {
            typing_number.append(".");
        }
        else
        {
            typing_number = "0.";
        }
        ui->lcdNumber->display(typing_number);
    }
}

void QtCalc::display_cleaning()
{
    typing_number.clear();
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

