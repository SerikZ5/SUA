#include "sua.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SUA w;
    w.show();
    return a.exec();
}
