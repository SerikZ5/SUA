#include <QDataStream>

#include "NetworkBase.h"

NetworkBase::NetworkBase(QString address, int port)
{
  this->address = address;
  this->port = port;
  this->state = SocketState::NOT_CONNECTED;
  clientSocket = NULL;
  m_nNextBlockSize = 0;
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
  delete clientSocket;
  clientSocket = new QTcpSocket(this);
  clientSocket->connectToHost(hostAddress, hostPort);
  connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnStateChanged(QAbstractSocket::SocketState)));
  connect(clientSocket, SIGNAL(readyRead()), this, SLOT(SlotReadyRead()));
  connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
}

void NetworkBase::SlotReadyRead()
{
  QDataStream in(clientSocket);
  in.setVersion(QDataStream::Qt_4_2);
  for (;;) 
  {
    if (!m_nNextBlockSize)
    {
      if (clientSocket->bytesAvailable() < sizeof(quint16))
      {
        break;
      }
      in >> m_nNextBlockSize;
    }

    if (clientSocket->bytesAvailable() < m_nNextBlockSize)
    {
      break;
    }
    QByteArray str;
    in >> str;

    //RecievedArray arr(str);
    //DataRecieved(arr);

    m_nNextBlockSize = 0;
  }
}

void NetworkBase::Send(QByteArray data)
{
  QByteArray arrBlock;
  QDataStream out(&arrBlock, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_2);
  out << quint16(0) << data;

  out.device()->seek(0);
  out << quint16(arrBlock.size() - sizeof(quint16));

  clientSocket->write(arrBlock);
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
