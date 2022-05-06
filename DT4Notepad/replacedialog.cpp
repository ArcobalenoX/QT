#include "replacedialog.h"
#include "ui_replacedialog.h"

replaceDialog::replaceDialog(QWidget *parent, QPlainTextEdit* pText) :
   QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer),
    ui(new Ui::replaceDialog)
{
    ui->setupUi(this);
    connect(ui->findbtn, SIGNAL(clicked()), this, SLOT(onFindClicked()));
    connect(ui->clsoebtn, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
    connect(ui->replacebtn, SIGNAL(clicked()), this, SLOT(onReplaceClicked()));
    connect(ui->replaceallbtn, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));
    setWindowTitle("Replace");
    setPlainTextEdit(pText);
}



replaceDialog::~replaceDialog()
{
    delete ui;
}
void replaceDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;
}
QPlainTextEdit* replaceDialog::getPlainTextEdit()
{
    return m_pText;
}

bool replaceDialog::event(QEvent* evt)
{
    if( evt->type() == QEvent::Close )
    {
        hide();

        return true;
    }

    return QDialog::event(evt);
}

void replaceDialog::onFindClicked()
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
void replaceDialog::onCloseClicked()
{
    close();
}


void replaceDialog:: onReplaceClicked()
{
    QString target = ui->lineEdit->text();
    QString to = ui->replaceedit->text();
    if(m_pText && target!="" && to!="")
    {
        QString selText = m_pText->textCursor().selectedText();
        if(selText == target)
        {
            m_pText->insertPlainText(to);
        }
        onFindClicked();
    }
}
void replaceDialog:: onReplaceAllClicked()
{
    QString target = ui->lineEdit->text();
    QString to = ui->replaceedit->text();
    if(m_pText && target!="" && to!="")
    {
        QString text = m_pText->toPlainText();
        text.replace(target,to,ui->matchcase->isChecked()?Qt::CaseSensitive : Qt::CaseInsensitive);
        m_pText->clear();
        m_pText->insertPlainText(text);
    }

}

