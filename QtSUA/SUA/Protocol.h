#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>

enum RotatePlane
{
  AZIMUTH = 0,
  ZENITH = 1
};

class Protocol
{
public:
  static QByteArray OrientedAntennas();
  static QByteArray FollowCmnd();
  static QByteArray TestCmnd();
  static QByteArray SetZeroPos();
  static QByteArray SetAzimuth(float azimuth);
  static QByteArray SetZenith(float zenith);
  static QByteArray SetDNAAzimuth(float dnaAzimuth);
  static QByteArray SetDNAZenith(float dnaZenith);
  static QByteArray SetMovementOptions(RotatePlane plane, int acceleration, int velocity, int braking);
  static QByteArray SetPositioningOptions(RotatePlane plane, int speed, int zeroSeek);
  static QByteArray SetTempHeating(int heatOnTemp, int heatOffTemp);
  static QByteArray SetTempVentilation(int ventOnTemp, int ventOffTemp);
  static QByteArray StopEngines();
  static QByteArray EnableHeating(bool heat);
  static QByteArray EnableVentilation(bool ventilation);
  static QByteArray ResetCommand();
  static int DecodeAnsver(QByteArray arr);
  static void DecodeTelemetry(QByteArray arr);

private:
  static QByteArray preambula;
};

#endif