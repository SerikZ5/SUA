#include "sua.h"
#include <QtWidgets/QApplication>
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(".");
  paths.append("platforms");
  QCoreApplication::setLibraryPaths(paths);
  QApplication a(argc, argv);
  SUA w;
  w.show();
  return a.exec();
}
