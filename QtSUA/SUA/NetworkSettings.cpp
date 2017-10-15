#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "NetworkSettings.h"
#include "SUASerializer.h"


NetworkSettings::NetworkSettings(SUASettings* settings, QString configFilePath, QWidget* parent) : QDialog(parent)
{
  this->settings = settings;
  this->configFilePath = configFilePath;
  setWindowTitle(tr("Настройки сети"));
  QGridLayout* layout = new QGridLayout();

  QLabel* host = new QLabel(tr("IP-адрес"), this);
  layout->addWidget(host, 0, 0, 1, 1);
  txtHostAddress = new QLineEdit(this);
  txtHostAddress->setText(settings->hostAddress);
  layout->addWidget(txtHostAddress, 0, 1, 1, 1);

  QLabel* modem = new QLabel(tr("Порт модема"), this);
  layout->addWidget(modem, 1, 0, 1, 1);
  txtModemPort = new QLineEdit(this);
  txtModemPort->setText(QString::number(settings->modemPort));
  layout->addWidget(txtModemPort, 1, 1, 1, 1);

  QLabel* telemetry = new QLabel(tr("Порт телеметрии"), this);
  layout->addWidget(telemetry, 2, 0, 1, 1);
  txtTelemetryPort = new QLineEdit(this);
  txtTelemetryPort->setText(QString::number(settings->telemetryPort));
  layout->addWidget(txtTelemetryPort, 2, 1, 1, 1);

  QLabel* command = new QLabel(tr("Порт управления"), this);
  layout->addWidget(command, 3, 0, 1, 1);
  txtCommandPort = new QLineEdit(this);
  txtCommandPort->setText(QString::number(settings->commandPort));
  layout->addWidget(txtCommandPort, 3, 1, 1, 1);

  QPushButton* ok = new QPushButton(tr("Применить"), this);
  connect(ok, SIGNAL(clicked()), this, SLOT(buttonOk_click()));
  layout->addWidget(ok, 4, 0, 1, 1);
  QPushButton* cancel = new QPushButton(tr("Отмена"), this);
  connect(cancel, SIGNAL(clicked()), this, SLOT(buttonCancel_click()));
  layout->addWidget(cancel, 4, 1, 1, 1);
  setLayout(layout);
}

NetworkSettings::~NetworkSettings()
{}

void NetworkSettings::buttonOk_click()
{
  try
  {
    settings->hostAddress = txtHostAddress->text();
    bool ok = true;
    int value = txtCommandPort->text().toInt(&ok);
    if (ok)
      settings->commandPort = value;
    else
      throw QString(tr("Некорректное значение!"));
    
    value = txtModemPort->text().toInt(&ok);
    if (ok)
      settings->modemPort = value;
    else
      throw QString(tr("Некорректное значение!"));

    value = txtTelemetryPort->text().toInt(&ok);
    if (ok)
      settings->telemetryPort = value;
    else
      throw QString(tr("Некорректное значение!"));
    
    SUASerializer::Serialize(configFilePath, settings);
    done(1);
  }
  catch (QString s)
  {
    QMessageBox::warning(this, "Предупреждение!", s);
  }
}

void NetworkSettings::buttonCancel_click()
{
  done(0);
}
