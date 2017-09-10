#include "server.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer w(9875);
    w.show();
    return a.exec();
}
