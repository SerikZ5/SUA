#include <QValidator>
#include <QMessageBox>
#include <QScrollBar>
#include <QMetaEnum>
#include <QDateTime>
#include <QDir>

#include "sua.h"
#include "Spoiler.h"
#include "Protocol.h"
#include "NetworkSettings.h"
#include "SUASerializer.h"

#define userWindowHeight 560
#define userWindowWight 300
#define mainWindowHeight 560
#define mainWindowWight 720
#define configFilePath "config.conf"

SUA::SUA(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);

  ui.btnSetDNAUpUsers->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp));
  ui.btnSetDNAUpUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNADownUsers->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowDown));
  ui.btnSetDNADownUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNALeftUsers->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
  ui.btnSetDNALeftUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNARightUsers->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));
  ui.btnSetDNARightUsers->setIconSize(QSize(25, 25));
  ui.txbSuaDNAUsers->setValidator(new QDoubleValidator(0, 99, 2));
  addSpoilers();
  addStatusBar();
  activateFullMode(false);
  resize(userWindowWight, userWindowHeight);

  if (!SUASerializer::Deserialize(configFilePath, &suaSettings))
  {
    SUASerializer::Serialize(configFilePath, &suaSettings);
  }
  telemetryLogFile = "";
    
  commandNetwork = new NetworkBase(suaSettings.hostAddress, suaSettings.commandPort);
  connect(commandNetwork, SIGNAL(NetworkStateChanged(int)), this, SLOT(updateCommSockLbl(int)));
  connect(commandNetwork, SIGNAL(DataRecieved(RecievedArray)), this, SLOT(updateConsoleWindow(RecievedArray)));

  telemetryNetwork = new NetworkBase(suaSettings.hostAddress, suaSettings.telemetryPort);
  connect(telemetryNetwork, SIGNAL(NetworkStateChanged(int)), this, SLOT(updateTelemSockLbl(int)));
  connect(telemetryNetwork, SIGNAL(DataRecieved(RecievedArray)), this, SLOT(telelemtryRcvd(RecievedArray)));

  modemNetwork = new NetworkBase(suaSettings.hostAddress, suaSettings.modemPort);
  connect(modemNetwork, SIGNAL(NetworkStateChanged(int)), this, SLOT(updateModemSockLbl(int)));
  connect(modemNetwork, SIGNAL(DataRecieved(RecievedArray)), this, SLOT(updateSUAModemConsole(RecievedArray)));

  udpClient = new QUdpSocket(this);

  imageStatus.SetImage(ui.imgConnectState, SUAImages::GREY);
  imageStatus.SetImage(ui.imgGPSState, SUAImages::GREY);
  imageStatus.SetImage(ui.imgFollowState, SUAImages::GREY);
  imageStatus.SetImage(ui.imgOrientationState, SUAImages::GREY);
  imageStatus.SetImage(ui.imgStopState, SUAImages::GREY);
  updateTextField();
  ui.widget->setEnabled(false);
}

SUA::~SUA()
{
  disconnectAllSockets();
  delete commandNetwork;
  delete telemetryNetwork;
  delete modemNetwork;
  delete udpClient;
}

