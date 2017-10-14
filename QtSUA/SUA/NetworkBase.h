#ifndef NETWORK_BASE
#define NETWORK_BASE

#include <QString>
#include <QByteArray>
#include <QTcpSocket>

#include "RecievedArray.h"

class NetworkBase: public QObject
{
  Q_OBJECT
public:
  enum SocketState
  {
    NOT_CONNECTED = 0,
    CONNECTED = 1,
    PAUSED = 2,
    CONNECTING = 3,
    DISCONNECTING = 4,
    ERROR = 5
  };
  Q_ENUM(SocketState)

  NetworkBase(QString address, int port);
  virtual ~NetworkBase();

  SocketState State() { return state; }

  void ConnectToHost();
  void ConnectToHost(QString hostAddress, int hostPort); 
  void DisconnectFromHost();

  void Send(QByteArray data);

public slots:
  void SlotReadyRead();
  void OnStateChanged(QAbstractSocket::SocketState);
  void SlotError(QAbstractSocket::SocketError);

signals:
  void NetworkStateChanged(int);
  void DataRecieved(RecievedArray arr);

protected:
  QString address;
  int port;
  QTcpSocket* clientSocket;
  SocketState state;
  quint16 m_nNextBlockSize;
};

#endif
