#ifndef TELEMETRY_DECODER
#define TELEMETRY_DECODER

#include <QByteArray>

#include "RecievedArray.h"

class TelemetryPacket;

class TelemetryDecored
{
public:
  TelemetryDecored();
  virtual ~TelemetryDecored();

  void AddBytesToQueue(RecievedArray bytes); 
  int Avaible();
  TelemetryPacket GetTelemetryPacket();

protected:
  void Synchronize();

  QByteArray inputBytes;
  bool isSynchonized;
};

#endif