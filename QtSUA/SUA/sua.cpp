#include <QValidator>
#include <QMessageBox>
#include <QScrollBar>
#include <QMetaEnum>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>

#include "sua.h"
#include "Spoiler.h"
#include "Protocol.h"
#include "NetworkSettings.h"
#include "SUASerializer.h"

#define userWindowHeight 560
#define userWindowWight 310
#define mainWindowHeight 700
#define mainWindowWight 820
#define configFilePath "config.conf"
#define languageFile "SUA_"

SUA::SUA(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  Qt::WindowFlags flags = windowFlags();
  flags ^= Qt::WindowMaximizeButtonHint;
  flags ^= Qt::WindowMinimizeButtonHint;
  setWindowFlags(flags);

  if (!SUASerializer::Deserialize(configFilePath, &suaSettings))
  {
    SUASerializer::Serialize(configFilePath, &suaSettings);
  }

  ui.btnSetDNAUpUsers->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
  ui.btnSetDNAUpUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNADownUsers->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
  ui.btnSetDNADownUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNALeftUsers->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
  ui.btnSetDNALeftUsers->setIconSize(QSize(25, 25));
  ui.btnSetDNARightUsers->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
  ui.btnSetDNARightUsers->setIconSize(QSize(25, 25));
  ui.txbSuaDNAUsers->setValidator(new QDoubleValidator(0, 99, 2));
  addSpoilers();
  addStatusBar();
  activateFullMode(false);
  resize(userWindowWight, userWindowHeight);
  tranlate(suaSettings.language);

  if(suaSettings.telemetryLogDir.isEmpty())
    suaSettings.telemetryLogDir = QDir::currentPath() + "/LogFiles";
  txbLogFile->setText(suaSettings.telemetryLogDir);
    
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

  imageStatus.SetImage(ui.imgConnectState, GREY);
  imageStatus.SetImage(ui.imgGPSState, GREY);
  imageStatus.SetImage(ui.imgFollowState, GREY);
  imageStatus.SetImage(ui.imgOrientationState, GREY);
  imageStatus.SetImage(ui.imgStopState, GREY);
  updateTextField();

  ui.actionAll->setChecked(true);
  reconnectAllSockets();

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
  QWidget* w = new QWidget(ui.tabWidget);
  w->setObjectName(QStringLiteral("w"));
  adjustment = new Spoiler(tr("Корректировка"), 300, w); // Adjustment
  QVBoxLayout* adjustmentSpoilerLayout = new QVBoxLayout();
  adjustmentSpoilerLayout->setSpacing(6);
  QHBoxLayout* h1 = new QHBoxLayout();
  h1->setSpacing(6);
  QPushButton* btnSetDNAUp = new QPushButton(adjustment);
  btnSetDNAUp->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
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
  btnSetDNALeft->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
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
  btnSetDNARight->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
  btnSetDNARight->setIconSize(QSize(25, 25));
  btnSetDNARight->setMinimumSize(QSize(30, 30));
  btnSetDNARight->setMaximumSize(QSize(30, 30));
  h2->addWidget(btnSetDNARight);
  h2->addItem(spacer2);
  adjustmentSpoilerLayout->addLayout(h2);
  QHBoxLayout* h3 = new QHBoxLayout();
  h3->setSpacing(6);
  QPushButton* btnSetDNADown = new QPushButton(adjustment);
  btnSetDNADown->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
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


  azimuth = new Spoiler(tr("Азимут"), 300, w); // Azimuth
  QVBoxLayout* azimuthSpoilerLayout = new QVBoxLayout();
  motionAzimuth = new QGroupBox(azimuth);
  QVBoxLayout* motionAzimuthLayout = new QVBoxLayout();
  lblAzimuthAcceleraion = new QLabel(motionAzimuth);
  txbAzimuthAcceleraion = new QLineEdit("15", motionAzimuth);
  txbAzimuthAcceleraion->setValidator(new QIntValidator());
  lblAzimuthSpeed = new QLabel(motionAzimuth);
  txbAzimuthSpeed = new QLineEdit("150", motionAzimuth);
  txbAzimuthSpeed->setValidator(new QIntValidator());
  lblAzimuthBraking = new QLabel(motionAzimuth);
  txbAzimuthBraking = new QLineEdit("20", motionAzimuth);
  txbAzimuthBraking->setValidator(new QIntValidator());
  btnSetAzimuthMovOptCommand = new QPushButton(motionAzimuth);
  btnSetAzimuthMovOptCommand->setMinimumHeight(23);
  motionAzimuthLayout->addWidget(lblAzimuthAcceleraion);
  motionAzimuthLayout->addWidget(txbAzimuthAcceleraion);
  motionAzimuthLayout->addWidget(lblAzimuthSpeed);
  motionAzimuthLayout->addWidget(txbAzimuthSpeed);
  motionAzimuthLayout->addWidget(lblAzimuthBraking);
  motionAzimuthLayout->addWidget(txbAzimuthBraking);
  motionAzimuthLayout->addWidget(btnSetAzimuthMovOptCommand);
  motionAzimuth->setLayout(motionAzimuthLayout);
  connect(txbAzimuthAcceleraion, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(txbAzimuthSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(txbAzimuthBraking, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthMovOptCommand_returnPressed()));
  connect(btnSetAzimuthMovOptCommand, SIGNAL(pressed()), this, SLOT(btnSetAzimuthMovOptCommand_clicked()));

  additionallyAzimuth = new QGroupBox(azimuth);
  QVBoxLayout* additionallyAzimuthLayout = new QVBoxLayout();
  lblAzimuthSlowSpeed = new QLabel(additionallyAzimuth);
  txbAzimuthSlowSpeed = new QLineEdit("20", additionallyAzimuth);
  txbAzimuthSlowSpeed->setValidator(new QIntValidator());
  lblAzimuthZeroSeek = new QLabel(additionallyAzimuth);
  txbAzimuthZeroSeek = new QLineEdit("50", additionallyAzimuth);
  txbAzimuthZeroSeek->setValidator(new QIntValidator());
  btnSetAzimuthZerSOptCommand = new QPushButton(additionallyAzimuth);
  additionallyAzimuthLayout->addWidget(lblAzimuthSlowSpeed);
  additionallyAzimuthLayout->addWidget(txbAzimuthSlowSpeed);
  additionallyAzimuthLayout->addWidget(lblAzimuthZeroSeek);
  additionallyAzimuthLayout->addWidget(txbAzimuthZeroSeek);
  additionallyAzimuthLayout->addWidget(btnSetAzimuthZerSOptCommand);
  additionallyAzimuth->setLayout(additionallyAzimuthLayout);
  connect(txbAzimuthSlowSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthZerSOptCommand_returnPressed()));
  connect(txbAzimuthZeroSeek, SIGNAL(returnPressed()), this, SLOT(txbSetAzimuthZerSOptCommand_returnPressed()));
  connect(btnSetAzimuthZerSOptCommand, SIGNAL(pressed()), this, SLOT(btnSetAzimuthZerSOptCommand_cliked()));

  azimuthSpoilerLayout->addWidget(motionAzimuth);
  azimuthSpoilerLayout->addWidget(additionallyAzimuth);
  azimuth->setContentLayout(azimuthSpoilerLayout);

  
  zenith = new Spoiler(tr("Угол места"), 300, w);
  QVBoxLayout* zenithSpoilerLayout = new QVBoxLayout();
  motionZenith = new QGroupBox(zenith);
  QVBoxLayout* motionZenithLayout = new QVBoxLayout();
  lblZenithAcceleraion = new QLabel(motionZenith);
  txbZenithAcceleraion = new QLineEdit("15", motionZenith);
  txbZenithAcceleraion->setValidator(new QIntValidator());
  lblZenithSpeed = new QLabel(motionZenith);
  txbZenithSpeed = new QLineEdit("150", motionZenith);
  txbZenithSpeed->setValidator(new QIntValidator());
  lblZenithBraking = new QLabel(motionZenith);
  txbZenithBraking = new QLineEdit("20", motionZenith);
  txbZenithBraking->setValidator(new QIntValidator());
  btnSetZenithMovOptCommand = new QPushButton(motionZenith);
  btnSetZenithMovOptCommand->setMinimumHeight(23);
  motionZenithLayout->addWidget(lblZenithAcceleraion);
  motionZenithLayout->addWidget(txbZenithAcceleraion);
  motionZenithLayout->addWidget(lblZenithSpeed);
  motionZenithLayout->addWidget(txbZenithSpeed);
  motionZenithLayout->addWidget(lblZenithBraking);
  motionZenithLayout->addWidget(txbZenithBraking);
  motionZenithLayout->addWidget(btnSetZenithMovOptCommand);
  motionZenith->setLayout(motionZenithLayout);
  connect(txbZenithAcceleraion, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(txbZenithSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(txbZenithBraking, SIGNAL(returnPressed()), this, SLOT(txbSetZenithMovOptCommand_returnPressed()));
  connect(btnSetZenithMovOptCommand, SIGNAL(pressed()), this, SLOT(btnSetZenithMovOptCommand_clicked()));

  additionallyZenith = new QGroupBox(azimuth);
  QVBoxLayout* additionallyZenithLayout = new QVBoxLayout();
  lblZenithSlowSpeed = new QLabel(additionallyZenith);
  txbZenithSlowSpeed = new QLineEdit("20", additionallyZenith);
  txbZenithSlowSpeed->setValidator(new QIntValidator());
  lblZenithZeroSeek = new QLabel(additionallyZenith);
  txbZenithZeroSeek = new QLineEdit("50", additionallyZenith);
  txbZenithZeroSeek->setValidator(new QIntValidator());
  btnSetZenithZerSOptCommand = new QPushButton(additionallyZenith);
  additionallyZenithLayout->addWidget(lblZenithSlowSpeed);
  additionallyZenithLayout->addWidget(txbZenithSlowSpeed);
  additionallyZenithLayout->addWidget(lblZenithZeroSeek);
  additionallyZenithLayout->addWidget(txbZenithZeroSeek);
  additionallyZenithLayout->addWidget(btnSetZenithZerSOptCommand);
  additionallyZenith->setLayout(additionallyZenithLayout);
  connect(txbZenithSlowSpeed, SIGNAL(returnPressed()), this, SLOT(txbSetZenithZerSOptCommand_returnPressed()));
  connect(txbZenithZeroSeek, SIGNAL(returnPressed()), this, SLOT(txbSetZenithZerSOptCommand_returnPressed()));
  connect(btnSetZenithZerSOptCommand, SIGNAL(pressed()), this, SLOT(btnSetZenithZerSOptCommand_clicked()));

  zenithSpoilerLayout->addWidget(motionZenith);
  zenithSpoilerLayout->addWidget(additionallyZenith);
  zenith->setContentLayout(zenithSpoilerLayout);

  heating = new Spoiler(tr("Подогрев"), 300, w);
  QVBoxLayout* heatingSpoilerLayout = new QVBoxLayout();
  lblTempEnHeating = new QLabel(heating);
  txbTempEnHeating = new QLineEdit("10", heating);
  txbTempEnHeating->setValidator(new QIntValidator());
  lblTempDisHeating= new QLabel(heating);
  txbTempDisHeating = new QLineEdit("18", heating);
  txbTempDisHeating->setValidator(new QIntValidator());
  btnSetHeatingOptCommand = new QPushButton(heating);
  heatingSpoilerLayout->addWidget(lblTempEnHeating);
  heatingSpoilerLayout->addWidget(txbTempEnHeating);
  heatingSpoilerLayout->addWidget(lblTempDisHeating);
  heatingSpoilerLayout->addWidget(txbTempDisHeating);
  heatingSpoilerLayout->addWidget(btnSetHeatingOptCommand);
  heating->setContentLayout(heatingSpoilerLayout);
  connect(txbTempEnHeating, SIGNAL(returnPressed()), this, SLOT(txbSetHeatingOptCommand_returnPressed()));
  connect(txbTempDisHeating, SIGNAL(returnPressed()), this, SLOT(txbSetHeatingOptCommand_returnPressed()));
  connect(btnSetHeatingOptCommand, SIGNAL(pressed()), this, SLOT(btnSetHeatingOptCommand_clicked()));

  ventilation = new Spoiler(tr("Охлаждение"), 300, w);
  QVBoxLayout* ventilationSpoilerLayout = new QVBoxLayout();
  lblTempEnVentilation = new QLabel(ventilation);
  txbTempEnVentilation = new QLineEdit("45", ventilation);
  txbTempEnVentilation->setValidator(new QIntValidator());
  lblTempDisVentilation = new QLabel(ventilation);
  txbTempDisVentilation = new QLineEdit("35", ventilation);
  txbTempDisVentilation->setValidator(new QIntValidator());
  btnSetVentilationOptCommand = new QPushButton(ventilation);
  ventilationSpoilerLayout->addWidget(lblTempEnVentilation);
  ventilationSpoilerLayout->addWidget(txbTempEnVentilation);
  ventilationSpoilerLayout->addWidget(lblTempDisVentilation);
  ventilationSpoilerLayout->addWidget(txbTempDisVentilation);
  ventilationSpoilerLayout->addWidget(btnSetVentilationOptCommand);
  ventilation->setContentLayout(ventilationSpoilerLayout);
  connect(txbTempEnVentilation, SIGNAL(returnPressed()), this, SLOT(txbSetVentilationOptCommand_returnPressed()));
  connect(txbTempDisVentilation, SIGNAL(returnPressed()), this, SLOT(txbSetVentilationOptCommand_returnPressed()));
  connect(btnSetVentilationOptCommand, SIGNAL(pressed()), this, SLOT(btnSetVentilationOptCommand_clicked()));

  language = new Spoiler(tr("Язык"), 300, w);
  QVBoxLayout* languageSpoilerLayout = new QVBoxLayout();
  lblLanguage = new QLabel(language);
  cmbLanguage = new QComboBox(language);
  btnLanguageSelect = new QPushButton(language);
  languageSpoilerLayout->addWidget(lblLanguage);
  languageSpoilerLayout->addWidget(cmbLanguage);
  languageSpoilerLayout->addWidget(btnLanguageSelect);
  language->setContentLayout(languageSpoilerLayout);
  connect(btnLanguageSelect, SIGNAL(pressed()), this, SLOT(btnSelectLanguage_clicked()));

  logFile = new Spoiler(tr("Файл логирования"), 300, w);
  QVBoxLayout* logFileSpoilerLayout = new QVBoxLayout();
  lblLogFile = new QLabel(logFile);
  txbLogFile = new QLineEdit(this);
  btnSetLogFile = new QPushButton(this);
  logFileSpoilerLayout->addWidget(lblLogFile);
  logFileSpoilerLayout->addWidget(txbLogFile);
  logFileSpoilerLayout->addWidget(btnSetLogFile);
  logFile->setContentLayout(logFileSpoilerLayout);
  connect(btnSetLogFile, SIGNAL(pressed()), this, SLOT(chooseLogDir()));

  QVBoxLayout* optionsSpoilerLayout = new QVBoxLayout();
  optionsSpoilerLayout->addWidget(adjustment);
  optionsSpoilerLayout->addWidget(azimuth);
  optionsSpoilerLayout->addWidget(zenith);
  optionsSpoilerLayout->addWidget(heating);
  optionsSpoilerLayout->addWidget(ventilation);
  optionsSpoilerLayout->addWidget(language);
  optionsSpoilerLayout->addWidget(logFile);
  QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  optionsSpoilerLayout->addItem(verticalSpacer);

  w->setLayout(optionsSpoilerLayout);
  w->setStyleSheet("#w { background-color:white; }");
  ui.scrollArea->setWidget(w);
}

void SUA::translateSpoilers()
{
  adjustment->SetTitle(tr("Корректировка"));
  azimuth->SetTitle(tr("Азимут"));
  motionAzimuth->setTitle(tr("Движение"));
  lblAzimuthAcceleraion->setText(tr("Ускорение"));
  lblAzimuthSpeed->setText(tr("Постоянная скорость"));
  lblAzimuthBraking->setText(tr("Торможение"));
  btnSetAzimuthMovOptCommand->setText(tr("Ввод"));
  additionallyAzimuth->setTitle(tr("Дополнительно"));
  lblAzimuthSlowSpeed->setText(tr("Скорость поиска нуля"));
  lblAzimuthZeroSeek->setText(tr("Скорость подхода к точке"));
  btnSetAzimuthZerSOptCommand->setText(tr("Ввод"));
  zenith->SetTitle(tr("Угол места"));
  motionZenith->setTitle(tr("Движение"));
  lblZenithAcceleraion->setText(tr("Ускорение"));
  lblZenithSpeed->setText(tr("Постоянная скорость"));
  lblZenithBraking->setText(tr("Торможение"));
  btnSetZenithMovOptCommand->setText(tr("Ввод"));
  additionallyZenith->setTitle(tr("Дополнительно"));
  lblZenithSlowSpeed->setText(tr("Скорость поиска нуля"));
  lblZenithZeroSeek->setText(tr("Скорость подхода к точке"));
  btnSetZenithZerSOptCommand->setText(tr("Ввод"));
  heating->SetTitle(tr("Подогрев"));
  lblTempEnHeating->setText(tr("Температура включения"));
  lblTempDisHeating->setText(tr("Температура выключения"));
  btnSetHeatingOptCommand->setText(tr("Ввод"));
  ventilation->SetTitle(tr("Охлаждение"));
  lblTempEnVentilation->setText(tr("Температура включения"));
  lblTempDisVentilation->setText(tr("Температура выключения"));
  btnSetVentilationOptCommand->setText(tr("Ввод"));
  language->SetTitle(tr("Язык"));
  lblLanguage->setText(tr("Выберите язык"));
  QStringList languages;
  languages << tr("Русский") << tr("Английский");
  cmbLanguage->clear();
  cmbLanguage->addItems(languages);
  switch(suaSettings.language)
  {
   case SUASettings::ru:
    cmbLanguage->setCurrentIndex(0);
    break;
  case SUASettings::en:
    cmbLanguage->setCurrentIndex(1);
    break;
  }
  btnLanguageSelect->setText(tr("Установить"));
  logFile->SetTitle(tr("Файл логирования"));
  lblLogFile->setText(tr("Выберите папку для файлов"));
  btnSetLogFile->setText(tr("Выбрать"));
}

void SUA::addStatusBar()
{
  lblState = new QLabel(this);
  lblState->setStyleSheet("font-weight: bold;");
  modem = new QLabel(this);
  telemetry = new QLabel(this);
  command = new QLabel(this);
  lblModemSocketState = new QLabel(this);
  lblTelemetrySocketState = new QLabel(this);
  lblCommandSocketState = new QLabel(this);
  lblCommandSocketState->setText("NOT_CONNECTED");
  lblTelemetrySocketState->setText("NOT_CONNECTED");
  lblModemSocketState->setText("NOT_CONNECTED");
  lblCommandSocketState->setStyleSheet("background-color:pink;");
  lblTelemetrySocketState->setStyleSheet("background-color:pink;");
  lblModemSocketState->setStyleSheet("background-color:pink;");
  statusBar()->addWidget(lblState);
  statusBar()->addWidget(modem);
  statusBar()->addWidget(lblModemSocketState);
  statusBar()->addWidget(telemetry);
  statusBar()->addWidget(lblTelemetrySocketState);
  statusBar()->addWidget(command);
  statusBar()->addWidget(lblCommandSocketState);
}

void SUA::translateSatatusBar()
{
  lblState->setText(tr("Состояние"));
  modem->setText(tr("Порт модема"));
  telemetry->setText(tr("Порт телеметрии"));
  command->setText(tr("Порт управления"));
}

void SUA::tranlate(SUASettings::Languages lng)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<SUASettings::Languages>();
  qtLanguageTranslator.load(QString(languageFile) + metaEnum.valueToKey(lng));
  qApp->installTranslator(&qtLanguageTranslator);
  translateSatatusBar();
  translateSpoilers();
  ui.retranslateUi(this);
}

void SUA::activateFullMode(bool value)
{
  userWindows = !value;
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

void SUA::SendIP(QString IP)
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::SendIP(IP);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
  //reconnectAllSockets();
}

void SUA::on_actionUserMod_triggered()
{
  activateFullMode(false);
  setFixedSize(userWindowWight, userWindowHeight);
}

void SUA::on_actionLogFiles_triggered()
{
  QDesktopServices::openUrl(QUrl("file:///" + suaSettings.telemetryLogDir, QUrl::TolerantMode));
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
  setFixedSize(mainWindowWight, mainWindowHeight);
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::MoveCommand(ZENITH, value);
    commandNetwork->Send(comm);
  }
}

