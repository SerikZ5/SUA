#ifndef SUA_H
#define SUA_H

#include <QtWidgets/QMainWindow>
#include <QUdpSocket>
#include <QKeyEvent>
#include <QTranslator>
#include <QComboBox>

#include "ui_sua.h"

#include "ImageStatus.h"
#include "SUASettings.h"
#include "TelemetryDecoder.h"
#include "NetworkBase.h"

class Spoiler;

class SUA : public QMainWindow
{
  Q_OBJECT

public:
  SUA(QWidget *parent = 0);
  ~SUA();

public slots:
  void telelemtryRcvd(RecievedArray arr);
  void updateConsoleWindow(RecievedArray arr);
  void updateSUAModemConsole(RecievedArray arr);

  void updateModemSockLbl(int state);
  void updateTelemSockLbl(int state);
  void updateCommSockLbl(int state);

protected slots:
  void on_actionAll_changed();
  void on_actionReConnect_triggered();
  void on_actionDisconnect_triggered();
  void on_actionNetworkSettings_triggered();
  void on_actionUserMod_triggered();

  void on_btnReConnectUsers_clicked();
  void on_btnOrientedAntennasCommandUsers_clicked();
  void on_btnFollowCommandUsers_clicked();
  void on_btnStopEnginesCommandUsers_clicked();
  void on_btnFullMode_clicked();

  void on_btnSetDNAUpUsers_clicked();
  void on_btnSetDNALeftUsers_clicked();
  void on_btnSetDNARightUsers_clicked();
  void on_btnSetDNADownUsers_clicked();

  void on_btnSetAzimuthCommand_clicked();
  void on_btnSetZenithCommand_clicked();
  void on_txbSuaCommandAzimuthUsers_returnPressed();
  void on_txbSuaCommandZenithUsers_returnPressed();
  void on_txbSuaCommandAzimuth_returnPressed();
  void on_txbSuaCommandZenith_returnPressed();

  void on_SLZen_valueChanged(int value);
  void on_SLZen_sliderReleased();
  void on_SLAzim_valueChanged(int value);
  void on_SLAzim_sliderReleased();

  void on_btnFollowCommand_clicked();
  void on_btnOrientedAntennasCommand_clicked();
  void on_btnSetZeroPositionCommand_clicked();
  void on_btnStopEnginesCommand_clicked();
  void on_btnHeatingCommand_clicked(bool checked);
  void on_btnVentilationCommand_clicked(bool checked);
  void on_btnTestCommand_clicked();
  void on_btnResetCommand_clicked();

  void on_txbSuaControlPacket_returnPressed();
  void on_btnSendSUAControlPacketCommand_clicked();
  
  void btnSetAzimuthMovOptCommand_clicked();
  void txbSetAzimuthMovOptCommand_returnPressed();
  void btnSetAzimuthZerSOptCommand_cliked();
  void txbSetAzimuthZerSOptCommand_returnPressed();
  
  void btnSetZenithMovOptCommand_clicked();
  void txbSetZenithMovOptCommand_returnPressed();
  void btnSetZenithZerSOptCommand_clicked();
  void txbSetZenithZerSOptCommand_returnPressed();

  void btnSetHeatingOptCommand_clicked();
  void txbSetHeatingOptCommand_returnPressed();
  void btnSetVentilationOptCommand_clicked();
  void txbSetVentilationOptCommand_returnPressed();

  void btnSelectLanguage_clicked();
  void chooseLogDir();

private:
  void activateFullMode(bool value);
  void addSpoilers();
  void addStatusBar();
  void tranlate(SUASettings::Languages lng);
  void translateSatatusBar();
  void translateSpoilers();
  
  void updateTextField();

  void updateSUATelemetryConsole(RecievedArray arr);
  void updateTelemetryWorkMode(QString workModeString);
  void updateTelemetryErrors(QString errors);
  void updateTelemetryStatus(QString statusGPSString);
  void updateSUAStateLabels(TelemetryPacket decodedPacket);
  
  void setConsoleCommand(QByteArray command);

  void reconnectAllSockets();
  void disconnectAllSockets();

  void printTelemetryLogFileBegin(QString path);
  void printTelemetryLogFile(QString path, TelemetryPacket packet);
    
  ImageStatus imageStatus;

  SUASettings suaSettings;
  TelemetryDecoder telemetryDecoder;
  QString telemetryLogFile;
  QString telemetryLogDir;

  NetworkBase* commandNetwork;
  NetworkBase* telemetryNetwork;
  NetworkBase* modemNetwork;
  QUdpSocket* udpClient;

  bool userWindows;

  QTranslator qtLanguageTranslator;

  Ui::SUAClass ui;

  QLabel* lblState;
  QLabel* modem;
  QLabel* telemetry;
  QLabel* command;
  QLabel* lblModemSocketState;
  QLabel* lblTelemetrySocketState;
  QLabel* lblCommandSocketState;

  Spoiler* adjustment;
  QLineEdit* txbSuaDNA;
  Spoiler* azimuth;
  QGroupBox* motionAzimuth;
  QLabel* lblAzimuthAcceleraion;
  QLabel* lblAzimuthSpeed;
  QLabel* lblAzimuthBraking;
  QLineEdit* txbAzimuthAcceleraion;
  QLineEdit* txbAzimuthSpeed;
  QLineEdit* txbAzimuthBraking;
  QPushButton* btnSetAzimuthMovOptCommand;
  QGroupBox* additionallyAzimuth;
  QLabel* lblAzimuthSlowSpeed;
  QLabel* lblAzimuthZeroSeek;
  QLineEdit* txbAzimuthSlowSpeed;
  QLineEdit* txbAzimuthZeroSeek;
  QPushButton* btnSetAzimuthZerSOptCommand;
  Spoiler* zenith;
  QGroupBox* motionZenith;
  QLabel* lblZenithAcceleraion;
  QLabel* lblZenithSpeed;
  QLabel* lblZenithBraking;
  QLineEdit* txbZenithAcceleraion;
  QLineEdit* txbZenithSpeed;
  QLineEdit* txbZenithBraking;
  QPushButton* btnSetZenithMovOptCommand;
  QGroupBox* additionallyZenith;
  QLabel* lblZenithSlowSpeed;
  QLabel* lblZenithZeroSeek;
  QLineEdit* txbZenithSlowSpeed;
  QLineEdit* txbZenithZeroSeek;
  QPushButton* btnSetZenithZerSOptCommand;
  Spoiler* heating;
  QLabel* lblTempEnHeating;
  QLabel* lblTempDisHeating;
  QLineEdit* txbTempEnHeating;
  QLineEdit* txbTempDisHeating;
  QPushButton* btnSetHeatingOptCommand;
  Spoiler* ventilation;
  QLabel* lblTempEnVentilation;
  QLabel* lblTempDisVentilation;
  QLineEdit* txbTempEnVentilation;
  QLineEdit* txbTempDisVentilation;
  QPushButton* btnSetVentilationOptCommand;
  Spoiler* language;
  QLabel* lblLanguage;
  QComboBox* cmbLanguage;
  QPushButton* btnLanguageSelect;
  Spoiler* logFile;
  QLabel* lblLogFile;
  QLineEdit* txbLogFile;
  QPushButton* btnSetLogFile;

};

#endif // SUA_H