void SUA::addSpoilers()
{
  /*Spoiler* control = new Spoiler("Управление"), 300, this);
  QVBoxLayout* controlSpoilerLayout = new QVBoxLayout(control);
  QPushButton* btnFollowCommand = new QPushButton("Режим слежения"), control);
  QPushButton* btnOrientedAntennasCommand = new QPushButton("Ориентация"), control);
  QPushButton* btnSetZeroPositionCommand = new QPushButton("Установить ноль"), control);
  QPushButton* btnStopEnginesCommand = new QPushButton("Стоп"), control);
  QPushButton* btnEnableHeatingCommand = new QPushButton("Вкл. подогрев"), control);
  QPushButton* btnDisableHeatingCommand = new QPushButton("Выкл. подогрев"), control);
  QPushButton* btnEnableVentilationCommand = new QPushButton("Вкл. охлаждение"), control);
  QPushButton* btnDisableVentilationCommand = new QPushButton("Выкл. охлаждение"), control);
  QPushButton* btnTestCommand = new QPushButton("Тестовый режим"), control);
  QPushButton* btnResetCommand = new QPushButton("Сброс"), control);
  controlSpoilerLayout->addWidget(btnFollowCommand);
  controlSpoilerLayout->addWidget(btnOrientedAntennasCommand);
  controlSpoilerLayout->addWidget(btnSetZeroPositionCommand);
  controlSpoilerLayout->addWidget(btnStopEnginesCommand);
  controlSpoilerLayout->addWidget(btnEnableHeatingCommand);
  controlSpoilerLayout->addWidget(btnDisableHeatingCommand);
  controlSpoilerLayout->addWidget(btnEnableVentilationCommand);
  controlSpoilerLayout->addWidget(btnDisableVentilationCommand);
  controlSpoilerLayout->addWidget(btnTestCommand);
  controlSpoilerLayout->addWidget(btnResetCommand);
  control->setContentLayout(controlSpoilerLayout);



  Spoiler* userControl = new Spoiler("Ручной режим управления"), 300, this);
  QVBoxLayout* userControlSpoilerLayout = new QVBoxLayout(userControl);

  QGroupBox* groupAzimuth = new QGroupBox(userControl);
  groupAzimuth->setTitle("Азимут"));
  QHBoxLayout* azimuthLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandAzimuth = new QLineEdit(groupAzimuth);
  QPushButton* btnSetAzimuthCommand = new QPushButton("Ввод"), groupAzimuth);
  azimuthLayout->addWidget(txbSuaCommandAzimuth);
  azimuthLayout->addWidget(btnSetAzimuthCommand);
  groupAzimuth->setLayout(azimuthLayout);
  QGroupBox* groupElevationAngle = new QGroupBox(userControl);
  groupElevationAngle->setTitle("Угол места"));
  QHBoxLayout* ElevationAngleLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandZenith = new QLineEdit(groupAzimuth);
  QPushButton* btnSetZenithCommand = new QPushButton("Ввод"), groupAzimuth);
  ElevationAngleLayout->addWidget(txbSuaCommandZenith);
  ElevationAngleLayout->addWidget(btnSetZenithCommand);
  groupElevationAngle->setLayout(ElevationAngleLayout);

  userControlSpoilerLayout->addWidget(groupAzimuth);
  userControlSpoilerLayout->addWidget(groupElevationAngle);
  userControl->setContentLayout(userControlSpoilerLayout);


  Spoiler* options = new Spoiler("Опции"), 300, this);*/


  QWidget* w = new QWidget(ui.tabWidget);
  w->setObjectName(QStringLiteral("w"));
  Spoiler* adjustment = new Spoiler("Корректировка", 300, w);
  QVBoxLayout* adjustmentSpoilerLayout = new QVBoxLayout();
  adjustmentSpoilerLayout->setSpacing(6);
  QHBoxLayout* h1 = new QHBoxLayout();
  h1->setSpacing(6);
  QPushButton* btnSetDNAUp = new QPushButton(adjustment);
  btnSetDNAUp->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp));
  btnSetDNAUp->setIconSize(QSize(25, 25));
  btnSetDNAUp->setMinimumSize(QSize(30, 30));
  btnSetDNAUp->setMaximumSize(QSize(30, 30));
  h1->addWidget(btnSetDNAUp);
  adjustmentSpoilerLayout->addLayout(h1);
  QHBoxLayout* h2 = new QHBoxLayout();
  h2->setSpacing(6);
  QSpacerItem* spacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  QSpacerItem* spacer2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  h2->addItem(spacer1);
  QPushButton* btnSetDNALeft = new QPushButton(adjustment);
  btnSetDNALeft->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
  btnSetDNALeft->setIconSize(QSize(25, 25));
  btnSetDNALeft->setMinimumSize(QSize(30, 30));
  btnSetDNALeft->setMaximumSize(QSize(30, 30));
  h2->addWidget(btnSetDNALeft);
  txbSuaDNA = new QLineEdit(adjustment);
  txbSuaDNA->setMinimumSize(QSize(30, 30));
  txbSuaDNA->setMaximumSize(QSize(30, 30));
  txbSuaDNA->setValidator(new QDoubleValidator(0, 99, 2));
  h2->addWidget(txbSuaDNA);
  QPushButton* btnSetDNARight = new QPushButton(adjustment);
  btnSetDNARight->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));
  btnSetDNARight->setIconSize(QSize(25, 25));
  btnSetDNARight->setMinimumSize(QSize(30, 30));
  btnSetDNARight->setMaximumSize(QSize(30, 30));
  h2->addWidget(btnSetDNARight);
  h2->addItem(spacer2);
  adjustmentSpoilerLayout->addLayout(h2);
  QHBoxLayout* h3 = new QHBoxLayout();
  h3->setSpacing(6);
  QPushButton* btnSetDNADown = new QPushButton(adjustment);
  btnSetDNADown->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowDown));
  btnSetDNADown->setIconSize(QSize(25, 25));
  btnSetDNADown->setMinimumSize(QSize(30, 30));
  btnSetDNADown->setMaximumSize(QSize(30, 30));
  h3->addWidget(btnSetDNADown);
  adjustmentSpoilerLayout->addLayout(h3);
  adjustment->setContentLayout(adjustmentSpoilerLayout);
  connect(btnSetDNAUp, SIGNAL(clicked()), this, SLOT(on_btnSetDNAUpUsers_clicked()));
  connect(btnSetDNALeft, SIGNAL(clicked()), this, SLOT(on_btnSetDNALeftUsers_clicked()));
  connect(btnSetDNARight, SIGNAL(clicked()), this, SLOT(on_btnSetDNARightUsers_clicked()));
  connect(btnSetDNADown, SIGNAL(clicked()), this, SLOT(on_btnSetDNADownUsers_clicked()));


  Spoiler* azimuth = new Spoiler("Азимут", 300, w);
  QVBoxLayout* azimuthSpoilerLayout = new QVBoxLayout(azimuth);
  QGroupBox* motionAzimuth = new QGroupBox("Движение", azimuth);
  QVBoxLayout* motionAzimuthLayout = new QVBoxLayout();
  QLabel* l1 = new QLabel("Ускорение", motionAzimuth);
  txbAzimuthAcceleraion = new QLineEdit("15", motionAzimuth);
  txbAzimuthAcceleraion->setValidator(new QIntValidator());
  QLabel* l2 = new QLabel("Постоянная скорость", motionAzimuth);
  txbAzimuthSpeed = new QLineEdit("150", motionAzimuth);
  txbAzimuthSpeed->setValidator(new QIntValidator());
  QLabel* l3 = new QLabel("Торможение", motionAzimuth);
  txbAzimuthBraking = new QLineEdit("20", motionAzimuth);
  txbAzimuthBraking->setValidator(new QIntValidator());
  QPushButton* btnSetAzimuthMovOptCommand = new QPushButton("Ввод", motionAzimuth);
  motionAzimuthLayout->addWidget(l1);
  motionAzimuthLayout->addWidget(txbAzimuthAcceleraion);
  motionAzimuthLayout->addWidget(l2);
  motionAzimuthLayout->addWidget(txbAzimuthSpeed);
  motionAzimuthLayout->addWidget(l3);
  motionAzimuthLayout->addWidget(txbAzimuthBraking);
  motionAzimuthLayout->addWidget(btnSetAzimuthMovOptCommand);
  motionAzimuth->setLayout(motionAzimuthLayout);
  connect(txbAzimuthAcceleraion, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(txbAzimuthSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(txbAzimuthBraking, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(btnSetAzimuthMovOptCommand, SIGNAL(pressed()), this, SLOT(btnSetAzimuthMovOptCommand_clicked()));

  QGroupBox* additionallyAzimuth = new QGroupBox("Дополнительно", azimuth);
  QVBoxLayout* additionallyAzimuthLayout = new QVBoxLayout();
  QLabel* l4 = new QLabel("Скорость поиска нуля", additionallyAzimuth);
  txbAzimuthSlowSpeed = new QLineEdit("20", additionallyAzimuth);
  txbAzimuthSlowSpeed->setValidator(new QIntValidator());
  QLabel* l5 = new QLabel("Скорость подхода к точке", additionallyAzimuth);
  txbAzimuthZeroSeek = new QLineEdit("50", additionallyAzimuth);
  txbAzimuthZeroSeek->setValidator(new QIntValidator());
  QPushButton* btnSetAzimuthZerSOptCommand = new QPushButton("Ввод", additionallyAzimuth);
  additionallyAzimuthLayout->addWidget(l4);
  additionallyAzimuthLayout->addWidget(txbAzimuthSlowSpeed);
  additionallyAzimuthLayout->addWidget(l5);
  additionallyAzimuthLayout->addWidget(txbAzimuthZeroSeek);
  additionallyAzimuthLayout->addWidget(btnSetAzimuthZerSOptCommand);
  additionallyAzimuth->setLayout(additionallyAzimuthLayout);
  connect(txbAzimuthSlowSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthZerSOptCommand_returnPressed()));
  connect(txbAzimuthZeroSeek, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthZerSOptCommand_returnPressed()));
  connect(btnSetAzimuthZerSOptCommand, SIGNAL(pressed()), this, SLOT(btnSetAzimuthZerSOptCommand_cliked()));

  azimuthSpoilerLayout->addWidget(motionAzimuth);
  azimuthSpoilerLayout->addWidget(additionallyAzimuth);
  azimuth->setContentLayout(azimuthSpoilerLayout);

  
  Spoiler* zenith = new Spoiler("Угол места", 300, w);
  QVBoxLayout* zenithSpoilerLayout = new QVBoxLayout(zenith);
  QGroupBox* motionZenith = new QGroupBox("Движение", zenith);
  QVBoxLayout* motionZenithLayout = new QVBoxLayout();
  QLabel* l6 = new QLabel("Ускорение", motionZenith);
  txbZenithAcceleraion = new QLineEdit("15", motionZenith);
  txbZenithAcceleraion->setValidator(new QIntValidator());
  QLabel* l7 = new QLabel("Постоянная скорость", motionZenith);
  txbZenithSpeed = new QLineEdit("150", motionZenith);
  txbZenithSpeed->setValidator(new QIntValidator());
  QLabel* l8 = new QLabel("Торможение", motionZenith);
  txbZenithBraking = new QLineEdit("20", motionZenith);
  txbZenithBraking->setValidator(new QIntValidator());
  QPushButton* btnSetZenithMovOptCommand = new QPushButton("Ввод", motionZenith);
  motionZenithLayout->addWidget(l6);
  motionZenithLayout->addWidget(txbZenithAcceleraion);
  motionZenithLayout->addWidget(l7);
  motionZenithLayout->addWidget(txbZenithSpeed);
  motionZenithLayout->addWidget(l8);
  motionZenithLayout->addWidget(txbZenithBraking);
  motionZenithLayout->addWidget(btnSetZenithMovOptCommand);
  motionZenith->setLayout(motionZenithLayout);
  connect(txbZenithAcceleraion, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(txbZenithSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(txbZenithBraking, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(btnSetZenithMovOptCommand, SIGNAL(pressed()), this, SLOT(btnSetZenithMovOptCommand_clicked()));

  QGroupBox* additionallyZenith = new QGroupBox("Дополнительно", azimuth);
  QVBoxLayout* additionallyZenithLayout = new QVBoxLayout();
  QLabel* l9 = new QLabel("Скорость поиска нуля", additionallyZenith);
  txbZenithSlowSpeed = new QLineEdit("20", additionallyZenith);
  txbZenithSlowSpeed->setValidator(new QIntValidator());
  QLabel* l10 = new QLabel("Скорость подхода к точке", additionallyZenith);
  txbZenithZeroSeek = new QLineEdit("50", additionallyZenith);
  txbZenithZeroSeek->setValidator(new QIntValidator());
  QPushButton* btnSetZenithZerSOptCommand = new QPushButton("Ввод", additionallyZenith);
  additionallyZenithLayout->addWidget(l9);
  additionallyZenithLayout->addWidget(txbZenithSlowSpeed);
  additionallyZenithLayout->addWidget(l10);
  additionallyZenithLayout->addWidget(txbZenithZeroSeek);
  additionallyZenithLayout->addWidget(btnSetZenithZerSOptCommand);
  additionallyZenith->setLayout(additionallyZenithLayout);
  connect(txbZenithSlowSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetZenithZerSOptCommand_returnPressed()));
  connect(txbZenithZeroSeek, SIGNAL(returnPressed()), this, SLOT(txbSetZenithZerSOptCommand_returnPressed()));
  connect(btnSetZenithZerSOptCommand, SIGNAL(pressed()), this, SLOT(btnSetZenithZerSOptCommand_clicked()));

  zenithSpoilerLayout->addWidget(motionZenith);
  zenithSpoilerLayout->addWidget(additionallyZenith);
  zenith->setContentLayout(zenithSpoilerLayout);


  Spoiler* heating = new Spoiler("Подогрев", 300, w);
  QVBoxLayout* heatingSpoilerLayout = new QVBoxLayout(zenith);
  QLabel* l11 = new QLabel("Температура включения", heating);
  txbTempEnHeating = new QLineEdit("10", heating);
  txbTempEnHeating->setValidator(new QIntValidator());
  QLabel* l12 = new QLabel("Температура выключения", heating);
  txbTempDisHeating = new QLineEdit("18", heating);
  txbTempDisHeating->setValidator(new QIntValidator());
  QPushButton* btnSetHeatingOptCommand = new QPushButton("Ввод", heating);
  heatingSpoilerLayout->addWidget(l11);
  heatingSpoilerLayout->addWidget(txbTempEnHeating);
  heatingSpoilerLayout->addWidget(l12);
  heatingSpoilerLayout->addWidget(txbTempDisHeating);
  heatingSpoilerLayout->addWidget(btnSetHeatingOptCommand);
  heating->setContentLayout(heatingSpoilerLayout);
  connect(txbTempEnHeating, SIGNAL(returnPressed()), this, SLOT(txbSetHeatingOptCommand_returnPressed()));
  connect(txbTempDisHeating, SIGNAL(returnPressed()), this, SLOT(txbSetHeatingOptCommand_returnPressed()));
  connect(btnSetHeatingOptCommand, SIGNAL(pressed()), this, SLOT(btnSetHeatingOptCommand_clicked()));

  Spoiler* ventilation = new Spoiler("Охлаждение", 300, w);
  QVBoxLayout* ventilationSpoilerLayout = new QVBoxLayout(zenith);
  QLabel* l13 = new QLabel("Температура включения", ventilation);
  txbTempEnVentilation = new QLineEdit("45", ventilation);
  txbTempEnVentilation->setValidator(new QIntValidator());
  QLabel* l14 = new QLabel("Температура выключения", ventilation);
  txbTempDisVentilation = new QLineEdit("35", ventilation);
  txbTempDisVentilation->setValidator(new QIntValidator());
  QPushButton* btnSetVentilationOptCommand = new QPushButton("Ввод", ventilation);
  ventilationSpoilerLayout->addWidget(l13);
  ventilationSpoilerLayout->addWidget(txbTempEnVentilation);
  ventilationSpoilerLayout->addWidget(l14);
  ventilationSpoilerLayout->addWidget(txbTempDisVentilation);
  ventilationSpoilerLayout->addWidget(btnSetVentilationOptCommand);
  ventilation->setContentLayout(ventilationSpoilerLayout);
  connect(txbTempEnVentilation, SIGNAL(returnPressed()), this, SLOT(txbSetVentilationOptCommand_returnPressed()));
  connect(txbTempDisVentilation, SIGNAL(returnPressed()), this, SLOT(txbSetVentilationOptCommand_returnPressed()));
  connect(btnSetVentilationOptCommand, SIGNAL(pressed()), this, SLOT(btnSetVentilationOptCommand_clicked()));

  QVBoxLayout* optionsSpoilerLayout = new QVBoxLayout();
  optionsSpoilerLayout->addWidget(adjustment);
  optionsSpoilerLayout->addWidget(azimuth);
  optionsSpoilerLayout->addWidget(zenith);
  optionsSpoilerLayout->addWidget(heating);
  optionsSpoilerLayout->addWidget(ventilation);
  QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  optionsSpoilerLayout->addItem(verticalSpacer);

  w->setLayout(optionsSpoilerLayout);
  w->setStyleSheet("#w { background-color:white; }");
  ui.scrollArea->setWidget(w);
}

void SUA::addStatusBar()
{
  QLabel* l = new QLabel("Состояние", this);
  l->setStyleSheet("font-weight: bold;");
  QLabel* modem = new QLabel("Порт модема", this);
  QLabel* telemetry = new QLabel("Порт телеметрии", this);
  QLabel* command = new QLabel("Порт управления", this);
  lblModemSocketState = new QLabel(this);
  lblTelemetrySocketState = new QLabel(this);
  lblCommandSocketState = new QLabel(this);
  lblCommandSocketState->setText("NOT_CONNECTED");
  lblTelemetrySocketState->setText("NOT_CONNECTED");
  lblModemSocketState->setText("NOT_CONNECTED");
  statusBar()->addWidget(l);
  statusBar()->addWidget(modem);
  statusBar()->addWidget(lblModemSocketState);
  statusBar()->addWidget(telemetry);
  statusBar()->addWidget(lblTelemetrySocketState);
  statusBar()->addWidget(command);
  statusBar()->addWidget(lblCommandSocketState);
}

void SUA::activateFullMode(bool value)
{
  ui.toolBar->setVisible(value);
  statusBar()->setVisible(value);
  ui.statusBar->setVisible(value);
  ui.mainFrame->setVisible(value);
  ui.tabWidget->setVisible(value);
  ui.usersFrame->setVisible(!value);
}

void SUA::on_actionReConnect_triggered()
{
  reconnectAllSockets();
}

void SUA::on_actionDisconnect_triggered()
{
  disconnectAllSockets();
}

void SUA::on_actionNetworkSettings_triggered()
{
  NetworkSettings networkSettings(&suaSettings, configFilePath, this);
  networkSettings.exec();
}

void SUA::on_actionUserMod_triggered()
{
  activateFullMode(false);
  resize(userWindowWight, userWindowHeight);
}

void SUA::on_actionAll_changed()
{
  bool value = ui.actionAll->isChecked();
  ui.actionModem->setChecked(value);
  ui.actionTelemetry->setChecked(value);
  ui.actionCommand->setChecked(value);
}

void SUA::on_btnFullMode_clicked()
{
  activateFullMode(true);
  resize(mainWindowWight, mainWindowHeight);
}

void SUA::on_btnReConnectUsers_clicked()
{
  reconnectAllSockets();
}

void SUA::on_btnOrientedAntennasCommandUsers_clicked()
{
  on_btnOrientedAntennasCommand_clicked();
}

void SUA::on_btnFollowCommandUsers_clicked()
{
  on_btnFollowCommand_clicked();
}

void SUA::on_btnStopEnginesCommandUsers_clicked()
{
  on_btnStopEnginesCommand_clicked();
}

void SUA::on_btnSetDNAUpUsers_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaDNAUsers->text();
      }
      else
      {
        text = txbSuaDNA->text();
      }
      text = text.replace(".", ",");
      suaSettings.zenithDNA = text.toFloat();
      QByteArray comm = Protocol::SetDNAZenith(suaSettings.zenithDNA);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_btnSetDNADownUsers_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaDNAUsers->text();
      }
      else
      {
        text = txbSuaDNA->text();
      }
      text = text.replace(".", ",");
      suaSettings.zenithDNA = text.toFloat();
      suaSettings.zenithDNA *= (-1);
      QByteArray comm = Protocol::SetDNAZenith(suaSettings.zenithDNA);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_btnSetDNALeftUsers_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaDNAUsers->text();
      }
      else
      {
        text = txbSuaDNA->text();
      }
      text = text.replace(".", ",");
      suaSettings.azimuthDNA = text.toFloat();
      suaSettings.azimuthDNA *= (-1);
      QByteArray comm = Protocol::SetDNAAzimuth(suaSettings.azimuthDNA);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_btnSetDNARightUsers_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaDNAUsers->text();
      }
      else
      {
        text = txbSuaDNA->text();
      }
      text = text.replace(".", ",");
      suaSettings.azimuthDNA = text.toFloat();
      QByteArray comm = Protocol::SetDNAAzimuth(suaSettings.azimuthDNA);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_btnSetAzimuthCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaCommandAzimuthUsers->text();
      }
      else
      {
        text = ui.txbSuaCommandAzimuth->text();
      }
      text = text.replace(".", ",");
      float arg = text.toFloat();
      QByteArray comm = Protocol::SetAzimuth(arg);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_btnSetZenithCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text;
      if (userWindows == true)
      {
        text = ui.txbSuaCommandZenithUsers->text();
      }
      else
      {
        text = ui.txbSuaCommandZenith->text();
      }
      text = text.replace(".", ",");
      float arg = text.toFloat();
      QByteArray comm = Protocol::SetZenith(arg);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Некорректное значение!");
    }
  }
}

