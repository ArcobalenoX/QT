#include "mainwindow.h"

using namespace Qt;

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);

    msg.exec();
}

int MainWindow::showQueryMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Query");
    msg.setText(message);
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    return msg.exec();
}

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog fd(this);
    QStringList filters;
    QMap<QString, QString> map;
    const char* filterArray[][2] =
    {
        {"Text Files (*.txt)", ".txt"},
        {"All Files (*)",      "*"   },
        {NULL,                 NULL  }
    };

    for(int i=0; filterArray[i][0]!=NULL; i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0], filterArray[i][1]);
    }

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);
    fd.setNameFilters(filters);

    if( mode == QFileDialog::AcceptOpen )
    {
        fd.setFileMode(QFileDialog::ExistingFile);
    }

    if( fd.exec() == QFileDialog::Accepted )
    {
        ret = fd.selectedFiles()[0];

        if( mode == QFileDialog::AcceptSave )
        {
            QString postfix = map[fd.selectedNameFilter()];

            if( (postfix != "*") && !ret.endsWith(postfix) )
            {
                ret = ret + postfix;
            }
        }
    }
    return ret;
}

void MainWindow::openFileToEditor(QString path)
{
    if( path != "" )
    {
        QFile file(path);

        if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            mainEditor.setPlainText(QString(file.readAll()));

            file.close();

            m_filePath = path;

            m_isTextChanged = false;

            setWindowTitle("NotePad - [ " + m_filePath + " ]");
        }
        else
        {
            showErrorMessage(QString("Open file error! \n\n") + "\"" + path + "\"");
        }
    }
}

void MainWindow::onFileOpen()
{
    preEditorChange();

    if( !m_isTextChanged )
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen, "Open");

        openFileToEditor(path);
    }
}

void MainWindow::openFile(QString path)
{
    preEditorChange();
    if(!m_isTextChanged)
    {
        openFileToEditor(path);
    }

}


QString MainWindow::saveCurrentData(QString path)
{
    QString ret = path;

    if( ret == "" )
    {
        ret = showFileDialog(QFileDialog::AcceptSave, "Save");
    }

    if( ret != "" )
    {
        QFile file(ret);

        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);

            out << mainEditor.toPlainText();

            file.close();

            setWindowTitle("NotePad - [ " + ret + " ]");

            m_isTextChanged = false;
        }
        else
        {
            showErrorMessage(QString("Save file error! \n\n") + "\"" + ret + "\"");

            ret = "";
        }
    }

    return ret;
}
void MainWindow::onFileSave()
{
    if( m_filePath == "" )
    {
        m_filePath = showFileDialog(QFileDialog::AcceptSave, "Save");
    }

    if( m_filePath != "" )
    {
        QFile file(m_filePath);

        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);

            out << mainEditor.toPlainText();

            file.close();

            setWindowTitle("NotePad - [ " + m_filePath + " ]");
        }
        else
        {
            showErrorMessage(QString("Save file error! \n\n") + "\"" + m_filePath + "\"");

            m_filePath = "";
        }
    }

}

void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData();

    if( path != "" )
    {
        m_filePath = path;
    }
}
void MainWindow:: onFileExit()
{
    close();
}


void MainWindow::onTextChanged()
{
    if( !m_isTextChanged )
    {
        setWindowTitle("*" + windowTitle());
    }

    m_isTextChanged = true;
}



