#ifndef QPYAT_H
#define QPYAT_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QSize>
#include <QPoint>

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class QPyat; }
QT_END_NAMESPACE

class QPyat : public QDialog
{
    Q_OBJECT

public:
    QPyat(QWidget *parent = nullptr);
    ~QPyat();

private slots:
    void tileMovement();
private:
    Ui::QPyat *ui;
    void createPuzzle();
    QPoint  findEmpty(const QPoint &tilePos);
};
#endif // QPYAT_H
