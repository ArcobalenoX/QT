#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QPointer>
#include <QEvent>
#include <QTextCursor>
#include <QMessageBox>
namespace Ui {
class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

protected:
    QPointer<QPlainTextEdit> m_pText;

protected slots:
    void onFindClicked();
    void onCloseClicked();

public:
    explicit findDialog(QWidget *parent = nullptr, QPlainTextEdit* pText = nullptr);
    ~findDialog();
    void setPlainTextEdit(QPlainTextEdit* pText);
    QPlainTextEdit* getPlainTextEdit();
    bool event(QEvent* evt);
private:
    Ui::findDialog *ui;
};

#endif // FINDDIALOG_H
