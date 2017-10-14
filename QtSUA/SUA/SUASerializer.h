#ifndef SUA_SERIALIZER
#define SUA_SERIALIZER

#include <QString>
#include <QDomDocument>

#include "SUASettings.h"

class SUASerializer 
{
public:
  static bool Serialize(QString path, SUASettings* settings);
  static bool Deserialize(QString path, SUASettings* settings);

private:
  static QDomElement createElement(QDomDocument document, QString name, QString value);
  static QString parseElemet(QDomElement root, QString name);
};


#endif