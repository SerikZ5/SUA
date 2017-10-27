#ifndef TelemetryPacket_H
#define TelemetryPacket_H

#include <QString>

struct TelemetryPacket
{
  QString time;
  float latitude;
  float longitude;
  float height;
  QString status;
  float uavLatitude;
  float uavLongitude;
  float uavAltitude;
  float direction;
  float azimuth;
  float zenith;
  QString temperature;
  QString workMode;
  QString error;
  float uavAzimuth;
  float uavZenith;
  uint distance;
  int heightGPS;
};

#endif
