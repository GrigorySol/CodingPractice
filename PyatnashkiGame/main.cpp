#include "qpyat.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPyat w;
    w.show();
    return a.exec();
}