void SUA::on_txbSuaCommandAzimuth_returnPressed()
{
  on_btnSetAzimuthCommand_clicked();
}

void SUA::on_txbSuaCommandZenith_returnPressed()
{
  on_btnSetZenithCommand_clicked();
}

void SUA::on_txbSuaCommandAzimuthUsers_returnPressed()
{
  on_btnSetAzimuthCommand_clicked();
}

void SUA::on_txbSuaCommandZenithUsers_returnPressed()
{
  on_btnSetZenithCommand_clicked();
}

void SUA::on_SLZen_valueChanged(int value)
{
  QByteArray comm = Protocol::SetZenith(value);
  commandNetwork->Send(comm);
}

void SUA::on_SLAzim_valueChanged(int value)
{
  QByteArray comm = Protocol::SetAzimuth(value);
  commandNetwork->Send(comm);
}

void SUA::on_btnFollowCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::FollowCmnd();
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnOrientedAntennasCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::OrientedAntennas();
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnSetZeroPositionCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::SetZeroPos();
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnStopEnginesCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::StopEngines();
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnEnableHeatingCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableHeating(true);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnDisableHeatingCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableHeating(false);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnEnableVentilationCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableVentilation(true);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnDisableVentilationCommand_clicked() 
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableVentilation(false);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnTestCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::TestCmnd();
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::on_btnResetCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::ResetCommand();
    udpClient->writeDatagram(comm, QHostAddress(suaSettings.hostAddress), 69);
    setConsoleCommand(comm);
  }
}

void SUA::btnSetAzimuthMovOptCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbAzimuthAcceleraion->text();
      text = text.replace(".", ",");
      suaSettings.azimuthAcceleraion = text.toInt();
      text = txbAzimuthSpeed->text();
      text = text.replace(".", ",");
      suaSettings.azimuthSpeed = text.toInt();
      text = txbAzimuthBraking->text();
      text = text.replace(".", ",");
      suaSettings.azimuthBraking = text.toInt();
      QByteArray comm = Protocol::SetMovementOptions(RotatePlane::AZIMUTH, suaSettings.azimuthAcceleraion, suaSettings.azimuthSpeed, suaSettings.azimuthBraking);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetAzimuthMovOptCommand_returnPressed()
{
  btnSetAzimuthMovOptCommand_clicked();
}

void SUA::btnSetAzimuthZerSOptCommand_cliked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbAzimuthSlowSpeed->text();
      text = text.replace(".", ",");
      suaSettings.azimuthSlowSpeed = text.toInt();
      text = txbAzimuthZeroSeek->text();
      text = text.replace(".", ",");
      suaSettings.azimuthZeroSeek = text.toInt();
      QByteArray comm = Protocol::SetPositioningOptions(RotatePlane::AZIMUTH, suaSettings.azimuthSlowSpeed, suaSettings.azimuthZeroSeek);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetAzimuthZerSOptCommand_returnPressed()
{
  btnSetAzimuthZerSOptCommand_cliked();
}

