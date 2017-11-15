#include <QFile>
#include <QTextStream>

#include "SUASerializer.h"

bool SUASerializer::Serialize(QString path, SUASettings* settings)
{
  QDomDocument document; 
  QDomElement root = document.createElement("Sua");
  root.appendChild(createElement(document, "AzimuthAcceleraion", QString::number(settings->azimuthAcceleraion)));
  root.appendChild(createElement(document, "AzimuthSpeed", QString::number(settings->azimuthSpeed)));
  root.appendChild(createElement(document, "AzimuthBraking", QString::number(settings->azimuthBraking)));
  root.appendChild(createElement(document, "ZenithAcceleraion", QString::number(settings->zenithAcceleraion)));
  root.appendChild(createElement(document, "ZenithSpeed", QString::number(settings->zenithSpeed)));
  root.appendChild(createElement(document, "ZenithBraking", QString::number(settings->zenithBraking)));
  root.appendChild(createElement(document, "AzimuthSlowSpeed", QString::number(settings->azimuthSlowSpeed)));
  root.appendChild(createElement(document, "AzimuthZeroSeek", QString::number(settings->azimuthZeroSeek)));
  root.appendChild(createElement(document, "ZenithSlowSpeed", QString::number(settings->zenithSlowSpeed)));
  root.appendChild(createElement(document, "ZenithZeroSeek", QString::number(settings->zenithZeroSeek)));
  root.appendChild(createElement(document, "TempEnHeating", QString::number(settings->tempEnHeating)));
  root.appendChild(createElement(document, "TempDisHeating", QString::number(settings->tempDisHeating)));
  root.appendChild(createElement(document, "TempEnVentilation", QString::number(settings->tempEnVentilation)));
  root.appendChild(createElement(document, "TempDisVentilation", QString::number(settings->tempDisVentilation)));
  root.appendChild(createElement(document, "AzimuthDNA", QString::number(settings->azimuthDNA)));
  root.appendChild(createElement(document, "ZenithDNA", QString::number(settings->zenithDNA)));
  root.appendChild(createElement(document, "HostAddress", settings->hostAddress));
  root.appendChild(createElement(document, "ModemPort", QString::number(settings->modemPort)));
  root.appendChild(createElement(document, "CommandPort", QString::number(settings->commandPort)));
  root.appendChild(createElement(document, "TelemetryPort", QString::number(settings->telemetryPort)));
  root.appendChild(createElement(document, "Language", QString(settings->language)));
  root.appendChild(createElement(document, "LogDir", QString(settings->telemetryLogDir)));
  document.appendChild(root);
  // Writing to a file
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return false;
  }
  else
  {
    QTextStream stream(&file);
    stream << document.toString();
    file.close();
  }
  return true;
}

QDomElement SUASerializer::createElement(QDomDocument document, QString name, QString value)
{
  QDomElement element = document.createElement(name); 
  QDomText txt = document.createTextNode(value);
  element.appendChild(txt);
  return element;
}

bool SUASerializer::Deserialize(QString path, SUASettings* settings)
{
  QDomDocument document;
  // Open a file for reading
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }
  else
  {
    // loading
    if (!document.setContent(&file))
    {
      return false;
    }
    file.close();
  }
  QDomElement root = document.firstChildElement();
  if (root.isNull())
    return false;

  settings->azimuthAcceleraion = parseElemet(root, "AzimuthAcceleraion").toInt();
  settings->azimuthSpeed = parseElemet(root, "AzimuthSpeed").toInt();
  settings->azimuthBraking = parseElemet(root, "AzimuthBraking").toInt();
  settings->zenithAcceleraion = parseElemet(root, "ZenithAcceleraion").toInt();
  settings->zenithSpeed = parseElemet(root, "ZenithSpeed").toInt();
  settings->zenithBraking = parseElemet(root, "ZenithBraking").toInt();
  settings->azimuthSlowSpeed = parseElemet(root, "AzimuthSlowSpeed").toInt();
  settings->azimuthZeroSeek = parseElemet(root, "AzimuthZeroSeek").toInt();
  settings->zenithSlowSpeed = parseElemet(root, "ZenithSlowSpeed").toInt();
  settings->zenithZeroSeek = parseElemet(root, "ZenithZeroSeek").toInt();
  settings->tempEnHeating = parseElemet(root, "TempEnHeating").toInt();
  settings->tempDisHeating = parseElemet(root, "TempDisHeating").toInt();
  settings->tempEnVentilation = parseElemet(root, "TempEnVentilation").toInt();
  settings->tempDisVentilation = parseElemet(root, "TempDisVentilation").toInt();
  settings->azimuthDNA = parseElemet(root, "AzimuthDNA").toFloat();
  settings->zenithDNA = parseElemet(root, "ZenithDNA").toFloat();
  settings->hostAddress = parseElemet(root, "HostAddress");
  settings->modemPort = parseElemet(root, "ModemPort").toInt();
  settings->commandPort = parseElemet(root, "CommandPort").toInt();
  settings->telemetryPort = parseElemet(root, "TelemetryPort").toInt();
  settings->telemetryLogDir = parseElemet(root, "LogDir");
  settings->language = (SUASettings::Languages)parseElemet(root, "Language").toInt();

  return true;
}

QString SUASerializer::parseElemet(QDomElement root, QString name)
{
  QDomElement el = root.firstChildElement(name);
  if (!el.isNull())
    return el.text();
  return "";
}
