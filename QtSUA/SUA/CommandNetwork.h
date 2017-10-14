#ifndef COMMAND_NETWORK
#define COMMAND_NETWORK

#include "NetworkBase.h"

class CommandNetwork : public NetworkBase
{
public:
  CommandNetwork(QString address, int port);
  virtual ~CommandNetwork();

private:

};

#endif
