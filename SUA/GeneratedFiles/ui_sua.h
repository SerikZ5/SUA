/********************************************************************************
** Form generated from reading UI file 'sua.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUA_H
#define UI_SUA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SUAClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SUAClass)
    {
        if (SUAClass->objectName().isEmpty())
            SUAClass->setObjectName(QStringLiteral("SUAClass"));
        SUAClass->resize(600, 400);
        menuBar = new QMenuBar(SUAClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SUAClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SUAClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SUAClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SUAClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SUAClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SUAClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SUAClass->setStatusBar(statusBar);

        retranslateUi(SUAClass);

        QMetaObject::connectSlotsByName(SUAClass);
    } // setupUi

    void retranslateUi(QMainWindow *SUAClass)
    {
        SUAClass->setWindowTitle(QApplication::translate("SUAClass", "SUA", 0));
    } // retranslateUi

};

namespace Ui {
    class SUAClass: public Ui_SUAClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUA_H
