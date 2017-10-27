#ifndef TELEMETRY_DECODER
#define TELEMETRY_DECODER

#include <QByteArray>

#include "RecievedArray.h"
#include "TelemetryPacket.h"

class TelemetryDecoder
{
public:
  TelemetryDecoder();
  virtual ~TelemetryDecoder();

  void AddBytesToQueue(RecievedArray bytes); 
  int Avaible();
  TelemetryPacket GetTelemetryPacket(bool* ok);

  bool IsVentilationEnable(QString workMode);
  bool IsHeatingEnable(QString workMode);
  QString GetErrors(QString error);

protected:
  void Synchronize();

  QByteArray inputBytes;
  bool isSynchonized;
};

#endif
