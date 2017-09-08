#ifndef IMAGE_STATUS_H
#define IMAGE_STATUS_H

#include <QLabel>
#include <QMovie>
#include <QPixmap>

enum SUAImages
{
  GREEN,
  GREY,
  GREEN_BLINK,
  YELLOW_BLINK,
};

class ImageStatus
{
public:
  ImageStatus();
  virtual ~ImageStatus();

  void SetImage(QLabel* label, SUAImages color);

private:
  QMovie* yellowBlink;
  QMovie* greenBlink;
  QPixmap* grey;
  QPixmap* green;
};

#endif
