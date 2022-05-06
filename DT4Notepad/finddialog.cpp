#include "finddialog.h"
#include "ui_finddialog.h"

findDialog::findDialog(QWidget *parent, QPlainTextEdit* pText) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    connect(ui->findbtn, SIGNAL(clicked()), this, SLOT(onFindClicked()));
    connect(ui->clsoebtn, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
    setWindowTitle("Find");
    setPlainTextEdit(pText);
}

findDialog::~findDialog()
{
    delete ui;
}
void findDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;
}
QPlainTextEdit* findDialog::getPlainTextEdit()
{
    return m_pText;
}

bool findDialog::event(QEvent* evt)
{
    if( evt->type() == QEvent::Close )
    {
        hide();

        return true;
    }

    return QDialog::event(evt);
}

void findDialog::onFindClicked()
{
    QString target = ui->lineEdit->text();

    if( (m_pText != NULL) && (target != "") )
    {
        QString text = m_pText->toPlainText();
        QTextCursor c = m_pText->textCursor();
        int index = -1;

        if( ui->forward->isChecked() )
        {
            index = text.indexOf(target, c.position(), ui->matchcase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
            if( index >= 0 )
            {
                c.setPosition(index);
                c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
                m_pText->setTextCursor(c);
            }
        }

        if(  ui->backward->isChecked() )
        {
            index = text.lastIndexOf(target, c.position() - text.length() - 1, ui->matchcase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition(index + target.length());
                c.setPosition(index, QTextCursor::KeepAnchor);
                m_pText->setTextCursor(c);
            }
        }

        if( index < 0 )
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Find");
            msg.setText("Can not find \"" + target + "\" any more...");
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
}
void findDialog::onCloseClicked()
{
    close();
}
