/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_DIALOG_H
#define UI_SETTING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class Ui_SettingDialog
{
public:
    QGroupBox *gb_fps;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *radio_btn_30;
    QRadioButton *radio_btn_48;
    QRadioButton *radio_btn_60;
    QGroupBox *gb_model_window;
    QPushButton *btn_change_bg_color;
    QPushButton *btn_changed_model;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *cb_open_sound;
    QCheckBox *cb_show_background;
    QCheckBox *cb_hover_background;
    QCheckBox *cb_resize_window;
    QToolButton *btn_bg_color;
    QLabel *preview;
    QGroupBox *gb_other;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *cb_startup;
    QCheckBox *cb_enable_xf;

    void setupUi(QWidget *SettingDialog)
    {
        if (SettingDialog->objectName().isEmpty())
            SettingDialog->setObjectName(QStringLiteral("SettingDialog"));
        SettingDialog->resize(321, 554);
        gb_fps = new QGroupBox(SettingDialog);
        gb_fps->setObjectName(QStringLiteral("gb_fps"));
        gb_fps->setGeometry(QRect(9, 9, 301, 121));
        layoutWidget = new QWidget(gb_fps);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 281, 91));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        radio_btn_30 = new QRadioButton(layoutWidget);
        radio_btn_30->setObjectName(QStringLiteral("radio_btn_30"));

        verticalLayout->addWidget(radio_btn_30);

        radio_btn_48 = new QRadioButton(layoutWidget);
        radio_btn_48->setObjectName(QStringLiteral("radio_btn_48"));

        verticalLayout->addWidget(radio_btn_48);

        radio_btn_60 = new QRadioButton(layoutWidget);
        radio_btn_60->setObjectName(QStringLiteral("radio_btn_60"));

        verticalLayout->addWidget(radio_btn_60);

        gb_model_window = new QGroupBox(SettingDialog);
        gb_model_window->setObjectName(QStringLiteral("gb_model_window"));
        gb_model_window->setGeometry(QRect(10, 140, 301, 311));
        btn_change_bg_color = new QPushButton(gb_model_window);
        btn_change_bg_color->setObjectName(QStringLiteral("btn_change_bg_color"));
        btn_change_bg_color->setGeometry(QRect(50, 150, 241, 31));
        btn_changed_model = new QPushButton(gb_model_window);
        btn_changed_model->setObjectName(QStringLiteral("btn_changed_model"));
        btn_changed_model->setGeometry(QRect(20, 70, 101, 31));
        layoutWidget1 = new QWidget(gb_model_window);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 190, 281, 111));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cb_open_sound = new QCheckBox(layoutWidget1);
        cb_open_sound->setObjectName(QStringLiteral("cb_open_sound"));

        verticalLayout_2->addWidget(cb_open_sound);

        cb_show_background = new QCheckBox(layoutWidget1);
        cb_show_background->setObjectName(QStringLiteral("cb_show_background"));

        verticalLayout_2->addWidget(cb_show_background);

        cb_hover_background = new QCheckBox(layoutWidget1);
        cb_hover_background->setObjectName(QStringLiteral("cb_hover_background"));

        verticalLayout_2->addWidget(cb_hover_background);

        cb_resize_window = new QCheckBox(layoutWidget1);
        cb_resize_window->setObjectName(QStringLiteral("cb_resize_window"));

        verticalLayout_2->addWidget(cb_resize_window);

        btn_bg_color = new QToolButton(gb_model_window);
        btn_bg_color->setObjectName(QStringLiteral("btn_bg_color"));
        btn_bg_color->setGeometry(QRect(10, 150, 31, 31));
        btn_bg_color->setAutoRaise(false);
        preview = new QLabel(gb_model_window);
        preview->setObjectName(QStringLiteral("preview"));
        preview->setGeometry(QRect(160, 20, 121, 121));
        gb_other = new QGroupBox(SettingDialog);
        gb_other->setObjectName(QStringLiteral("gb_other"));
        gb_other->setGeometry(QRect(10, 460, 301, 91));
        layoutWidget2 = new QWidget(gb_other);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 20, 281, 61));
        verticalLayout_4 = new QVBoxLayout(layoutWidget2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        cb_startup = new QCheckBox(layoutWidget2);
        cb_startup->setObjectName(QStringLiteral("cb_startup"));

        verticalLayout_4->addWidget(cb_startup);

        cb_enable_xf = new QCheckBox(layoutWidget2);
        cb_enable_xf->setObjectName(QStringLiteral("cb_enable_xf"));

        verticalLayout_4->addWidget(cb_enable_xf);


        retranslateUi(SettingDialog);

        QMetaObject::connectSlotsByName(SettingDialog);
    } // setupUi

    void retranslateUi(QWidget *SettingDialog)
    {
        SettingDialog->setWindowTitle(QApplication::translate("SettingDialog", "Form", 0));
        gb_fps->setTitle(QApplication::translate("SettingDialog", "\346\250\241\345\236\213 FPS", 0));
        radio_btn_30->setText(QApplication::translate("SettingDialog", "30 FPS", 0));
        radio_btn_48->setText(QApplication::translate("SettingDialog", "48 FPS\357\274\210\346\216\250\350\215\220\344\275\277\347\224\250\357\274\211", 0));
        radio_btn_60->setText(QApplication::translate("SettingDialog", "60 FPS\357\274\210\345\215\240\347\224\250\350\276\203\345\244\232 CPU \350\265\204\346\272\220\357\274\211", 0));
        gb_model_window->setTitle(QApplication::translate("SettingDialog", "\346\250\241\345\236\213\347\252\227\345\217\243\350\256\276\347\275\256", 0));
        btn_change_bg_color->setText(QApplication::translate("SettingDialog", "\346\233\264\346\224\271\350\203\214\346\231\257\351\242\234\350\211\262", 0));
        btn_changed_model->setText(QApplication::translate("SettingDialog", "\345\210\207\346\215\242\346\250\241\345\236\213", 0));
        cb_open_sound->setText(QApplication::translate("SettingDialog", "\345\274\200\345\220\257\346\250\241\345\236\213\345\243\260\351\237\263", 0));
        cb_show_background->setText(QApplication::translate("SettingDialog", "\346\230\276\347\244\272\347\252\227\345\217\243\350\203\214\346\231\257", 0));
        cb_hover_background->setText(QApplication::translate("SettingDialog", "\350\277\233\345\205\245\347\252\227\345\217\243\346\230\276\347\244\272\350\203\214\346\231\257", 0));
        cb_resize_window->setText(QApplication::translate("SettingDialog", "\345\217\257\346\224\271\345\217\230\347\252\227\345\217\243\345\244\247\345\260\217", 0));
        btn_bg_color->setText(QString());
        preview->setText(QString());
        gb_other->setTitle(QApplication::translate("SettingDialog", "\345\205\266\344\273\226\350\256\276\347\275\256", 0));
        cb_startup->setText(QApplication::translate("SettingDialog", "\345\274\200\345\220\257\345\220\257\345\212\250", 0));
        cb_enable_xf->setText(QString::fromLocal8Bit("¿ªÆô×··¬¹¤¾ß"));
    } // retranslateUi

};

#endif // UI_FORM_H
