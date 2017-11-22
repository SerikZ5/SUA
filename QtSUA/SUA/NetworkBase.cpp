#include <QDataStream>

#include "NetworkBase.h"

NetworkBase::NetworkBase(QString address, int port)
{
  this->address = address;
  this->port = port;
  this->state = NOT_CONNECTED;
  clientSocket = NULL;
  m_nNextBlockSize = 0;
}

NetworkBase::~NetworkBase()
{
  if (clientSocket)
    delete clientSocket;
}

void NetworkBase::ConnectToHost()
{
  ConnectToHost(address, port);
}

void NetworkBase::ConnectToHost(QString hostAddress, int hostPort)
{
  delete clientSocket;
  clientSocket = new QTcpSocket(this);
  clientSocket->connectToHost(hostAddress, hostPort);
  connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnStateChanged(QAbstractSocket::SocketState)));
  connect(clientSocket, SIGNAL(readyRead()), this, SLOT(SlotReadyRead()));
  connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
}

void NetworkBase::SlotReadyRead()
{
  for (;;) 
  {
    if (!m_nNextBlockSize)
    {
      m_nNextBlockSize = clientSocket->bytesAvailable();
      if (m_nNextBlockSize < 1)
      {
        break;
      }
    }
    QByteArray array = clientSocket->read(m_nNextBlockSize);
    RecievedArray arr(array);
    DataRecieved(arr);
    m_nNextBlockSize = 0;
  }
}

void NetworkBase::Send(QByteArray data)
{
  clientSocket->write(data);
}

void NetworkBase::SlotError(QAbstractSocket::SocketError error)
{
  state = ERROR;
  NetworkStateChanged(state);
}

void NetworkBase::DisconnectFromHost()
{
  clientSocket->disconnectFromHost();
  state = DISCONNECTING;
}

void NetworkBase::OnStateChanged(QAbstractSocket::SocketState newState)
{
  switch (newState)
  {
  case QAbstractSocket::UnconnectedState:
  case QAbstractSocket::HostLookupState:
    state = NOT_CONNECTED;
    NetworkStateChanged(state);
    break;
  case QAbstractSocket::ConnectingState:
    state = CONNECTING;
    NetworkStateChanged(state);
    break;
  case QAbstractSocket::ConnectedState:
  case QAbstractSocket::BoundState:
  case QAbstractSocket::ListeningState:
    state = CONNECTED;
    NetworkStateChanged(state);
    break;
  }
}
