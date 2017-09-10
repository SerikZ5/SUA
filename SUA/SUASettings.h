#ifndef SUASettings_H
#define SUASettings_H

#include <QString>

class SUASettings
{
public:
  SUASettings();
  
  int azimuthAcceleraion;
  int azimuthSpeed;
  int azimuthBraking;
  int azimuthSlowSpeed;
  int azimuthZeroSeek;

  int zenithAcceleraion;
  int zenithSpeed;
  int zenithBraking;
  int zenithSlowSpeed;
  int zenithZeroSeek;

  int tempEnHeating;
  int tempDisHeating;

  int tempEnVentilation;
  int tempDisVentilation;

  float azimuthDNA;
  float zenithDNA;

  QString hostAddress;
  int modemPort;
  int commandPort;
  int telemetryPort;  
};

#endif
