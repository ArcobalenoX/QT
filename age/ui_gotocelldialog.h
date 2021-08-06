/********************************************************************************
** Form generated from reading UI file 'gotocelldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTOCELLDIALOG_H
#define UI_GOTOCELLDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GoToCellDialog
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *okButton;
    QPushButton *canelButton;

    void setupUi(QDialog *GoToCellDialog)
    {
        if (GoToCellDialog->objectName().isEmpty())
            GoToCellDialog->setObjectName(QString::fromUtf8("GoToCellDialog"));
        GoToCellDialog->resize(400, 300);
        widget = new QWidget(GoToCellDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(90, 120, 219, 22));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        widget1 = new QWidget(GoToCellDialog);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(160, 210, 158, 25));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        okButton = new QPushButton(widget1);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_2->addWidget(okButton);

        canelButton = new QPushButton(widget1);
        canelButton->setObjectName(QString::fromUtf8("canelButton"));

        horizontalLayout_2->addWidget(canelButton);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(GoToCellDialog);

        QMetaObject::connectSlotsByName(GoToCellDialog);
    } // setupUi

    void retranslateUi(QDialog *GoToCellDialog)
    {
        GoToCellDialog->setWindowTitle(QCoreApplication::translate("GoToCellDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("GoToCellDialog", "&cell location", nullptr));
        okButton->setText(QCoreApplication::translate("GoToCellDialog", "ok", nullptr));
        canelButton->setText(QCoreApplication::translate("GoToCellDialog", "cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoToCellDialog: public Ui_GoToCellDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTOCELLDIALOG_H
