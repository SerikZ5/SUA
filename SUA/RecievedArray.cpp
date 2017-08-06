#include <QString>

#include "RecievedArray.h"

RecievedArray::RecievedArray(QByteArray arr)
{
  this->arr = arr;
}

RecievedArray::~RecievedArray()
{
}

QString RecievedArray::ToString()
{
  return QString(arr);
}

QString RecievedArray::ToHexString()
{
  QByteArray hexArray = arr.toHex();
  QString result(hexArray);
  return result.toUpper();
}