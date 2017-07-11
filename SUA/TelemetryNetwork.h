#ifndef TELEMETRY_NETWORK
#define TELEMETRY_NETWORK

#include "NetworkBase.h"

class TelemetryNetwork: public NetworkBase
{
public:
  TelemetryNetwork(QString address, int port);
  ~TelemetryNetwork();

private:

};

#endif
