#ifndef COMMAND_NETWORK
#define COMMAND_NETWORK

#include "NetworkBase.h"

class CommandNetwok : public NetworkBase
{
public:
  CommandNetwok(QString address, int port);
  virtual ~CommandNetwok();

private:

};

#endif
