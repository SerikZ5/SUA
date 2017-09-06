#ifndef SUA_H
#define SUA_H

#include <QtWidgets/QMainWindow>
#include "ui_sua.h"

class SUA : public QMainWindow
{
  Q_OBJECT

public:
  SUA(QWidget *parent = 0);
  ~SUA();

protected slots:
  void on_btnFullMode_clicked();
  void on_actionUserMod_triggered(bool checked);

  void on_btnReConnectUsers_clicked();
  void on_btnOrientedAntennasCommandUsers_clicked();
  void on_btnFollowCommandUsers_clicked();
  void on_btnStopEnginesCommandUsers_clicked();



private:
  void addSpoilers();
  void activateFullMode(bool value);
  
  Ui::SUAClass ui;
};

#endif // SUA_H
