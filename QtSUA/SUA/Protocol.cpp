#include <QString>
#include <QStringBuilder>
#include <QObject>

#define useRN 0

#include "Protocol.h"

QByteArray Protocol::preambula = QString("SUA").toLatin1();

QByteArray Protocol::OrientedAntennas()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("111").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::FollowCmnd()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("E11").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::TestCmnd()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("F11").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetZeroPos()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("211").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetAzimuth(float azimuth)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("35").toLatin1());
  if (azimuth >= 0 && azimuth <= 360)
  {
    QString arg = QString("%1").arg(azimuth, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QObject::tr("Азимут выходит за допустимые пределы"); //Argument azimuth is out of range");
  }
  return arr;
}

QByteArray Protocol::SetZenith(float zenith)
{
  QByteArray arr;
  arr.push_back(preambula);
  if (zenith >= 0 && zenith <= 95)
  {
    arr.push_back(QString("45").toLatin1());
    QString arg = QString("%1").arg(zenith, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else if (zenith < 0 && zenith >= -5)
  {
    zenith *= (-1);
    arr.push_back(QString("46-").toLatin1());
    QString arg = QString("%1").arg(zenith, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QObject::tr("Зенит выходит за допустимые пределы"); //Argument zenith is out of range");
  }
  return arr;
}

QByteArray Protocol::SetDNAAzimuth(float dnaAzimuth)
{
  QByteArray arr;
  arr.push_back(preambula);
  if (dnaAzimuth >= 0 && dnaAzimuth <= 99.9f)
  {
    arr.push_back(QString("M4").toLatin1());
    QString arg = QString("%1").arg(dnaAzimuth, 4, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else if (dnaAzimuth < 0 && dnaAzimuth >= -99.9f)
  {
    arr.push_back(QString("M5-").toLatin1());
    dnaAzimuth *= (-1);
    QString arg = QString("%1").arg(dnaAzimuth, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(QString(arg).toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QObject::tr("Азимут выходит за допустимые пределы");
  }
  return arr;
}

QByteArray Protocol::SetDNAZenith(float dnaZenith)
{
  QByteArray arr;
  arr.push_back(preambula);
  if (dnaZenith >= 0 && dnaZenith <= 99.9f)
  {
    arr.push_back(QString("N4").toLatin1());
    QString arg = QString("%1").arg(dnaZenith, 4, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else if (dnaZenith < 0 && dnaZenith >= -99.9f)
  {
    arr.push_back(QString("N5-").toLatin1());
    dnaZenith *= (-1);
    QString arg = QString("%1").arg(dnaZenith, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(QString(arg).toLatin1());
    if (useRN)
      arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QObject::tr("Зенит выходит за допустимые пределы");
  }
  return arr;
}


QByteArray Protocol::SetMovementOptions(RotatePlane plane, int acceleration, int velocity, int braking)
{
  QByteArray arr;
  arr.push_back(preambula);
  switch (plane)
  {
  case AZIMUTH:
  {
    arr.push_back(QString("5").toLatin1());
    break;
  }
  case ZENITH:
  {
    arr.push_back(QString("6").toLatin1());
    break;
  }
  default:
    throw QObject::tr("Недопустимая плоскость вращения"); //Invalid rotation rplane"));
  }
  arr.push_back(QString("7").toLatin1());
  if (acceleration >= 1 && acceleration <= 24)
  {
    QString arg = QString("%1").arg(acceleration, 2, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Ускорение выходит за допустимые пределы");//"Argument acceleration is out of range");
  }
  if (velocity >= 0 && velocity <= 250)
  {
    QString arg = QString("%1").arg(velocity, 3, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Cкорость выходит за допустимые пределы");//Argument velocity is out of range");
  }
  if (braking >= 1 && braking <= 24)
  {
    QString arg = QString("%1").arg(braking, 2, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Торможение выходит за допустимые пределы");//Argument braking is out of range");
  }
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetPositioningOptions(RotatePlane plane, int speed, int zeroSeek)
{
  QByteArray arr;
  arr.push_back(preambula);
  switch (plane)
  {
  case AZIMUTH:
  {
    arr.push_back(QString("7").toLatin1());
    break;
  }
  case ZENITH:
  {
    arr.push_back(QString("8").toLatin1());
    break;
  }
  default:
    throw QObject::tr("Недопустимая плоскость вращения");
  }
  arr.push_back(QString("6").toLatin1());
  if (speed >= 0 && speed <= 250)
  {
    QString arg = QString("%1").arg(speed, 3, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Cкорость выходит за допустимые пределы");//"Argument speed is out of range");
  }
  if (zeroSeek >= 0 && zeroSeek <= 250)
  {
    QString arg = QString("%1").arg(zeroSeek, 3, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Ускорение выходит за допустимые пределы");//Argument acceleration is out of range");
  }
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetTempHeating(int heatOnTemp, int heatOffTemp)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("94").toLatin1());
  if (heatOnTemp >= 0 && heatOnTemp <= 99)
  {
    QString arg = QString("%1").arg(heatOnTemp, 2, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Температура включения подогрева выходит за допустимые пределы");//"Argument heat on temperature is out of range");
  }
  if (heatOffTemp >= 0 && heatOffTemp <= 99)
  {
    QString arg = QString("%1").arg(heatOffTemp, 2, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QObject::tr("Температура выключения подогрева выходит за допустимые пределы");//"Argument heat off temperature is out of range");
  }
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetTempVentilation(int ventOnTemp, int ventOffTemp)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("A4").toLatin1());
  if (ventOnTemp >= 0 && ventOnTemp <= 99)
  {
    QString arg = QString("%1").arg(ventOnTemp, 2, 10, QChar('0'));
    arr.push_back(arg.toLatin1());
  }
  else
  {
    throw QObject::tr("Температура включения охлаждения выходит за допустимые пределы"); //"Argument ventilation on temperature is out of range"));
  }
  if (ventOffTemp >= 0 && ventOffTemp <= 99)
  {
    QString arg = QString("%1").arg(ventOffTemp, 2, 10, QChar('0'));
    arr.push_back(arg.toLatin1());
  }
  else
  {
    throw QObject::tr("Температура выключения охлаждения выходит за допустимые пределы");//"Argument ventilation off temperature is out of range");
  }
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::StopEngines()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("B11").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::EnableHeating(bool heat)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("C1").toLatin1());
  if (heat)
    arr.push_back(QString("1").toLatin1());
  else
    arr.push_back(QString("2").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::EnableVentilation(bool ventilation)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("D1").toLatin1());
  if (ventilation)
    arr.push_back(QString("1").toLatin1());
  else
    arr.push_back(QString("2").toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::ResetCommand()
{
  return QString("R").toLatin1();
}

QByteArray Protocol::MoveCommand(RotatePlane plane, int value)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("O%1").arg((int)plane, 1, 10, QChar('0')).toLatin1());
  if (value >= 0)
  {
    arr.push_back(QString("4+").toLatin1());
    arr.push_back(QString("%1").arg(value, 3, 10, QChar('0')).toLatin1());
  }
  else
  {
    arr.push_back(QString("4-").toLatin1());
    value *= (-1);
    arr.push_back(QString("%1").arg(value, 3, 10, QChar('0')).toLatin1());
  }
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SendIP(QString IP)
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("I%1").arg(IP.count()).toLatin1());
  arr.push_back(IP.toLatin1());
  if (useRN)
    arr.push_back(QString("\r\n").toLatin1());
  return arr;
}
