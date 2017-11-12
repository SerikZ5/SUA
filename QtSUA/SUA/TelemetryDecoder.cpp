#include <QtMath>
#include <QObject>

#include "TelemetryDecoder.h"

TelemetryDecoder::TelemetryDecoder()
{
  isSynchonized = false;
}

TelemetryDecoder::~TelemetryDecoder()
{
}

int TelemetryDecoder::Avaible()
{
  return inputBytes.count();
}

void TelemetryDecoder::AddBytesToQueue(RecievedArray bytes)
{
  inputBytes.append(bytes.arr);  
  QByteArray arr = inputBytes.left(5);
  if (QString(arr) != "Time:")
  {
    isSynchonized = false;
  }
  Synchronize();
}

void TelemetryDecoder::Synchronize()
{
  isSynchonized = false;
  while (!isSynchonized)
  {
    if (inputBytes.count() < 5)
    {
      break;
    }
    else
    {
      QByteArray arr = inputBytes.left(5);
      if (QString(arr) == "Time:")
      {
        isSynchonized = true;
      }
      else
      {
        inputBytes.remove(0, 1);
      }
    }
  }
}

float QByteArrayToFloat(QByteArray array)
{
  QString str(array);
  str = str.remove(' ');
  return str.toFloat();
}

TelemetryPacket TelemetryDecoder::GetTelemetryPacket(bool* ok)
{
  TelemetryPacket packet;
  if (isSynchonized && inputBytes.count() > 214)
  {
    QByteArray arr = inputBytes.left(215);
    inputBytes.remove(0, 215);
    try
    {
      packet.time = QString(arr.mid(6, 19));
      packet.latitude = QByteArrayToFloat(arr.mid(44, 9));
      packet.longitude = QByteArrayToFloat(arr.mid(54, 9));
      packet.height = QByteArrayToFloat(arr.mid(72, 8));
      packet.status = QString(arr.mid(89, 3));
      packet.uavLatitude = QByteArrayToFloat(arr.mid(110, 9));
      packet.uavLongitude = QByteArrayToFloat(arr.mid(120, 9));
      packet.uavAltitude = QByteArrayToFloat(arr.mid(141, 8));
      packet.direction = QByteArrayToFloat(arr.mid(156, 5));
      packet.azimuth = QByteArrayToFloat(arr.mid(168, 5));
      packet.zenith = QByteArrayToFloat(arr.mid(174, 5));
      packet.temperature = QString(arr.mid(183, 5));
      int workMode = (int)arr[189];
      workMode |= 0x80;
      //workmode - QString от int в двоичной системе координат;
      packet.workMode = QString::number(workMode, 2);
      packet.workMode = packet.workMode.right(8);
      int error = (int)arr[191];
      error |= 0x80;
      packet.error = QString::number(error, 2);
      packet.error = packet.error.right(8);
      packet.uavAzimuth = QByteArrayToFloat(arr.mid(200, 5));
      packet.uavZenith = QByteArrayToFloat(arr.mid(206, 5));

      double dLatitude = qAbs(packet.latitude - packet.uavLatitude) / 180 * M_PI;
      double dLongitude = qAbs(packet.longitude - packet.uavLongitude) / 180 * M_PI;
      double LatitudeUAV = packet.uavLatitude / 180 * M_PI;
      double LatitudeBase = packet.latitude / 180 * M_PI;
      double a = qPow(qSin(dLatitude / 2), 2) + qCos(LatitudeBase) * qCos(LatitudeUAV) * qPow(qSin(dLongitude / 2), 2);
      double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));
      packet.distance = (uint)(6371000 * c);
      packet.heightGPS = (int)(packet.uavAltitude - packet.height);

      *ok = true;
      return packet;
    }
    catch(...)
    {
      isSynchonized = false;
    }
  }

  *ok = false;
  return packet;
}

bool TelemetryDecoder::IsVentilationEnable(QString workMode)
{
  if(workMode[7] == '1')
    return true;
  return false;
}

bool TelemetryDecoder::IsHeatingEnable(QString workMode)
{
  if(workMode[6] == '1')
    return true;
  return false;
}

QString TelemetryDecoder::GetErrors(QString error)
{
  QString result;
  int charCount = error.count();
  if(error[charCount-1] == '1')
    result += QObject::tr("ќшибка 1Wire \n");
  if(error[charCount-2] == '1')
    result +=  QObject::tr("ќшибка BMSD2 \n");
  if(error[charCount-3] == '1')
    result +=  QObject::tr("ќшибка BMSD1 \n");
  if(error[charCount-4] == '1')
    result +=  QObject::tr("ќшибка эмул€ции EEPROM");
  if(result.isEmpty())
    result = "no errors";
  return result;
}
