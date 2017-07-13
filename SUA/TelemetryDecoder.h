#ifndef TELEMETRY_DECODER
#define TELEMETRY_DECODER

#include <QByteArray>

class TelemetryDecored
{
public:
  TelemetryDecored();
  virtual ~TelemetryDecored();

protected:
  QByteArray inputBytes;
};

#endif