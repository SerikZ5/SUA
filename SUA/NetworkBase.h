#ifndef NETWORK_BASE
#define NETWORK_BASE

#include <QString>
#include <QByteArray>
#include <QTcpSocket>

enum SocketState
{
  NOT_CONNECTED = 0,
  CONNECTED = 1,
  PAUSED = 2,
  CONNECTING = 3,
  DISCONNECTING = 4,
  ERROR = 5
};

enum OperationResult
{
  CONNECTED = 0,
  CONNECTION_FAILED = 1,
  DISCONNECTED = 2,
  DISCONNECTION_FAILED = 3,
  TIME_OUT = 4
};

class NetworkBase: public QObject
{
  Q_OBJECT
public:
  NetworkBase(QString address, int port);
  virtual ~NetworkBase();

  SocketState State() { return state; }

  void ConnectToHost();
  void ConnectToHost(QString hostAddress, int hostPort);

public slots:
  void SlotConnected();
  void SlotReadyRead();
  void SlotError(QAbstractSocket::SocketError);

protected:
  QString address;
  int port;
  QTcpSocket* clientSocket;
  QByteArray recievedData;
  SocketState state;

};

#endif
