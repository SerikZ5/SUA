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
  sendToClient(pClientSocket, QString("Server Response: Connected!").toLatin1());
}

void MyServer::slotReadClient()
{
  QTcpSocket* pClientSocket = (QTcpSocket*)sender();
  for (;;)
  {
    quint16 m_nNextBlockSize = pClientSocket->bytesAvailable();
    if (m_nNextBlockSize < 1)
    {
      break;
    }
    QByteArray array = pClientSocket->read(m_nNextBlockSize);
    QString strMessage = "Client has sended - " + QString(array);
    m_ptxt->append(strMessage);
    sendToClient(pClientSocket, array);
  }  
}

void MyServer::sendToClient(QTcpSocket* pSocket, QByteArray arrBlock)
{
  pSocket->write(arrBlock);
}