void MainWindow::preEditorChange()
{
    if( m_isTextChanged )
    {
        int r = showQueryMessage("Do you want to save the changes to file?");
        switch(r)
        {
        case QMessageBox::Yes:
            saveCurrentData(m_filePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}


void MainWindow::onFileNew()
{
    preEditorChange();

    if( !m_isTextChanged )
    {
        mainEditor.clear();

        setWindowTitle("NotePad - [ New ]");

        m_filePath = "";

        m_isTextChanged = false;
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    preEditorChange();
    if ( !m_isTextChanged)
    {
        QFont font = mainEditor.font();
        bool isWrap = (mainEditor.lineWrapMode() == QPlainTextEdit::WidgetWidth);
        bool tbVisible = toolBar()->isVisible();
        bool sbVisible = statusBar()->isVisible();
        appconfig config(font, pos(), size(), isWrap, tbVisible, sbVisible);
        config.store();

        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }


}


void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent* e)
{
    if (e->mimeData()->hasUrls())
    {
        preEditorChange();
        if( !m_isTextChanged )
        {
            QList<QUrl> list = e->mimeData()->urls();
            QString path = list[0].toLocalFile();
            QFileInfo fi(path);
            if( fi.isFile())
            {
                openFileToEditor(path);
            }
        }
        else
        {
            showErrorMessage("connot open a dir!");
        }

    }

    else
    {
        e->ignore();
    }
}


void MainWindow:: onFilePrint()
{
    QPrintDialog printdialog(this) ;
    if( printdialog.exec() == QDialog::Accepted)
    {
        //QPrinter* pr = printdialog.printer();
        //mainEditor.document()->print( dynamic_cast<QPagedPaintDevice*>(pr) );
    }
}

void MainWindow:: onCopyAvailable(bool available)
{
    qDebug()<<"onCopyAvailable";
    findMenuBarAction("Copy")->setEnabled(available);
    findMenuBarAction("Cut")->setEnabled(available);
    findToolBarAction("Copy")->setEnabled(available);
    findToolBarAction("Cut")->setEnabled(available);
}
void MainWindow:: onRedoAvailable(bool available)
{
    qDebug()<<"onRedoAvailable";
    findMenuBarAction("Redo")->setEnabled(available);
    findToolBarAction("Redo")->setEnabled(available);
}
void MainWindow:: onUndoAvailable(bool available)
{
    qDebug()<<"onUndoAvailable";
    findMenuBarAction("Undo")->setEnabled(available);
    findToolBarAction("Undo")->setEnabled(available);
}
void MainWindow:: onCursorPositionChanged()
{
    int pos=mainEditor.textCursor().position();
    QString text = mainEditor.toPlainText();
    int col=0,ln=0,flag=-1;
    for(int i=0;i<pos;i++)
    {
        if (text[i]=='\n')
        {
            ln++;
            flag = i;
        }
    }
    flag++;
    col = pos-flag;
    statusLabel.setText("ln:  "+QString::number(ln+1)+ "col  "+QString::number(col+1));
}
void MainWindow::onEditDelete()
{
    QKeyEvent deletePress(QEvent::KeyPress, Key_Delete, NoModifier );
    QKeyEvent deleteRelease(QEvent::KeyRelease, Key_Delete, NoModifier);
    QApplication::sendEvent(&mainEditor, &deletePress);
    QApplication::sendEvent(&mainEditor, &deleteRelease);

}

void MainWindow:: onEditFind()
{
    m_pFindDialog->show();

}
void MainWindow:: onEditReplace()
{
    m_pReplaceDialog->show();
}

void MainWindow:: onEditGoto()
{
    bool ok = false;
    int line = QInputDialog::getInt(this,"goto","line",1,1,mainEditor.document()->lineCount(),1,&ok, WindowCloseButtonHint|Drawer);
    if(ok)
    {
        QString text = mainEditor.toPlainText();
        QTextCursor c = mainEditor.textCursor();
        int pos=0,next=-1;
        for(int i=0;i<line;i++)
        {
            pos = next+1;
            next = text.indexOf('\n',pos);
        }
        c.setPosition(pos);
        mainEditor.setTextCursor(c);
    }

}
void MainWindow:: onFormatWrap()
{
    QPlainTextEdit::LineWrapMode mode = mainEditor.lineWrapMode();
    if(mode == QPlainTextEdit::NoWrap)
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::WidgetWidth);
        findMenuBarAction("Auto Wrap")->setChecked(true);
        findToolBarAction("Auto Wrap")->setChecked(true);
    }
    else
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);
        findMenuBarAction("Auto Wrap")->setChecked(false);
        findToolBarAction("Auto Wrap")->setChecked(false);
    }
}
void MainWindow:: onFormatFont()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok, mainEditor.font(), this);
    if(ok)
    {
        mainEditor.setFont(font);
    }
}
void MainWindow:: onViewToolBar()
{
    QToolBar* tb = toolBar();
    bool visible = tb->isVisible();
    tb->setVisible(!visible);
    findMenuBarAction("Tool Bar")->setChecked(!visible);
    findToolBarAction("Tool Bar")->setChecked(!visible);
}
void MainWindow:: onViewStatusBar()
{
    QStatusBar* sb = statusBar();
    bool visible = sb->isVisible();
    sb->setVisible(!visible);
    findMenuBarAction("Status Bar")->setVisible(!visible);
    findToolBarAction("Status Bar")->setVisible(!visible);

}
void MainWindow:: onHelpManual()
{
    QDesktopServices::openUrl(QUrl("http://www.dt4sw.com"));
}
void MainWindow:: onHelpAbout()
{

}

