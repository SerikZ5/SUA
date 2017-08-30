#include <QTextCodec>

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
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

  Spoiler* control = new Spoiler(QString::fromUtf8("Управление"), 300, this);
  QVBoxLayout* controlSpoilerLayout = new QVBoxLayout(control);
  QPushButton* btnFollowCommand = new QPushButton(trUtf8("Режим слежения"), control);
  QPushButton* btnOrientedAntennasCommand = new QPushButton(trUtf8("Ориентация"), control);
  QPushButton* btnSetZeroPositionCommand = new QPushButton(trUtf8("Установить ноль"), control);
  QPushButton* btnStopEnginesCommand = new QPushButton(trUtf8("Стоп"), control);
  QPushButton* btnEnableHeatingCommand = new QPushButton(trUtf8("Вкл. подогрев"), control);
  QPushButton* btnDisableHeatingCommand = new QPushButton(trUtf8("Выкл. подогрев"), control);
  QPushButton* btnEnableVentilationCommand = new QPushButton("Вкл. охлаждение", control);
  QPushButton* btnDisableVentilationCommand = new QPushButton("Выкл. охлаждение", control);
  QPushButton* btnTestCommand = new QPushButton("Тестовый режим", control);
  QPushButton* btnResetCommand = new QPushButton("Сброс", control);
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

  Spoiler* userControl = new Spoiler("Ручной режим управления", 300, this);
  QVBoxLayout* userControlSpoilerLayout = new QVBoxLayout(userControl);

  QGroupBox* groupAzimuth = new QGroupBox(userControl);
  groupAzimuth->setTitle("Азимут");
  QHBoxLayout* azimuthLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandAzimuth = new QLineEdit(groupAzimuth);
  QPushButton* btnSetAzimuthCommand = new QPushButton("Ввод", groupAzimuth);
  azimuthLayout->addWidget(txbSuaCommandAzimuth);
  azimuthLayout->addWidget(btnSetAzimuthCommand);
  groupAzimuth->setLayout(azimuthLayout);

  QGroupBox* groupElevationAngle = new QGroupBox(userControl);
  groupElevationAngle->setTitle("Угол места");
  QHBoxLayout* ElevationAngleLayout = new QHBoxLayout(groupAzimuth);
  QLineEdit* txbSuaCommandZenith = new QLineEdit(groupAzimuth);
  QPushButton* btnSetZenithCommand = new QPushButton("Ввод", groupAzimuth);
  ElevationAngleLayout->addWidget(txbSuaCommandZenith);
  ElevationAngleLayout->addWidget(btnSetZenithCommand);
  groupElevationAngle->setLayout(ElevationAngleLayout);

  userControlSpoilerLayout->addWidget(groupAzimuth);
  userControlSpoilerLayout->addWidget(groupElevationAngle);
  userControl->setContentLayout(userControlSpoilerLayout);

  QVBoxLayout* vLayout = new QVBoxLayout(this);
  vLayout->addWidget(control);
  vLayout->addWidget(userControl);
  ui.scrollArea->setLayout(vLayout);
}
