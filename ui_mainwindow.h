/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPlainTextEdit *textEditLog;
    QPushButton *buttonStart;
    QPushButton *buttonStop;
    QPushButton *buttonRestart;
    QComboBox *comboBoxLevel;
    QLabel *labelLevel;
    QMenuBar *menuBar;
    QMenu *menuSerwer_fajnej_gry;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(502, 366);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textEditLog = new QPlainTextEdit(centralWidget);
        textEditLog->setObjectName(QStringLiteral("textEditLog"));
        textEditLog->setGeometry(QRect(50, 10, 221, 271));
        textEditLog->setReadOnly(true);
        buttonStart = new QPushButton(centralWidget);
        buttonStart->setObjectName(QStringLiteral("buttonStart"));
        buttonStart->setGeometry(QRect(350, 30, 99, 27));
        buttonStop = new QPushButton(centralWidget);
        buttonStop->setObjectName(QStringLiteral("buttonStop"));
        buttonStop->setGeometry(QRect(350, 70, 99, 27));
        buttonRestart = new QPushButton(centralWidget);
        buttonRestart->setObjectName(QStringLiteral("buttonRestart"));
        buttonRestart->setGeometry(QRect(350, 110, 99, 27));
        comboBoxLevel = new QComboBox(centralWidget);
        comboBoxLevel->setObjectName(QStringLiteral("comboBoxLevel"));
        comboBoxLevel->setGeometry(QRect(350, 220, 101, 27));
        labelLevel = new QLabel(centralWidget);
        labelLevel->setObjectName(QStringLiteral("labelLevel"));
        labelLevel->setGeometry(QRect(370, 200, 67, 17));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 502, 25));
        menuSerwer_fajnej_gry = new QMenu(menuBar);
        menuSerwer_fajnej_gry->setObjectName(QStringLiteral("menuSerwer_fajnej_gry"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSerwer_fajnej_gry->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        buttonStart->setText(QApplication::translate("MainWindow", "Start", 0));
        buttonStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        buttonRestart->setText(QApplication::translate("MainWindow", "Restart", 0));
        labelLevel->setText(QApplication::translate("MainWindow", "Level:", 0));
        menuSerwer_fajnej_gry->setTitle(QApplication::translate("MainWindow", "Serwer fajnej gry", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
