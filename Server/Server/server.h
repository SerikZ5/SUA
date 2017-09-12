#ifndef _MyServer_h_
#define _MyServer_h_

#include <QWidget>

class QTcpServer;
class QTextEdit;
class QTcpSocket;
class QLineEdit;

class MyServer : public QWidget 
{
  Q_OBJECT
private:
  QLineEdit*  port;
  QTcpServer* m_ptcpServer;
  QTextEdit*  m_ptxt;
  quint16     m_nNextBlockSize;

private:
  void setPort(int port);
  void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
  MyServer(int nPort, QWidget* pwgt = 0);

public slots:
  virtual void slotNewConnection();
  void slotReadClient();
  void changePort();
};
#endif  //_MyServer_h_
