#ifndef SUASettings_H
#define SUASettings_H

#include <QString>
#include <QObject>

class SUASettings : public QObject
{
  Q_OBJECT

public:
  enum Languages
  {
    ru = 0,
    en = 1
  };
  Q_ENUM(Languages)

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

  Languages language;
};

#endif
