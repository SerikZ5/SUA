#include <QDir>

#include "ImageStatus.h"

#define ImageSize 17

ImageStatus::ImageStatus()
{
  green = new QPixmap(":/SUA/images/green.png");
  grey = new QPixmap(":/SUA/images/grey.png");
  greenBlink = new QMovie(":/SUA/images/green_blink.gif");
  yellowBlink = new QMovie(":/SUA/images/yellow_blink.gif");
}

ImageStatus::~ImageStatus()
{
  delete green;
  delete grey;
  delete greenBlink;
  delete yellowBlink;
}

void ImageStatus::SetImage(QLabel* label, SUAImages color)
{
  switch (color)
  {
  case GREEN:
    label->setPixmap(green->scaled(ImageSize, ImageSize, Qt::KeepAspectRatio));
    break;
  case GREY:
    label->setPixmap(grey->scaled(ImageSize, ImageSize, Qt::KeepAspectRatio));
    break;
  case GREEN_BLINK:
    greenBlink->setScaledSize(label->size());
    greenBlink->start();
    label->setMovie(greenBlink);
    break;
  case YELLOW_BLINK:
    yellowBlink->setScaledSize(label->size());
    yellowBlink->start();
    label->setMovie(yellowBlink);
    break;
  default:
    break;
  }
}