#ifndef QLOGINDIALOG_H
#define QLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class QLoginDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel UserLabel;
    QLabel PwdLabel;
    QLineEdit UserEdit;
    QLineEdit PwdEdit;
    QPushButton LoginBtn;
    QPushButton CancelBtn;
    QString m_user;
    QString m_pwd;
private slots:
    void LoginBtn_Clicked();
    void CancelBtn_Clicked();
public:
    QLoginDialog(QWidget *parent = 0);
    QString getUser();
    QString getPwd();
    ~QLoginDialog();
};



#endif // QLOGINDIALOG_H