void SUA::btnSetZenithMovOptCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbZenithAcceleraion->text();
      text = text.replace(".", ",");
      suaSettings.zenithAcceleraion = text.toInt();
      text = txbZenithSpeed->text();
      text = text.replace(".", ",");
      suaSettings.zenithSpeed = text.toInt();
      text = txbZenithBraking->text();
      text = text.replace(".", ",");
      suaSettings.zenithBraking = text.toInt();
      QByteArray comm = Protocol::SetMovementOptions(RotatePlane::ZENITH, suaSettings.zenithAcceleraion, suaSettings.zenithSpeed, suaSettings.zenithBraking);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetZenithMovOptCommand_returnPressed()
{
  btnSetZenithMovOptCommand_clicked();
}

void SUA::btnSetZenithZerSOptCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbZenithSlowSpeed->text();
      text = text.replace(".", ",");
      suaSettings.zenithSlowSpeed = text.toInt();
      text = txbZenithZeroSeek->text();
      text = text.replace(".", ",");
      suaSettings.zenithZeroSeek = text.toInt();
      QByteArray comm = Protocol::SetPositioningOptions(RotatePlane::ZENITH, suaSettings.zenithSlowSpeed, suaSettings.zenithZeroSeek);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetZenithZerSOptCommand_returnPressed()
{
  btnSetZenithZerSOptCommand_clicked();
}

