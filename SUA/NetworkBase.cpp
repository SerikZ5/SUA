#include "NetworkBase.h"

NetworkBase::NetworkBase(QString address, int port)
{
  this->address = address;
  this->port = port;
  this->state = SocketState::NOT_CONNECTED;
  clientSocket = NULL;
}

NetworkBase::~NetworkBase()
{
}

void NetworkBase::ConnectToHost()
{
  ConnectToHost(address, port);
}

void NetworkBase::ConnectToHost(QString hostAddress, int hostPort)
{
  clientSocket = new QTcpSocket(this);
  clientSocket->connectToHost(hostAddress, hostPort);
  connect(clientSocket, SIGNAL(connected()), SLOT(slotConnected()));
  connect(clientSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
  connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void NetworkBase::SlotConnected()
{
  state = SocketState::CONNECTED;
  OnstateChanged(state);
}

void NetworkBase::SlotReadyRead()
{

}

void NetworkBase::SlotError(QAbstractSocket::SocketError error)
{
  delete clientSocket;
  clientSocket = NULL;
  state = SocketState::ERROR;
  OnstateChanged(state);
}
