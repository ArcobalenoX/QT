/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_4;
    QPushButton *pushButton_9;
    QPushButton *pushButton;
    QPushButton *pushButton_7;
    QPushButton *pushButton_10;
    QLineEdit *lineEdit;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_5;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QPushButton *pushButton_8;
    QPushButton *pushButton_2;
    QPushButton *pushButton_11;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(429, 286);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 211, 161));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 1, 3, 1, 1);

        pushButton_9 = new QPushButton(widget);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));

        gridLayout->addWidget(pushButton_9, 3, 0, 1, 1);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_7 = new QPushButton(widget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        gridLayout->addWidget(pushButton_7, 2, 2, 1, 1);

        pushButton_10 = new QPushButton(widget);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));

        gridLayout->addWidget(pushButton_10, 3, 1, 1, 1);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setLayoutDirection(Qt::LeftToRight);
        lineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lineEdit, 0, 0, 1, 3);

        pushButton_12 = new QPushButton(widget);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));

        gridLayout->addWidget(pushButton_12, 3, 3, 1, 1);

        pushButton_13 = new QPushButton(widget);
        pushButton_13->setObjectName(QString::fromUtf8("pushButton_13"));

        gridLayout->addWidget(pushButton_13, 0, 3, 1, 1);

        pushButton_5 = new QPushButton(widget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        gridLayout->addWidget(pushButton_5, 2, 0, 1, 1);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 1, 2, 1, 1);

        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        gridLayout->addWidget(pushButton_6, 2, 1, 1, 1);

        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        gridLayout->addWidget(pushButton_8, 2, 3, 1, 1);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 1, 1, 1);

        pushButton_11 = new QPushButton(widget);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));

        gridLayout->addWidget(pushButton_11, 3, 2, 1, 1);

        pushButton_14 = new QPushButton(widget);
        pushButton_14->setObjectName(QString::fromUtf8("pushButton_14"));

        gridLayout->addWidget(pushButton_14, 1, 4, 1, 1);

        pushButton_15 = new QPushButton(widget);
        pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));

        gridLayout->addWidget(pushButton_15, 2, 4, 1, 1);

        pushButton_16 = new QPushButton(widget);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));

        gridLayout->addWidget(pushButton_16, 3, 4, 1, 1);

        pushButton_17 = new QPushButton(widget);
        pushButton_17->setObjectName(QString::fromUtf8("pushButton_17"));

        gridLayout->addWidget(pushButton_17, 0, 4, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 429, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_13, SIGNAL(clicked()), lineEdit, SLOT(clear()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Calculate", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "123", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "\342\206\220", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "*", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        pushButton_17->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
