#include "sua.h"
#include <QtWidgets/QApplication>
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SUA w;
    w.show();
    return a.exec();
}
