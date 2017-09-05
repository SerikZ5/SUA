#include "sua.h"
#include "Spoiler.h"

SUA::SUA(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  addSpoilers();
}

SUA::~SUA()
{

}

void SUA::addSpoilers()
{
  /*Spoiler* control = new Spoiler(QString::fromLocal8Bit("Управление"), 300, this);
  QVBoxLayout* controlSpoilerLayout = new QVBoxLayout(control);
  QPushButton* btnFollowCommand = new QPushButton(QString::fromLocal8Bit("Режим слежения"), control);
  QPushButton* btnOrientedAntennasCommand = new QPushButton(QString::fromLocal8Bit("Ориентация"), control);
  QPushButton* btnSetZeroPositionCommand = new QPushButton(QString::fromLocal8Bit("Установить ноль"), control);
  QPushButton* btnStopEnginesCommand = new QPushButton(QString::fromLocal8Bit("Стоп"), control);
  QPushButton* btnEnableHeatingCommand = new QPushButton(QString::fromLocal8Bit("Вкл. подогрев"), control);
  QPushButton* btnDisableHeatingCommand = new QPushButton(QString::fromLocal8Bit("Выкл. подогрев"), control);
  QPushButton* btnEnableVentilationCommand = new QPushButton(QString::fromLocal8Bit("Вкл. охлаждение"), control);
  QPushButton* btnDisableVentilationCommand = new QPushButton(QString::fromLocal8Bit("Выкл. охлаждение"), control);
  QPushButton* btnTestCommand = new QPushButton(QString::fromLocal8Bit("Тестовый режим"), control);
  QPushButton* btnResetCommand = new QPushButton(QString::fromLocal8Bit("Сброс"), control);
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



  Spoiler* userControl = new Spoiler(QString::fromLocal8Bit("Ручной режим управления"), 300, this);
  QVBoxLayout* userControlSpoilerLayout = new QVBoxLayout(userControl);

  QGroupBox* groupAzimuth = new QGroupBox(userControl);
  groupAzimuth->setTitle(QString::fromLocal8Bit("Азимут"));
  QHBoxLayout* azimuthLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandAzimuth = new QLineEdit(groupAzimuth);
  QPushButton* btnSetAzimuthCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), groupAzimuth);
  azimuthLayout->addWidget(txbSuaCommandAzimuth);
  azimuthLayout->addWidget(btnSetAzimuthCommand);
  groupAzimuth->setLayout(azimuthLayout);
  QGroupBox* groupElevationAngle = new QGroupBox(userControl);
  groupElevationAngle->setTitle(QString::fromLocal8Bit("Угол места"));
  QHBoxLayout* ElevationAngleLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandZenith = new QLineEdit(groupAzimuth);
  QPushButton* btnSetZenithCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), groupAzimuth);
  ElevationAngleLayout->addWidget(txbSuaCommandZenith);
  ElevationAngleLayout->addWidget(btnSetZenithCommand);
  groupElevationAngle->setLayout(ElevationAngleLayout);

  userControlSpoilerLayout->addWidget(groupAzimuth);
  userControlSpoilerLayout->addWidget(groupElevationAngle);
  userControl->setContentLayout(userControlSpoilerLayout);


  Spoiler* options = new Spoiler(QString::fromLocal8Bit("Опции"), 300, this);*/


  QWidget* w = new QWidget(ui.tabWidget);
  Spoiler* adjustment = new Spoiler(QString::fromLocal8Bit("Корректировка"), 300, w);
  QVBoxLayout* adjustmentSpoilerLayout = new QVBoxLayout();
  adjustmentSpoilerLayout->setSpacing(6);
  QHBoxLayout* h1 = new QHBoxLayout();
  h1->setSpacing(6);
  QPushButton* btnSetDNAUp = new QPushButton(QString::fromLocal8Bit("f"),adjustment);
  h1->addWidget(btnSetDNAUp);
  adjustmentSpoilerLayout->addLayout(h1);
  QHBoxLayout* h2 = new QHBoxLayout();
  h2->setSpacing(6);
  QPushButton* btnSetDNALeft = new QPushButton(QString::fromLocal8Bit("f"), adjustment);
  h2->addWidget(btnSetDNALeft);
  QPushButton* txbSuaDNA = new QPushButton(QString::fromLocal8Bit("f"), adjustment);
  h2->addWidget(txbSuaDNA);
  QPushButton* btnSetDNARight = new QPushButton(QString::fromLocal8Bit("f"), adjustment);
  h2->addWidget(btnSetDNARight);
  adjustmentSpoilerLayout->addLayout(h2);
  QHBoxLayout* h3 = new QHBoxLayout();
  h3->setSpacing(6);
  QPushButton* btnSetDNADown = new QPushButton(QString::fromLocal8Bit("f"), adjustment);
  h3->addWidget(btnSetDNADown);
  adjustmentSpoilerLayout->addLayout(h3);
  adjustment->setContentLayout(adjustmentSpoilerLayout);



  Spoiler* azimuth = new Spoiler(QString::fromLocal8Bit("Азимут"), 300, w);
  QVBoxLayout* azimuthSpoilerLayout = new QVBoxLayout(azimuth);
  QGroupBox* motionAzimuth = new QGroupBox(QString::fromLocal8Bit("Движение"), azimuth);
  QVBoxLayout* motionAzimuthLayout = new QVBoxLayout();
  QLabel* l1 = new QLabel(QString::fromLocal8Bit("Ускорение"), motionAzimuth);
  QLineEdit* txbAzimuthAcceleraion = new QLineEdit("15", motionAzimuth);
  QLabel* l2 = new QLabel(QString::fromLocal8Bit("Постоянная скорость"), motionAzimuth);
  QLineEdit* txbAzimuthSpeed = new QLineEdit("150", motionAzimuth);
  QLabel* l3 = new QLabel(QString::fromLocal8Bit("Торможение"), motionAzimuth);
  QLineEdit* txbAzimuthBraking = new QLineEdit("20", motionAzimuth);
  QPushButton* btnSetAzimuthMovOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), motionAzimuth);
  motionAzimuthLayout->addWidget(l1);
  motionAzimuthLayout->addWidget(txbAzimuthAcceleraion);
  motionAzimuthLayout->addWidget(l2);
  motionAzimuthLayout->addWidget(txbAzimuthSpeed);
  motionAzimuthLayout->addWidget(l3);
  motionAzimuthLayout->addWidget(txbAzimuthBraking);
  motionAzimuthLayout->addWidget(btnSetAzimuthMovOptCommand);
  motionAzimuth->setLayout(motionAzimuthLayout);

  QGroupBox* additionallyAzimuth = new QGroupBox(QString::fromLocal8Bit("Движение"), azimuth);
  QVBoxLayout* additionallyAzimuthLayout = new QVBoxLayout();
  QLabel* l4 = new QLabel(QString::fromLocal8Bit("Скорость поиска нуля"), additionallyAzimuth);
  QLineEdit* txbAzimuthSlowSpeed = new QLineEdit("20", additionallyAzimuth);
  QLabel* l5 = new QLabel(QString::fromLocal8Bit("Скорость подхода к точке"), additionallyAzimuth);
  QLineEdit* txbAzimuthZeroSeek = new QLineEdit("50", additionallyAzimuth);
  QPushButton* btnSetAzimuthZerSOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), additionallyAzimuth);
  additionallyAzimuthLayout->addWidget(l4);
  additionallyAzimuthLayout->addWidget(txbAzimuthSlowSpeed);
  additionallyAzimuthLayout->addWidget(l5);
  additionallyAzimuthLayout->addWidget(txbAzimuthZeroSeek);
  additionallyAzimuthLayout->addWidget(btnSetAzimuthZerSOptCommand);
  additionallyAzimuth->setLayout(additionallyAzimuthLayout);

  azimuthSpoilerLayout->addWidget(motionAzimuth);
  azimuthSpoilerLayout->addWidget(additionallyAzimuth);
  azimuth->setContentLayout(azimuthSpoilerLayout);



  Spoiler* zenith = new Spoiler(QString::fromLocal8Bit("Угол места"), 300, w);
  QVBoxLayout* zenithSpoilerLayout = new QVBoxLayout(zenith);
  QGroupBox* motionZenith = new QGroupBox(QString::fromLocal8Bit("Движение"), zenith);
  QVBoxLayout* motionZenithLayout = new QVBoxLayout();
  QLabel* l6 = new QLabel(QString::fromLocal8Bit("Ускорение"), motionZenith);
  QLineEdit* txbZenithAcceleraion = new QLineEdit("15", motionZenith);
  QLabel* l7 = new QLabel(QString::fromLocal8Bit("Постоянная скорость"), motionZenith);
  QLineEdit* txbZenithSpeed = new QLineEdit("150", motionZenith);
  QLabel* l8 = new QLabel(QString::fromLocal8Bit("Торможение"), motionZenith);
  QLineEdit* txbZenithBraking = new QLineEdit("20", motionZenith);
  QPushButton* btnSetZenithMovOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), motionZenith);
  motionZenithLayout->addWidget(l6);
  motionZenithLayout->addWidget(txbZenithAcceleraion);
  motionZenithLayout->addWidget(l7);
  motionZenithLayout->addWidget(txbZenithSpeed);
  motionZenithLayout->addWidget(l8);
  motionZenithLayout->addWidget(txbZenithBraking);
  motionZenithLayout->addWidget(btnSetZenithMovOptCommand);
  motionZenith->setLayout(motionZenithLayout);

  QGroupBox* additionallyZenith = new QGroupBox(QString::fromLocal8Bit("Движение"), azimuth);
  QVBoxLayout* additionallyZenithLayout = new QVBoxLayout();
  QLabel* l9 = new QLabel(QString::fromLocal8Bit("Скорость поиска нуля"), additionallyZenith);
  QLineEdit* txbZenithSlowSpeed = new QLineEdit("20", additionallyZenith);
  QLabel* l10 = new QLabel(QString::fromLocal8Bit("Скорость подхода к точке"), additionallyZenith);
  QLineEdit* txbZenithZeroSeek = new QLineEdit("50", additionallyZenith);
  QPushButton* btnSetZenithZerSOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), additionallyZenith);
  additionallyZenithLayout->addWidget(l9);
  additionallyZenithLayout->addWidget(txbZenithSlowSpeed);
  additionallyZenithLayout->addWidget(l10);
  additionallyZenithLayout->addWidget(txbZenithZeroSeek);
  additionallyZenithLayout->addWidget(btnSetZenithZerSOptCommand);
  additionallyZenith->setLayout(additionallyZenithLayout);

  zenithSpoilerLayout->addWidget(motionZenith);
  zenithSpoilerLayout->addWidget(additionallyZenith);
  zenith->setContentLayout(zenithSpoilerLayout);


  Spoiler* heating = new Spoiler(QString::fromLocal8Bit("Подогрев"), 300, w);
  QVBoxLayout* heatingSpoilerLayout = new QVBoxLayout(zenith);
  QLabel* l11 = new QLabel(QString::fromLocal8Bit("Температура включения"), heating);
  QLineEdit* txbTempEnHeating = new QLineEdit("10", heating);
  QLabel* l12 = new QLabel(QString::fromLocal8Bit("Температура выключения"), heating);
  QLineEdit* txbTempDisHeating = new QLineEdit("18", heating);
  QPushButton* btnSetHeatingOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), heating);
  heatingSpoilerLayout->addWidget(l11);
  heatingSpoilerLayout->addWidget(txbTempEnHeating);
  heatingSpoilerLayout->addWidget(l12);
  heatingSpoilerLayout->addWidget(txbTempDisHeating);
  heatingSpoilerLayout->addWidget(btnSetHeatingOptCommand);
  heating->setContentLayout(heatingSpoilerLayout);

  Spoiler* ventilation = new Spoiler(QString::fromLocal8Bit("Подогрев"), 300, w);
  QVBoxLayout* ventilationSpoilerLayout = new QVBoxLayout(zenith);
  QLabel* l13 = new QLabel(QString::fromLocal8Bit("Температура включения"), ventilation);
  QLineEdit* txbTempEnVentilation = new QLineEdit("45", ventilation);
  QLabel* l14 = new QLabel(QString::fromLocal8Bit("Температура выключения"), ventilation);
  QLineEdit* txbTempDisVentilation = new QLineEdit("35", ventilation);
  QPushButton* btnSetVentilationOptCommand = new QPushButton(QString::fromLocal8Bit("Ввод"), ventilation);
  ventilationSpoilerLayout->addWidget(l13);
  ventilationSpoilerLayout->addWidget(txbTempEnVentilation);
  ventilationSpoilerLayout->addWidget(l14);
  ventilationSpoilerLayout->addWidget(txbTempDisVentilation);
  ventilationSpoilerLayout->addWidget(btnSetVentilationOptCommand);
  ventilation->setContentLayout(ventilationSpoilerLayout);


  QVBoxLayout* optionsSpoilerLayout = new QVBoxLayout();
  optionsSpoilerLayout->addWidget(adjustment);
  optionsSpoilerLayout->addWidget(azimuth);
  optionsSpoilerLayout->addWidget(zenith);
  optionsSpoilerLayout->addWidget(heating);
  optionsSpoilerLayout->addWidget(ventilation);
  QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  optionsSpoilerLayout->addItem(verticalSpacer);

  w->setLayout(optionsSpoilerLayout);
  ui.scrollArea->setWidget(w);
}