void SUA::on_SLZen_sliderReleased()
{
  ui.SLZen->setValue(0);
}

void SUA::on_SLAzim_valueChanged(int value)
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::MoveCommand(AZIMUTH, value);
    commandNetwork->Send(comm);
  }
}

void SUA::on_SLAzim_sliderReleased()
{
  ui.SLAzim->setValue(0);
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

void SUA::on_btnStopEnginesCommand2_clicked()
{
  on_btnStopEnginesCommand_clicked();
}

void SUA::on_btnHeatingCommand_clicked(bool checked)
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableHeating(checked);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
  else
  {
    ui.btnHeatingCommand->setChecked(false);
  }
}

void SUA::on_btnHeatingCommand_toggled(bool value)
{
  QString styleSheet = value ? "background-color:#FFDB8B" : "background-color:#D3D3D3";
  ui.btnHeatingCommand->setStyleSheet(styleSheet);
}

void SUA::on_btnVentilationCommand_clicked(bool checked)
{
  if (commandNetwork->State() == NetworkBase::CONNECTED)
  {
    QByteArray comm = Protocol::EnableVentilation(checked);
    commandNetwork->Send(comm);
    setConsoleCommand(comm);
  }
  else
  {
    ui.btnVentilationCommand->setChecked(false);
  }
}

