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
  connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(OnStateChanged(QAbstractSocket::SocketState)));
  connect(clientSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
  connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void NetworkBase::SlotReadyRead()
{

}

void NetworkBase::Send(QByteArray data)
{

}

void NetworkBase::SlotError(QAbstractSocket::SocketError error)
{
  delete clientSocket;
  clientSocket = NULL;
  state = ERROR;
  NetworkStateChanged(state);
}

void NetworkBase::DisconnectFromHost()
{
  clientSocket->disconnectFromHost();
  state = SocketState::DISCONNECTING;
}

void NetworkBase::OnStateChanged(QAbstractSocket::SocketState newState)
{
  switch (newState)
  {
  case QAbstractSocket::SocketState::UnconnectedState:
  case QAbstractSocket::SocketState::HostLookupState:
    state = SocketState::NOT_CONNECTED;
    NetworkStateChanged(state);
    break;
  case QAbstractSocket::SocketState::ConnectingState:
    state = SocketState::CONNECTING;
    NetworkStateChanged(state);
    break;
  case QAbstractSocket::SocketState::ConnectedState:
  case QAbstractSocket::SocketState::BoundState:
  case QAbstractSocket::SocketState::ListeningState:
    state = SocketState::CONNECTED;
    NetworkStateChanged(state);
    break;
  }
}
