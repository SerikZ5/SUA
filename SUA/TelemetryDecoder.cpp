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
      packet.Time = Encoding.ASCII.GetString(arr, 6, 19);//ok
      packet.Latitude = float.Parse(Encoding.ASCII.GetString(arr, 44, 9).Replace(".", ","));//ok
      packet.Longitude = float.Parse(Encoding.ASCII.GetString(arr, 54, 9).Replace(".", ","));//ok
      packet.Height = float.Parse(Encoding.ASCII.GetString(arr, 72, 8).Replace(".", ","));//ok
      packet.Status = Encoding.ASCII.GetString(arr, 89, 3);//ok
      packet.UAVLatitude = float.Parse(Encoding.ASCII.GetString(arr, 110, 9).Replace(".", ","));//ok
      packet.UAVLongitude = float.Parse(Encoding.ASCII.GetString(arr, 120, 9).Replace(".", ","));//ok
      packet.UAVAltitude = float.Parse(Encoding.ASCII.GetString(arr, 141, 8).Replace(".", ","));//ok
      packet.Direction = float.Parse(Encoding.ASCII.GetString(arr, 156, 5).Replace(".", ","));//ok
      packet.Azimuth = float.Parse(Encoding.ASCII.GetString(arr, 168, 5).Replace(".", ","));//ok
      packet.Zenith = float.Parse(Encoding.ASCII.GetString(arr, 174, 5).Replace(".", ","));//ok
      packet.Temperature = float.Parse(Encoding.ASCII.GetString(arr, 183, 5).Replace(".", ","));//ok
      arr[189] |= 0x80;
      packet.WorkMode = Convert.ToString(arr[189], 2);
      arr[191] |= 0x80;
      packet.Error = Convert.ToString(arr[191], 2);
      packet.UAVAzimuth = float.Parse(Encoding.ASCII.GetString(arr, 200, 5).Replace(".", ","));//ok
      packet.UAVZenith = float.Parse(Encoding.ASCII.GetString(arr, 206, 5).Replace(".", ","));//ok

      double dLatitude = qAbs(packet.Latitude - packet.UAVLatitude) / 180 * M_PI;
      double dLongitude = Math.Abs(packet.Longitude - packet.UAVLongitude) / 180 * M_PI;
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