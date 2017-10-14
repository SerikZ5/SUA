#ifndef RECIEVED_ARRAY
#define RECIEVED_ARRAY

#include <QByteArray>

class RecievedArray
{
public:
  RecievedArray(QByteArray arr);
  ~RecievedArray();

  QString ToString();
  QString ToHexString();

  QByteArray arr;
};

#endif // RECIEVED_ARRAY

