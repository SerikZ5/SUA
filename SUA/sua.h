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

private:
    Ui::SUAClass ui;
};

#endif // SUA_H
