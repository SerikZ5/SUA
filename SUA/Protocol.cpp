#include <QString>
#include <QStringBuilder>

#include "Protocol.h"

QByteArray Protocol::preambula = QString("SUA").toLatin1();

QByteArray Protocol::OrientedAntennas()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("111\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::FollowCmnd()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("E11\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::TestCmnd()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("F11\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetZeroPos()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("211\r\n").toLatin1());
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
    arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QString("Argument azimuth is out of range");
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
    arr.push_back(QString("\r\n").toLatin1());
  }
  else if (zenith < 0 && zenith >= -5)
  {
    zenith *= (-1);
    arr.push_back(QString("46-").toLatin1());
    QString arg = QString("%1").arg(zenith, 5, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(arg.toLatin1());
    arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QString("Argument zenith is out of range");
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
    arr.push_back(QString("\r\n").toLatin1());
  }
  else if (dnaAzimuth < 0 && dnaAzimuth >= -99.9f)
  {
    arr.push_back(QString("M5-").toLatin1());
    dnaAzimuth *= (-1);
    QString arg = QString("%1").arg(dnaAzimuth, 4, 'f', 1, QChar('0'));
    arg.replace(",", ".");
    arr.push_back(QString(arg).toLatin1());
    arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QString("Argument azimuth is out of range");
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
    string arg = string.Format("{0:00.0}", dnaZenith);
    arg.replace(",", ".");
    arr.push_back(QString(arg).toLatin1());
    arr.push_back(QString("\r\n").toLatin1());
  }
  else if (dnaZenith < 0 && dnaZenith >= -99.9f)
  {
    arr.push_back(QString("N5-").toLatin1());
    dnaZenith *= (-1);
    string arg = string.Format("{0:00.0}", dnaZenith);
    arg.Replace(",", ".");
    arr.push_back(QString(arg).toLatin1());
    arr.push_back(QString("\r\n").toLatin1());
  }
  else
  {
    throw QString("Argument zenith is out of range");
  }
  return arr;
}


QByteArray Protocol::SetMovementOptions(RotatePlane plane, Int32 acceleration, Int32 velocity, Int32 braking)
{
  QByteArray arr;
  arr.push_back(preambula);
  switch (plane)
  {
  case RotatePlane.AZIMUTH:
  {
    arr.push_back(QString("5").toLatin1());
    break;
  }
  case RotatePlane.ZENITH:
  {
    arr.push_back(QString("6").toLatin1());
    break;
  }
  default:
    throw QString("Invalid rotation rplane");
  }
  arr.push_back(QString("7").toLatin1());
  if (acceleration >= 1 && acceleration <= 24)
  {
    string arg = string.Format("{0:00}", acceleration);
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument acceleration is out of range");
  }
  if (velocity >= 0 && velocity <= 250)
  {
    string arg = string.Format("{0:000}", velocity);
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument velocity is out of range");
  }
  if (braking >= 1 && braking <= 24)
  {
    string arg = string.Format("{0:00}", braking);
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument braking is out of range");
  }
  arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::SetPositioningOptions(RotatePlane plane, int speed, int zeroSeek)
{
  QByteArray arr;
  arr.push_back(preambula);
  switch (plane)
  {
  case RotatePlane.AZIMUTH:
  {
    arr.push_back(QString("7").toLatin1());
    break;
  }
  case RotatePlane.ZENITH:
  {
    arr.push_back(QString("8").toLatin1());
    break;
  }
  default:
    throw QString("Invalid rotation rplane");
  }
  arr.push_back(QString("6").toLatin1());
  if (speed >= 0 && speed <= 250)
  {
    string arg = string.Format("{0:000}", speed);
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument speed is out of range");
  }
  if (zeroSeek >= 0 && zeroSeek <= 250)
  {
    QString arg = QString("%1").arg(zeroSeek, 3, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument acceleration is out of range");
  }
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
    throw QString("Argument heat on temperature is out of range");
  }
  if (heatOffTemp >= 0 && heatOffTemp <= 99)
  {
    QString arg = QString("%1").arg(heatOffTemp, 2, 10, QChar('0'));
    arr.push_back(QString(arg).toLatin1());
  }
  else
  {
    throw QString("Argument heat off temperature is out of range");
  }
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
    throw QString("Argument ventilation on temperature is out of range");
  }
  if (ventOffTemp >= 0 && ventOffTemp <= 99)
  {
    QString arg = QString("%1").arg(ventOffTemp, 2, 10, QChar('0'));
    arr.push_back(arg.toLatin1());
  }
  else
  {
    throw QString("Argument ventilation off temperature is out of range");
  }
  arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::StopEngines()
{
  QByteArray arr;
  arr.push_back(preambula);
  arr.push_back(QString("B11\r\n").toLatin1());
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
  arr.push_back(QString("\r\n").toLatin1());
  return arr;
}

QByteArray Protocol::ResetCommand()
{
  return QString("R").toLatin1();
}

int Protocol::DecodeAnsver(QByteArray arr)
{
  return (int)arr[3];
}

void Protocol::DecodeTelemetry(QByteArray arr)
{
}