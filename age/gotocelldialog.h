#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H
#include "ui_gotocelldialog.h"
#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
namespace Ui {
class GoToCellDialog;
}

class GoToCellDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToCellDialog(QWidget *parent = nullptr);
    ~GoToCellDialog();

private:
    Ui::GoToCellDialog *ui;
private slots:

    void on_lineEdit_textChanged(const QString &arg1);
};

#endif // GOTOCELLDIALOG_H
