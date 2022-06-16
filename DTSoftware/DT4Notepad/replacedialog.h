#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QPointer>
#include <QEvent>
#include <QTextCursor>
#include <QMessageBox>
namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

protected:
    QPointer<QPlainTextEdit> m_pText;

protected slots:
    void onFindClicked();
    void onCloseClicked();
    void onReplaceClicked();
    void onReplaceAllClicked();

public:
    explicit replaceDialog(QWidget *parent = nullptr, QPlainTextEdit* pText= nullptr);
    ~replaceDialog();
    void setPlainTextEdit(QPlainTextEdit* pText);
    QPlainTextEdit* getPlainTextEdit();
    bool event(QEvent* evt);
private:
    Ui::replaceDialog *ui;
};

#endif // REPLACEDIALOG_H
