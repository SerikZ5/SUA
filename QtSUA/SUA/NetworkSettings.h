#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H

#include <QDialog>
#include <QLineEdit>

#include "SUASettings.h"

class NetworkSettings : public QDialog
{
  Q_OBJECT
public:
  NetworkSettings(SUASettings* settings, QString configFilePath, QWidget* parent = NULL);
  virtual ~NetworkSettings();

private slots:
  void buttonOk_click();
  void buttonCancel_click();

private:
  QLineEdit* txtHostAddress;
  QLineEdit* txtModemPort;
  QLineEdit* txtTelemetryPort;
  QLineEdit* txtCommandPort;

  SUASettings* settings;
  QString configFilePath;
};

#endif