#ifndef SPOILER
#define SPOILER

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class Spoiler : public QWidget 
{
  Q_OBJECT

public:
  explicit Spoiler(const QString & title = "", const int animationDuration = 300, QWidget *parent = 0);

  void setContentLayout(QLayout* contentLayout);
  void setContentWidget(QWidget* content);

private:
  void countAnimation(int collapsedHeight, int contentHeight);

  QToolButton toggleButton;
  QScrollArea contentArea;
  QParallelAnimationGroup toggleAnimation;
  int animationDuration{ 300 };
};

#endif