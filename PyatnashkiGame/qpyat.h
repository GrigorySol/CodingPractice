#ifndef QPYAT_H
#define QPYAT_H

#include <QDialog>
#include <QFrame>
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
    void startNewGame();
private:
    Ui::QPyat *ui;
    QFrame *gameTable;
    QGridLayout *gameLayout;
    QFont textFont;
    QPushButton *btnNewGame;

    void init();
    void createPuzzle();
    QPoint  findEmpty(const QPoint &tilePos);
};
#endif // QPYAT_H