void SUA::on_btnVentilationCommand_toggled(bool value)
{
  QString styleSheet = value ? "background-color:#FFDB8B" : "background-color:#D3D3D3";
  ui.btnVentilationCommand->setStyleSheet(styleSheet);
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
      QByteArray comm = Protocol::SetMovementOptions(AZIMUTH, suaSettings.azimuthAcceleraion, suaSettings.azimuthSpeed, suaSettings.azimuthBraking);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QByteArray comm = Protocol::SetPositioningOptions(AZIMUTH, suaSettings.azimuthSlowSpeed, suaSettings.azimuthZeroSeek);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QByteArray comm = Protocol::SetMovementOptions(ZENITH, suaSettings.zenithAcceleraion, suaSettings.zenithSpeed, suaSettings.zenithBraking);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QByteArray comm = Protocol::SetPositioningOptions(ZENITH, suaSettings.zenithSlowSpeed, suaSettings.zenithZeroSeek);
      commandNetwork->Send(comm);
      setConsoleCommand(comm);
      SUASerializer::Serialize(configFilePath, &suaSettings);
    }
    catch (...)
    {
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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
      QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректное значение!"));
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

void SUA::btnSelectLanguage_clicked()
{
  suaSettings.language = (SUASettings::Languages)cmbLanguage->currentIndex();
  tranlate(suaSettings.language);
  SUASerializer::Serialize(configFilePath, &suaSettings);
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

void SUA::updateTelemetryWorkMode(QString workModeString)
{
  try
  {
    //Idle mode (STOP lable is GREEN)
    if (workModeString[7] == '1')
    {
      imageStatus.SetImage(ui.imgStopState, GREEN);
      //imageStatus.SetImage(ref imgMoveState, "GREY");
    }
    else
    {
      imageStatus.SetImage(ui.imgStopState, GREY);
    }

    //Orientation Mode (Orientation lable is GREEN_BLINK)
    if (workModeString[6] == '1' || workModeString[5] == '1')
    {
      imageStatus.SetImage(ui.imgOrientationState, GREEN_BLINK);
    }
    else
    {
      imageStatus.SetImage(ui.imgOrientationState, GREY);
    }

    //Following Mode (Follow lable is GREEN)
    if (workModeString[4] == '0')
    {
      imageStatus.SetImage(ui.imgFollowState, GREEN);
    }
    else
    {
      imageStatus.SetImage(ui.imgFollowState, GREY);
    }

    /*//Idle mode (STOP lable is GREEN)
    if (workModeString[0] == '1')
    {
      imageStatus.SetImage(ui.imgStopState, GREEN);
      //imageStatus.SetImage(ref imgMoveState, "GREY");
    }
    else
    {
      imageStatus.SetImage(ui.imgStopState, GREY);
    }

    //Orientation Mode (Orientation lable is GREEN_BLINK)
    if (workModeString[1] == '1' || workModeString[2] == '1')
    {
      imageStatus.SetImage(ui.imgOrientationState, GREEN_BLINK);
    }
    else
    {
      imageStatus.SetImage(ui.imgOrientationState, GREY);
    }

    //Following Mode (Follow lable is GREEN)
    if (workModeString[3] == '0')
    {
      imageStatus.SetImage(ui.imgFollowState, GREEN);
    }
    else
    {
      imageStatus.SetImage(ui.imgFollowState, GREY);
    }*/

    ui.btnHeatingCommand->blockSignals(true);
    ui.btnHeatingCommand->setChecked(workModeString[1] == '1');
    ui.btnHeatingCommand->blockSignals(false);

    ui.btnVentilationCommand->blockSignals(true);
    ui.btnVentilationCommand->setChecked(workModeString[0] == '1');
    ui.btnVentilationCommand->blockSignals(false);

  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректные входные данные!\n"));
  }
}

void SUA::updateTelemetryErrors(QString errors)
{
  try
  {    
    ui.lblError->setToolTip(errors);
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректные входные данные!\n"));
  }
}

void SUA::updateTelemetryStatus(QString statusGPSString)
{
  try
  {
    if (statusGPSString.length() == 2)
    {
      statusGPSString[2] = '0';
    }
    if (statusGPSString[0] == '1' && (statusGPSString[1] == 'A' || statusGPSString[2] == 'A'))
    {
      imageStatus.SetImage(ui.imgGPSState, GREEN);
      ui.btnFollowCommandUsers->setEnabled(true);
    }
    else if (statusGPSString[0] == '0' || statusGPSString[1] == 'V')
    {
      imageStatus.SetImage(ui.imgGPSState, YELLOW_BLINK);
      ui.btnFollowCommandUsers->setEnabled(false);
    }
    else
    {
      imageStatus.SetImage(ui.imgGPSState, GREY);
      ui.btnFollowCommandUsers->setEnabled(false);
    }
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Предупреждение!"), tr("Некорректные входные данные!\n"));
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
  /**/
  ui.lblZenith->setText(QString::number(packet.zenith));
  ui.lblUAVAzimuth->setText(QString::number(packet.uavAzimuth));
  ui.lblUAVZenith->setText(QString::number(packet.uavZenith));
  ui.lblTemp->setText(packet.temperature);
  ui.lblWorkMode->setText(packet.workMode);
  ui.lblError->setText(packet.error);
  ui.lblDistanceUser->setText(QString::number(packet.distance) + " м");
  ui.lblDistance->setText(QString::number(packet.distance) + " м");
  ui.lblHeightGPSUser->setText(QString::number(packet.heightGPS) + " м");
  ui.lblHeightGPS->setText(QString::number(packet.heightGPS) + " м");
  update();
}

void SUA::telelemtryRcvd(RecievedArray arr)
{
  telemetryDecoder.AddBytesToQueue(arr);
  bool ok;
  TelemetryPacket packet = telemetryDecoder.GetTelemetryPacket(&ok);
  if (ok)
  {
    updateSUATelemetryConsole(packet);
    updateSUAStateLabels(packet);
    updateTelemetryStatus(packet.status);
    updateTelemetryWorkMode(packet.workMode);
    updateTelemetryErrors(telemetryDecoder.GetErrors(packet.error));
    printTelemetryLogFile(packet);
  }
}

void SUA::updateSUATelemetryConsole(TelemetryPacket decodedPacket)
{
  QString text = "Time: " + decodedPacket.time + "\n";
  text += "Base Coordinates: " + QString::number(decodedPacket.latitude) + "   " + QString::number(decodedPacket.longitude) + " Height: " + QString::number(decodedPacket.height) + "\n";
  text += "Status: " + decodedPacket.status + "\n";
  text += "UAV Coordinates: " + QString::number(decodedPacket.uavLatitude) + "   " + QString::number(decodedPacket.uavLongitude) + " Height UAV: " + QString::number(decodedPacket.uavAltitude) + "\n";
  text += "Dir: " + QString::number(decodedPacket.direction) + "\n";
  text += "Enc: " + QString::number(decodedPacket.azimuth) + "   " + QString::number(decodedPacket.zenith) + " T: " + decodedPacket.temperature + "°C\n";
  text += "Calc: " + QString::number(decodedPacket.uavAzimuth) + "   " + QString::number(decodedPacket.uavZenith);
  ui.txbTelemetryWindow->setText(text);
  QScrollBar* scroll = ui.txbTelemetryWindow->verticalScrollBar();
  scroll->setValue(scroll->maximum());
  update();
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
  ui.txbConsoleWindow->append(">>>" + str /*+ "\n"*/);
  QScrollBar* scroll = ui.txbConsoleWindow->verticalScrollBar();
  scroll->setValue(scroll->maximum());
  txbSuaDNA->setText("");
  ui.txbSuaDNAUsers->setText("");
}

void SUA::updateCommSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::MySocketState>();
  lblCommandSocketState->setText(metaEnum.valueToKey(state));
  switch (state)
  {
  case NetworkBase::CONNECTED:
    lblCommandSocketState->setStyleSheet("background-color:rgb(0,255,128);");
    ui.widget->setEnabled(true);
    imageStatus.SetImage(ui.imgConnectState, GREEN);
    break;
  case NetworkBase::ERROR:
  case NetworkBase::NOT_CONNECTED:
    lblCommandSocketState->setStyleSheet("background-color:pink;");
    ui.widget->setEnabled(false);
  default:
    imageStatus.SetImage(ui.imgConnectState, GREY);
    imageStatus.SetImage(ui.imgFollowState, GREY);
    imageStatus.SetImage(ui.imgGPSState, GREY);
    //imageStatus.SetImage(ui.imgMoveState, "GREY");
    imageStatus.SetImage(ui.imgOrientationState, GREY);
    imageStatus.SetImage(ui.imgStopState, GREY);
    break;
  }
}

void SUA::updateModemSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::MySocketState>();
  lblModemSocketState->setText(metaEnum.valueToKey(state));  
  switch (state)
  {
  case NetworkBase::CONNECTED:
    lblModemSocketState->setStyleSheet("background-color:rgb(0,255,128);");
    break;
  case NetworkBase::ERROR:
  case NetworkBase::NOT_CONNECTED:
  default:
    lblModemSocketState->setStyleSheet("background-color:pink;");
    break;
  }
}

void SUA::updateTelemSockLbl(int state)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<NetworkBase::MySocketState>();
  lblTelemetrySocketState->setText(metaEnum.valueToKey(state));
  switch (state)
  {
  case NetworkBase::CONNECTED:
    lblTelemetrySocketState->setStyleSheet("background-color:rgb(0,255,128);");
    break;
  case NetworkBase::ERROR:
  case NetworkBase::NOT_CONNECTED:
  default:
    lblTelemetrySocketState->setStyleSheet("background-color:pink;");
    break;
  }
}

