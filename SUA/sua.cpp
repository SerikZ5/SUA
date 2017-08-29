#include "sua.h"
#include "Spoiler.h"

SUA::SUA(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

SUA::~SUA()
{
}

void SUA::addSpoilers()
{
  Spoiler* control = new Spoiler("Управление", 300, this);
  QVBoxLayout* controlSpoilerLayout = new QVBoxLayout(control);
  QPushButton* btnFollowCommand = new QPushButton("Режим слежения", control);
  QPushButton* btnOrientedAntennasCommand = new QPushButton("Ориентация", control);
  QPushButton* btnSetZeroPositionCommand = new QPushButton("Установить ноль", control);
  QPushButton* btnStopEnginesCommand = new QPushButton("Стоп", control);
  QPushButton* btnEnableHeatingCommand = new QPushButton("Вкл. подогрев", control);
  QPushButton* btnDisableHeatingCommand = new QPushButton("Выкл. подогрев", control);
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

  QVBoxLayout* vLayout = new QVBoxLayout(this);
  vLayout->addWidget(control);
  ui.scrollArea->setLayout(vLayout);
}