void SUA::btnSetHeatingOptCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbTempEnHeating->text();
      text = text.replace(".", ",");
      suaSettings.tempEnHeating = text.toInt();
      text = txbTempDisHeating->text();
      text = text.replace(".", ",");
      suaSettings.tempDisHeating = text.toInt();
      QByteArray comm = Protocol::SetTempHeating(suaSettings.tempEnHeating, suaSettings.tempDisHeating);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetHeatingOptCommand_returnPressed()
{
  btnSetHeatingOptCommand_clicked();
}

void SUA::btnSetVentilationOptCommand_clicked()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    try
    {
      QString text = txbTempEnVentilation->text();
      text = text.replace(".", ",");
      suaSettings.tempEnVentilation = text.toInt();
      text = txbTempDisVentilation->text();
      text = text.replace(".", ",");
      suaSettings.tempDisVentilation = text.toInt();
      QByteArray comm = Protocol::SetTempVentilation(suaSettings.tempEnVentilation, suaSettings.tempDisVentilation);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, "Warning!", "Uncorrect values!");
    }
  }
}

void SUA::txbSetVentilationOptCommand_returnPressed()
{
  btnSetVentilationOptCommand_clicked();
}

void SUA::on_txbSuaControlPacket_returnPressed()
{
  on_btnSendSUAControlPacketCommand_clicked();
}

