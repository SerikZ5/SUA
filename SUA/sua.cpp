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
  Spoiler* control = new Spoiler(QString::fromLocal8Bit("Управление"), 300, this);
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



  Spoiler* options = new Spoiler(QString::fromLocal8Bit("Опции"), 300, this);
  QVBoxLayout* optionsSpoilerLayout = new QVBoxLayout(options);

  Spoiler* adjustment = new Spoiler(QString::fromLocal8Bit("Корректировка"), 300, this);
  QVBoxLayout* adjustmentSpoilerLayout = new QVBoxLayout(adjustment);
  adjustmentSpoilerLayout->setSpacing(6);
  adjustmentSpoilerLayout->setContentsMargins(11, 11, 11, 11);
  QHBoxLayout* h1 = new QHBoxLayout();
  h1->setSpacing(6);
  QPushButton* btnSetDNAUp = new QPushButton(adjustment);
  h1->addWidget(btnSetDNAUp);
  adjustmentSpoilerLayout->addLayout(h1);
  QHBoxLayout* h2 = new QHBoxLayout();
  h2->setSpacing(6);
  QPushButton* btnSetDNALeft = new QPushButton(adjustment);
  h2->addWidget(btnSetDNALeft);
  QPushButton* txbSuaDNA = new QPushButton(adjustment);
  h2->addWidget(txbSuaDNA);
  QPushButton* btnSetDNARight = new QPushButton(adjustment);
  h2->addWidget(btnSetDNARight);
  adjustmentSpoilerLayout->addLayout(h2);
  QHBoxLayout* h3 = new QHBoxLayout();
  h3->setSpacing(6);
  QPushButton* btnSetDNADown = new QPushButton(adjustment);
  h3->addWidget(btnSetDNADown);
  adjustmentSpoilerLayout->addLayout(h3);
  adjustment->setContentLayout(adjustmentSpoilerLayout);

  Spoiler* azimuth = new Spoiler(QString::fromLocal8Bit("Азимут"), 300, this);
  QVBoxLayout* azimuthSpoilerLayout = new QVBoxLayout(azimuth);

  optionsSpoilerLayout->addWidget(adjustment);
  options->setContentLayout(optionsSpoilerLayout);

  QVBoxLayout* vLayout = new QVBoxLayout(this);
  vLayout->addWidget(control);
  vLayout->addWidget(userControl);
  ui.scrollArea->setLayout(vLayout);
}
