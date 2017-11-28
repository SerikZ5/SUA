#include <QPropertyAnimation>

#include "Spoiler.h"

Spoiler::Spoiler(const QString & title, const int animationDuration, QWidget *parent) : QWidget(parent), animationDuration(animationDuration) 
{  
  this->animationDuration = animationDuration;
  QFrame* headerLine = new QFrame(this);
  headerLine->setFrameShape(QFrame::HLine);
  headerLine->setFrameShadow(QFrame::Sunken);
  headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  toggleButton.setStyleSheet("QToolButton { border: none; }");
  toggleButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toggleButton.setArrowType(Qt::RightArrow);
  toggleButton.setText(title);
  toggleButton.setCheckable(true);
  toggleButton.setChecked(false);

  contentArea.setStyleSheet("QScrollArea { background-color:white; border: none; }");
  contentArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  contentArea.setWidgetResizable(true);
  // start out collapsed
  contentArea.setMaximumHeight(0);
  contentArea.setMinimumHeight(0);
  // let the entire widget grow and shrink with its content
  toggleAnimation.addAnimation(new QPropertyAnimation(this, "minimumHeight"));
  toggleAnimation.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
  toggleAnimation.addAnimation(new QPropertyAnimation(&contentArea, "maximumHeight"));
  // don't waste space

  QGridLayout* mainLayout = new QGridLayout(this);
  mainLayout->setVerticalSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  int row = 0;
  mainLayout->addWidget(&toggleButton, row, 0, 1, 1, Qt::AlignLeft);
  mainLayout->addWidget(headerLine, row++, 2, 1, 1);
  mainLayout->addWidget(&contentArea, row, 0, 1, 3);
  setLayout(mainLayout);
  connect(&toggleButton, SIGNAL(clicked(bool)), this, SLOT(animation(bool)));
}

void Spoiler::animation(bool checked)
{
    toggleButton.setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
    toggleAnimation.setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation.start();
}

void Spoiler::SetTitle(const QString &title)
{
  toggleButton.setText(title);
}

void Spoiler::setContentLayout(QLayout* content) 
{
  delete contentArea.layout();
  contentArea.setLayout(content);
  const int collapsedHeight = sizeHint().height() - contentArea.maximumHeight();
  int contentHeight = content->sizeHint().height();
  countAnimation(collapsedHeight, contentHeight);
}

void Spoiler::setContentWidget(QWidget* content)
{
  delete contentArea.layout();
  contentArea.setWidget(content);
  const int collapsedHeight = sizeHint().height() - contentArea.maximumHeight();
  int contentHeight = content->sizeHint().height();
  countAnimation(collapsedHeight, contentHeight);
}

void Spoiler::countAnimation(int collapsedHeight, int contentHeight)
{
  for (int i = 0; i < toggleAnimation.animationCount() - 1; ++i)
  {
    QPropertyAnimation * spoilerAnimation = static_cast<QPropertyAnimation *>(toggleAnimation.animationAt(i));
    spoilerAnimation->setDuration(animationDuration);
    spoilerAnimation->setStartValue(collapsedHeight);
    spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
  }
  QPropertyAnimation * contentAnimation = static_cast<QPropertyAnimation *>(toggleAnimation.animationAt(toggleAnimation.animationCount() - 1));
  contentAnimation->setDuration(animationDuration);
  contentAnimation->setStartValue(0);
  contentAnimation->setEndValue(contentHeight);
}
