#ifndef TELEMETRY_NETWORK
#define TELEMETRY_NETWORK

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
  float temperature;
  QString workmode;
  QString error;
  float UAVazimuth;
  float UAVzenith;
  uint distance;
  int heightGPS;
};

#endif