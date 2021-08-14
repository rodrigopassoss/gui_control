#include "control_interface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    control_interface w;
    w.show();
    return a.exec();
}
