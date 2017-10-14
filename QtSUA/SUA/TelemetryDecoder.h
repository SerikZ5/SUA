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

protected:
  void Synchronize();

  QByteArray inputBytes;
  bool isSynchonized;
};

#endif