void SUA::on_btnSendSUAControlPacketCommand_clicked()
{
  if (ui.txbSuaControlPacket->text().length() > 0 && commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QString text = ui.txbSuaControlPacket->text();
    QByteArray comm(text.toLatin1());
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
}

void SUA::updateTextField()
{
  txbAzimuthAcceleraion->text() = QString::number(suaSettings.azimuthAcceleraion);
  txbAzimuthBraking->text() = QString::number(suaSettings.azimuthBraking);
  txbAzimuthSlowSpeed->text() = QString::number(suaSettings.azimuthSlowSpeed);
  txbAzimuthSpeed->text() = QString::number(suaSettings.azimuthSpeed);
  txbAzimuthZeroSeek->text() = QString::number(suaSettings.azimuthZeroSeek);

  txbZenithAcceleraion->text() = QString::number(suaSettings.zenithAcceleraion);
  txbZenithBraking->text() = QString::number(suaSettings.zenithBraking);
  txbZenithSlowSpeed->text() = QString::number(suaSettings.zenithSlowSpeed);
  txbZenithSpeed->text() = QString::number(suaSettings.zenithSpeed);
  txbZenithZeroSeek->text() = QString::number(suaSettings.zenithZeroSeek);

  txbTempEnHeating->text() = QString::number(suaSettings.tempEnHeating);
  txbTempDisHeating->text() = QString::number(suaSettings.tempDisHeating);

  txbTempEnVentilation->text() = QString::number(suaSettings.tempEnVentilation);
  txbTempDisVentilation->text() = QString::number(suaSettings.tempDisVentilation);
}

void SUA::updateSUAStateLeds(QString workModeString, QString statusGPSString)
{
  try
  {
    if (statusGPSString.length() == 2)
    {
      statusGPSString[2] = '0';
    }

    if (statusGPSString[0] == '1' && (statusGPSString[1] == 'A' || statusGPSString[2] == 'A'))
    {
      imageStatus.SetImage(ui.imgGPSState, SUAImages::GREEN);
      ui.btnFollowCommandUsers->setEnabled(true);
    }
    else if (statusGPSString[0] == '0' || statusGPSString[1] == 'V')
    {
      imageStatus.SetImage(ui.imgGPSState, SUAImages::YELLOW_BLINK);
      ui.btnFollowCommandUsers->setEnabled(false);
    }
    else
    {
      imageStatus.SetImage(ui.imgGPSState, SUAImages::GREY);
      ui.btnFollowCommandUsers->setEnabled(false);
    }

    //Idle mode (STOP lable is GREEN)
    if (workModeString[7] == '1')
    {
      imageStatus.SetImage(ui.imgStopState, SUAImages::GREEN);
      //imageStatus.SetImage(ref imgMoveState, "GREY");
    }
    else
    {
      imageStatus.SetImage(ui.imgStopState, SUAImages::GREY);
    }

    //Orientation Mode (Orientation lable is GREEN_BLINK)
    if (workModeString[6] == '1' || workModeString[5] == '1')
    {
      imageStatus.SetImage(ui.imgOrientationState, SUAImages::GREEN_BLINK);
    }
    else
    {
      imageStatus.SetImage(ui.imgOrientationState, SUAImages::GREY);
    }

    //Following Mode (Follow lable is GREEN)
    if (workModeString[4] == '0')
    {
      imageStatus.SetImage(ui.imgFollowState, SUAImages::GREEN);
    }
    else
    {
      imageStatus.SetImage(ui.imgFollowState, SUAImages::GREY);
    }
  }
  catch (...)
  {
    QMessageBox::warning(this, "Warning!", "Не корректные входные данные!\n");
  }
}

void SUA::updateSUAStateLabels(TelemetryPacket packet)
{
  ui.lblSUATime->setText(packet.time);
  ui.lblLatitude->setText(QString::number(packet.latitude));
  ui.lblLongitude->setText(QString::number(packet.longitude));
  ui.lblAltitude->setText(QString::number(packet.height));
  ui.lblStatus->setText(packet.status);
  ui.lblUAVLatitude->setText(QString::number(packet.uavLatitude));
  ui.lblUAVLongitude->setText(QString::number(packet.uavLongitude));
  ui.lblUAVAltitude->setText(QString::number(packet.uavAltitude));
  ui.lblDirection->setText(QString::number(packet.direction));
  ui.lblAzimuth->setText(QString::number(packet.azimuth));
  /**/
  ui.txbSuaCommandAzimuthUsers->setText(QString::number(packet.azimuth));
  ui.txbSuaCommandZenithUsers->setText(QString::number(packet.zenith));
  if (!userWindows)
  {
    ui.SLZen->setValue(packet.zenith);
    ui.SLAzim->setValue(packet.azimuth);
  }
  /**/
  ui.lblZenith->setText(QString::number(packet.zenith));
  ui.lblUAVAzimuth->setText(QString::number(packet.uavAzimuth));
  ui.lblUAVZenith->setText(QString::number(packet.uavZenith));
  ui.lblTemp->setText(QString::number(packet.temperature));
  ui.lblWorkMode->setText(packet.workMode);
  ui.lblError->setText(packet.error);
  ui.lblDistanceUser->setText(QString::number(packet.distance) + " м");
  ui.lblDistance->setText(QString::number(packet.distance) + " м");
  ui.lblHeightGPSUser->setText(QString::number(packet.heightGPS) + " м");
  ui.lblHeightGPS->setText(QString::number(packet.heightGPS) + " м");
}

void SUA::telelemtryRcvd(RecievedArray arr)
{
  updateSUATelemetryConsole(arr);
  telemetryDecoder.AddBytesToQueue(arr);
  bool ok;
  TelemetryPacket packet = telemetryDecoder.GetTelemetryPacket(&ok);
  if (ok)
  {
    updateSUAStateLabels(packet);
    updateSUAStateLeds(packet.workMode, packet.status);
    if (telemetryLogFile != "")
      printTelemetryLogFile(telemetryLogFile, packet);
  }
}

void SUA::updateSUATelemetryConsole(RecievedArray arr)
{
  ui.txbTelemetryWindow->append(arr.ToString());
  QScrollBar* scroll = ui.txbTelemetryWindow->verticalScrollBar();
  scroll->setValue(scroll->maximum());
}

void SUA::updateSUAModemConsole(RecievedArray arr)
{
  ui.txbModemConsole->append("----------------------\n");
  ui.txbModemConsole->append(arr.ToHexString());
  ui.txbModemConsole->append("\n");
  QScrollBar* scroll = ui.txbModemConsole->verticalScrollBar();
  scroll->setValue(scroll->maximum());
}

void SUA::updateConsoleWindow(RecievedArray arr)
{
  QString str = arr.ToString();
  ui.txbConsoleWindow->append(">>>" + str + "\n");
  QScrollBar* scroll = ui.txbConsoleWindow->verticalScrollBar();
  scroll->setValue(scroll->maximum());
  txbSuaDNA->setText("");
  ui.txbSuaDNAUsers->setText("");
}

void SUA::updateCommSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::SocketState>();
  lblCommandSocketState->setText(metaEnum.valueToKey(state));

  switch (state)
  {
  case NetworkBase::SocketState::CONNECTED:
    ui.widget->setEnabled(true);
    imageStatus.SetImage(ui.imgConnectState, SUAImages::GREEN);
    break;
  case NetworkBase::SocketState::NOT_CONNECTED:
    ui.widget->setEnabled(false);
  default:
    imageStatus.SetImage(ui.imgConnectState, SUAImages::GREY);
    imageStatus.SetImage(ui.imgFollowState, SUAImages::GREY);
    imageStatus.SetImage(ui.imgGPSState, SUAImages::GREY);
    //imageStatus.SetImage(ui.imgMoveState, "GREY");
    imageStatus.SetImage(ui.imgOrientationState, SUAImages::GREY);
    imageStatus.SetImage(ui.imgStopState, SUAImages::GREY);
    break;
  }
}

