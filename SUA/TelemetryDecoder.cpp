#include <QtMath>

#include "TelemetryDecoder.h"
#include "TelemetryPacket.h"

TelemetryDecored::TelemetryDecored()
{
}

TelemetryDecored::~TelemetryDecored()
{

}

void TelemetryDecored::AddBytesToQueue(RecievedArray bytes)
{

}

int TelemetryDecored::Avaible()
{
  inputBytes.count();
}

TelemetryPacket TelemetryDecored::GetTelemetryPacket()
{
  QByteArray arr;
  TelemetryPacket packet;
  if (isSynchonized && inputBytes.count() > 214)
  {
    arr = inputBytes. .Take(215).ToArray();

    for (int i = 0; i < 215; i++)
    {
      inputBytes.Dequeue();
    }
    try
    {
      packet.time = QString(inputBytes.mid(6, 19));
      packet.latitude = QString(inputBytes.mid(44, 9)).replace(".", ",").toFloat();
      packet.longitude = QString(inputBytes.mid(54, 9)).replace(".", ",").toFloat();
      packet.height = QString(inputBytes.mid(72, 8)).replace(".", ",").toFloat();
      packet.status = QString(89, 3);
      packet.uavLatitude = QString(inputBytes.mid(110, 9)).replace(".", ",").toFloat();
      packet.uavLongitude = QString(inputBytes.mid(120, 9)).replace(".", ",").toFloat();
      packet.uavAltitude = QString(inputBytes.mid(141, 8)).replace(".", ",").toFloat();
      packet.direction = QString(inputBytes.mid(156, 5)).replace(".", ",").toFloat();
      packet.azimuth = QString(inputBytes.mid(168, 5)).replace(".", ",").toFloat();
      packet.zenith = QString(inputBytes.mid(174, 5)).replace(".", ",").toFloat();
      packet.temperature = QString(inputBytes.mid(183, 5)).replace(".", ",").toFloat();
      //arr[189] |= 0x80;
      //workmode - QString от int в двоичной системе координат;
      packet.workMode = QString("%1").arg((int)inputBytes[189], 8, 2);
      //arr[191] |= 0x80;
      packet.error = QString("%1").arg((int)inputBytes[191], 8, 2);
      packet.uavAzimuth = QString(inputBytes.mid(200, 5)).replace(".", ",").toFloat();
      packet.uavZenith = QString(inputBytes.mid(206, 5)).replace(".", ",").toFloat();

      double dLatitude = qAbs(packet.latitude - packet.uavLatitude) / 180 * M_PI;
      double dLongitude = qAbs(packet.longitude - packet.uavLongitude) / 180 * M_PI;
      double LatitudeUAV = packet.uavLatitude / 180 * M_PI;
      double LatitudeBase = packet.latitude / 180 * M_PI;
      double a = qPow(qSin(dLatitude / 2), 2) + qCos(LatitudeBase) * qCos(LatitudeUAV) * qPow(qSin(dLongitude / 2), 2);
      double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));
      packet.distance = (uint)(6371000 * c);
      packet.heightGPS = (int)(packet.uavAltitude - packet.height);

      return packet;
    }
    catch
    {
      isSynchonized = false;
    }
  }
  return NULL;
}

void TelemetryDecored::Synchronize()
{

}