void SUA::reconnectAllSockets()
{
  imageStatus.SetImage(ui.imgConnectState, YELLOW_BLINK);
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
    QDir().mkdir(suaSettings.telemetryLogDir);
    printTelemetryLogFileBegin(suaSettings.telemetryLogDir + QString("/LogFile_%1.txt").arg(timeSystem));
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

    telemetryLogFile.close();
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
  telemetryLogFile.setFileName(path);
  if (telemetryLogFile.open(QFile::WriteOnly))
  {
    telemetryLogFileStream.setDevice(&telemetryLogFile);
    telemetryLogFileStream << "time_date;base_lat;base_long;base_alt;base_status;uav_lat;uav_long;uav_alt;dir;enc_az;enc_zen;temp;calc_az;calc_zen;work;error;distance;height_GPS\n";
    telemetryLogFileStream << "\r\n";
  }
}

void SUA::printTelemetryLogFile(TelemetryPacket packet)
{
  if (telemetryLogFileStream.status() == QTextStream::Ok)
  {
    telemetryLogFileStream << packet.time + ';' + QString::number(packet.latitude) + ';' + QString::number(packet.longitude) + ';' +
                              QString::number(packet.height) + ';' + packet.status + ';' +
                              QString::number(packet.uavLatitude) + ';' + QString::number(packet.uavLongitude) + ';' + QString::number(packet.uavAltitude) + ';' +
                              QString::number(packet.direction) + ';' +
                              QString::number(packet.azimuth) + ';' + QString::number(packet.zenith) + ';' + packet.temperature + ';' +
                              QString::number(packet.uavAzimuth) + ';' + QString::number(packet.uavZenith) + ';' +
                              packet.workMode + ';' + packet.error + ';' + QString::number(packet.distance) + ';' + QString::number(packet.heightGPS);
    telemetryLogFileStream << "\r\n";
  }
}

void SUA::chooseLogDir()
{
  QString tpath = QFileDialog::getExistingDirectory(this, tr("Выберите путь для сохранения файла"), suaSettings.telemetryLogDir);
  if(tpath != "")
  {
    suaSettings.telemetryLogDir = tpath;
    SUASerializer::Serialize(configFilePath, &suaSettings);
    txbLogFile->setText(tpath);
  }
}