void SUA::updateModemSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::SocketState>();
  lblModemSocketState->setText(metaEnum.valueToKey(state));
}

void SUA::updateTelemSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::SocketState>();
  lblTelemetrySocketState->setText(metaEnum.valueToKey(state));
}

void SUA::reconnectAllSockets()
{
  imageStatus.SetImage(ui.imgConnectState, SUAImages::YELLOW_BLINK);
  if (ui.usersFrame->isVisible())
  {
    ui.actionAll->setChecked(true);
  }
  disconnectAllSockets();
  if (ui.actionCommand->isChecked())
  {
    lblCommandSocketState->setText("Connecting... Wait!");
    commandNetwork->ConnectToHost(suaSettings.hostAddress, suaSettings.commandPort);
  }
  if (ui.actionTelemetry->isChecked())
  {
    lblTelemetrySocketState->setText("Connecting... Wait!");
    telemetryNetwork->ConnectToHost(suaSettings.hostAddress, suaSettings.telemetryPort);
    QString timeSystem = QDateTime::currentDateTime().toString("yyyy.MM.dd_HH-mm-ss");
    QDir().mkdir("LogFilesSUA");
    telemetryLogFile = QString("LogFilesSUA/LogFile_%1.txt").arg(timeSystem);
    printTelemetryLogFileBegin(telemetryLogFile);
  }
  if (ui.actionModem->isChecked())
  {
    lblModemSocketState->setText("Connecting... Wait");
    modemNetwork->ConnectToHost(suaSettings.hostAddress, suaSettings.modemPort);
  }
}

void SUA::disconnectAllSockets()
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    lblCommandSocketState->setText("Disconnecting... Wait!");
    commandNetwork->DisconnectFromHost();
  }
  else
  {
    lblCommandSocketState->setText("NOT_CONNECTED");
  }
  if (modemNetwork->State() == NetworkBase::CONNECTED)
  {
    lblModemSocketState->setText("Disconnecting... Wait!");
    modemNetwork->DisconnectFromHost();
  }
  else
  {
    lblModemSocketState->setText("NOT_CONNECTED");
  }
  if (telemetryNetwork->State() == NetworkBase::CONNECTED)
  {
    lblTelemetrySocketState->setText("Disconnecting... Wait!");
    telemetryNetwork->DisconnectFromHost();

    telemetryLogFile = "";
  }
  else
  {
    lblTelemetrySocketState->setText("NOT_CONNECTED");
  }
}

void SUA::setConsoleCommand(QByteArray command)
{
  ui.txbConsoleWindow->append("<<< " + QString(command));
  QScrollBar* scroll = ui.txbConsoleWindow->verticalScrollBar();
  scroll->setValue(scroll->maximum());
}

void SUA::printTelemetryLogFileBegin(QString path)
{
  QFile data(path);
  if (data.open(QFile::WriteOnly | QFile::Truncate)) 
  {
    QTextStream out(&data);
    out << "time_date;base_lat;base_long;base_alt;base_status;uav_lat;uav_long;uav_alt;dir;enc_az;enc_zen;temp;calc_az;calc_zen;work;error;distance;height_GPS/n";
    out << "/n";
  }
}

void SUA::printTelemetryLogFile(QString path, TelemetryPacket packet)
{
  QFile data(path);
  if (data.open(QFile::WriteOnly | QFile::Truncate))
  {
    QTextStream out(&data);
    out << packet.time + ';' + packet.latitude + ';' + packet.longitude + ';' + packet.height + ';' + packet.status + ';' +
      packet.uavLatitude + ';' + packet.uavLongitude + ';' + packet.uavAltitude + ';' + packet.direction + ';' +
      packet.azimuth + ';' + packet.zenith + ';' + packet.temperature + ';' + packet.uavAzimuth + ';' + packet.uavZenith + ';' +
      packet.workMode + ';' + packet.error + ';' + packet.distance + ';' + packet.heightGPS;
    out << "/n";
  }
}
