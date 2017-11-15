#include <QDir>

#include "SUASettings.h"

SUASettings::SUASettings()
{
  azimuthAcceleraion = 15;
  azimuthSpeed = 200;
  azimuthBraking = 20;
  azimuthSlowSpeed = 20;
  azimuthZeroSeek = 50;

  zenithAcceleraion = 15;
  zenithSpeed = 150;
  zenithBraking = 20;
  zenithSlowSpeed = 20;
  zenithZeroSeek = 50;

  tempEnHeating = 10;
  tempDisHeating = 18;

  tempEnVentilation = 45;
  tempDisVentilation = 35;

  azimuthDNA = 0;
  zenithDNA = 0;

  hostAddress = "10.10.10.250";
  telemetryPort = 8876;
  modemPort = 9876;
  commandPort = 9875;

  language = Languages::ru;
  telemetryLogDir = QDir::current().currentPath() + "/LogFilesSUA";
}
