#include <QtGui/QApplication>
#include "reservations.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Reservations w;
    w.show();
    
    return a.exec();
}
