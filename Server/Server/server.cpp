#include <QMessageBox>
#include <QTcpServer>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>

#include "server.h"

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
, m_nNextBlockSize(0)
{
  m_ptcpServer = new QTcpServer(this);
  setPort(nPort);
  connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
  m_ptxt = new QTextEdit;
  m_ptxt->setReadOnly(true);

  //Layout setup
  QHBoxLayout* portLayout = new QHBoxLayout;
  port = new QLineEdit(QString::number(nPort), this);
  port->setValidator(new QIntValidator());
  QPushButton* button = new QPushButton("change", this);
  portLayout->addWidget(port);
  portLayout->addWidget(button);
  connect(button, SIGNAL(clicked()), this, SLOT(changePort()));
  QVBoxLayout* pvbxLayout = new QVBoxLayout;
  pvbxLayout->addLayout(portLayout);
  pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
  pvbxLayout->addWidget(m_ptxt);
  setLayout(pvbxLayout);
}

void MyServer::setPort(int port)
{
  if (!m_ptcpServer->listen(QHostAddress::Any, port))
  {
    QMessageBox::critical(0,
      "Server Error",
      "Unable to start the server:"
      + m_ptcpServer->errorString());
    m_ptcpServer->close();
    return;
  }
}

void MyServer::changePort()
{
  int i = port->text().toInt();
  setPort(i);
}

/*virtual*/ void MyServer::slotNewConnection()
{
  QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
  connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
  connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
  sendToClient(pClientSocket, "Server Response: Connected!");
}

void MyServer::slotReadClient()
{
  QTcpSocket* pClientSocket = (QTcpSocket*)sender();
  QDataStream in(pClientSocket);
  in.setVersion(QDataStream::Qt_4_2);
  for (;;)
  {
    if (!m_nNextBlockSize)
    {
      if (pClientSocket->bytesAvailable() < sizeof(quint16)) 
      {
        break;
      }
      in >> m_nNextBlockSize;
    }

    if (pClientSocket->bytesAvailable() < m_nNextBlockSize) 
    {
      break;
    }
    QByteArray str;
    in >> str;

    QString strMessage = "Client has sended - " + QString(str);
    m_ptxt->append(strMessage);
    m_nNextBlockSize = 0;
    sendToClient(pClientSocket, str);
  }
}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
  QByteArray  arrBlock;
  QDataStream out(&arrBlock, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_2);
  out << quint16(0) << str.toLatin1();

  out.device()->seek(0);
  out << quint16(arrBlock.size() - sizeof(quint16));

  pSocket->write(arrBlock);
}
