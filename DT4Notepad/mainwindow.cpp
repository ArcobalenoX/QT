#include "mainwindow.h"
MainWindow::MainWindow()
{
    m_pFindDialog = new findDialog(this, &mainEditor);
    m_pReplaceDialog = new replaceDialog(this, &mainEditor);
    setWindowTitle("NotePad - [ New ]");
    m_filePath = "";
    m_isTextChanged = false;
    setAcceptDrops(true);

}

MainWindow* MainWindow:: newInstance()
{
    MainWindow* ret = new MainWindow();
    if( (ret==NULL) || !ret->construct())
    {
        delete ret;
        ret = NULL;
    }
    return ret;
}

bool MainWindow:: construct()
{
    bool ret = true;
    appconfig config;
    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEditor();
    if(!config.isValid())
    {
        if(!config.isAutoWrap())
        {
            mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);
            findMenuBarAction("Auto Wrap")->setChecked(false);
            findToolBarAction("Auto Wrap")->setChecked(false);
        }
        if(!config.isToolBarVisible())
        {
            toolBar()->setVisible(false);
            findMenuBarAction("Tool Bar")->setChecked(false);
            findToolBarAction("Tool Bar")->setChecked(false);
        }
        if(!config.isStatusBarVisible())
        {
            statusBar()->setVisible(false);
            findMenuBarAction("Status Bar")->setChecked(false);
            findToolBarAction("Status Bar")->setChecked(false);
        }
        move(config.mainWindowPoint());
        resize(config.mainWindowSize());
    }

    return ret;
}

bool MainWindow::initMenuBar()
{
    bool ret = true;
    QMenuBar* mb = menuBar();
    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initHelpMenu(mb);
    return ret;
}

bool MainWindow::initToolBar()
{
    bool ret = true;
    QToolBar* tb = addToolBar("Tool Bar");
    tb->setIconSize(QSize(20,20));
    ret = ret && initFileToolItem(tb);
    tb->addSeparator();
    ret = ret && initEditToolItem(tb);
    tb->addSeparator();
    ret = ret && initFormatToolItem(tb);
    tb->addSeparator();
    ret = ret && initViewToolItem(tb);
    return ret;
}

bool MainWindow::initStatusBar()
{
    bool ret = true;
    QStatusBar* sb = statusBar();
    QLabel* label = new QLabel("NOTEPAD");
    if( label != NULL )
    {
        statusLabel.setMinimumWidth(200);
        statusLabel.setAlignment(Qt::AlignCenter);
        statusLabel.setText("ln:1  col:1");
        label->setMinimumWidth(200);
        label->setAlignment(Qt::AlignCenter);

        sb->addPermanentWidget(&statusLabel);
        sb->addPermanentWidget(label);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::initMainEditor()
{
    bool ret = true;
    mainEditor.setParent(this);
    //mainEditor.insertPlainText("");
    setCentralWidget(&mainEditor);

    QPalette p = mainEditor.palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active,  QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    mainEditor.setPalette(p);

    connect(&mainEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(&mainEditor, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(&mainEditor, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    connect(&mainEditor, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(&mainEditor, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

    return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("File(&F)", mb);
    bool ret = (menu != NULL);
    QAction* action = NULL;
    if( ret )
    {
        ret = ret && menuAddAction(action, "New(&N)", Qt::CTRL + Qt::Key_N, menu);
        if(ret)
        {
            connect(action, SIGNAL(triggered()), this , SLOT(onFileNew()));
        }
        ret = ret && menuAddAction(action, "Open(&O)...", Qt::CTRL + Qt::Key_O, menu);
        if(ret)
        {
            connect(action, SIGNAL(triggered()), this , SLOT(onFileOpen()));
        }

        menu->addSeparator();
        ret = ret && menuAddAction(action, "Save(&S)", Qt::CTRL + Qt::Key_S, menu);
        if(ret)
        {
            connect(action, SIGNAL(triggered()), this , SLOT(onFileSave()));
        }

        ret = ret && menuAddAction(action, "Save As(&A)...", 0, menu);
        if(ret)
        {
            connect(action, SIGNAL(triggered()), this , SLOT(onFileSaveAs()));
        }

        ret = ret && menuAddAction(action, "Print(&P)...", Qt::CTRL + Qt::Key_P, menu);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onFilePrint()));
        }
        menu->addSeparator();
        ret = ret && menuAddAction(action, "Exit(&X)", Qt::CTRL + Qt::Key_X, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFileExit()));
        }
    }
    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initEditMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("Edit(&E)", mb);
    bool ret = (menu != NULL);
    QAction* action = NULL;
    if( ret )
    {
        ret = ret && menuAddAction(action, "Undo(&U)", Qt::CTRL + Qt::Key_Z, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
            action->setEnabled(false);
        }

        ret = ret && menuAddAction(action, "Redo(&R)...", Qt::CTRL + Qt::Key_Y, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
            action->setEnabled(false);
        }

        menu->addSeparator();
        ret = ret && menuAddAction(action, "Cut(&T)", Qt::CTRL + Qt::Key_X, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
            action->setEnabled(false);
        }

        ret = ret && menuAddAction(action, "Copy(&C)...", Qt::CTRL + Qt::Key_C, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
            action->setEnabled(false);
        }

        ret = ret && menuAddAction(action, "Paste(&P)...", Qt::CTRL + Qt::Key_V, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
        }

        ret = ret && menuAddAction(action, "Delete(&L)", Qt::Key_Delete, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(clear()));
        }

        menu->addSeparator();
        ret = ret && menuAddAction(action, "Find(&F)...", Qt::CTRL + Qt::Key_F, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditFind()));
        }

        ret = ret && menuAddAction(action, "Replace(&R)...", Qt::CTRL + Qt::Key_H, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditReplace()));
        }

        ret = ret && menuAddAction(action, "Goto(&G)...", Qt::CTRL + Qt::Key_G, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
        }
        menu->addSeparator();
        ret = ret && menuAddAction(action, "Select All(&A)", Qt::CTRL + Qt::Key_A, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(selectAll()));
        }
    }
    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    /*
    QList<QAction*> menuactions = menu->actions();
    for(int i =0; i <menuactions.length(); i++)
    {
        qDebug()<<menuactions[i];
    }
    */
    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("Format(&O)", mb);
    bool ret = (menu != NULL);
    QAction* action = NULL;
    if( ret )
    {

        ret = ret && menuAddAction(action, "Auto Wrap(&W)", 0, menu);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onFormatWrap()));
        }
        ret = ret && menuAddAction(action, "Font(&F)...", 0, menu);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onFormatFont()));
        }

    }
    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initViewMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("View(&V)", mb);
    bool ret = (menu != NULL);
    QAction* action = NULL;
    if( ret )
    {
        ret = ret && menuAddAction(action, "Tool Bar(&T)", 0, menu);
        if( ret )
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));
            menu->addAction(action);
        }
        ret = ret && menuAddAction(action, "Status Bar(&S)", 0, menu);
        if( ret )
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));
            menu->addAction(action);
        }
    }
    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("Help(&H)", mb);
    bool ret = (menu != NULL);
    QAction* action = NULL;
    if( ret )
    {

        ret = ret && menuAddAction(action, "User Manual", 0, menu);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onHelpManual()));
            menu->addAction(action);
        }
        ret = ret && menuAddAction(action, "About NotePad...", 0, menu);

        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onHelpAbout()));
            menu->addAction(action);
        }
    }
    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initFileToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;
    ret = ret && toolBarAddAction(action, "New", ":/pic/new.png", tb);
    if(ret)
    {
        connect(action, SIGNAL(triggered()), this , SLOT(onFileNew()));
    }
    ret = ret && toolBarAddAction(action, "Open", ":/pic/open.png", tb);
    if(ret)
    {
        connect(action, SIGNAL(triggered()), this , SLOT(onFileOpen()));
    }
    ret = ret && toolBarAddAction(action, "Save", ":/pic/save.png", tb);
    if(ret)
    {
        connect(action, SIGNAL(triggered()), this , SLOT(onFileSave()));
    }
    ret = ret && toolBarAddAction(action, "Save As", ":/pic/saveas.png", tb);
    if(ret)
    {
        connect(action, SIGNAL(triggered()), this , SLOT(onFileSaveAs()));
    }
    ret = ret && toolBarAddAction(action, "Print", ":/pic/print.png", tb);
    connect(action,SIGNAL(triggered()),this,SLOT(onFilePrint()));
    return ret;
}

bool MainWindow::initEditToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;
    ret = ret && toolBarAddAction(action, "Undo", ":/pic/undo.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
        action->setEnabled(false);
    }
    ret = ret && toolBarAddAction(action, "Redo", ":/pic/redo.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
        action->setEnabled(false);
    }
    ret = ret && toolBarAddAction(action, "Cut", ":/pic/cut.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
        action->setEnabled(false);
    }
    ret = ret && toolBarAddAction(action, "Copy", ":/pic/copy.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
        action->setEnabled(false);
    }
    ret = ret && toolBarAddAction(action, "Paste", ":/pic/paste.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
    }
    ret = ret && toolBarAddAction(action, "Find", ":/pic/find.png", tb);

    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditFind()));
    }

    ret = ret && toolBarAddAction(action, "Replace", ":/pic/replace.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditReplace()));
    }
    ret = ret && toolBarAddAction(action, "Goto", ":/pic/goto.png", tb);
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
    }
    return ret;
}

bool MainWindow::initFormatToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;
    ret = ret && toolBarAddAction(action, "Auto Wrap", ":/pic/wrap.png",tb);
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFormatWrap()));
    }
    ret = ret && toolBarAddAction(action, "Font", ":/pic/font.png",tb);
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFormatFont()));
    }
    return ret;
}

bool MainWindow::initViewToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;
    ret = ret && toolBarAddAction(action, "Tool Bar", ":/pic/tool.png",tb);
    if( ret )
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));
        tb->addAction(action);
    }
    ret = ret && toolBarAddAction(action, "Status Bar", ":/pic/status.png",tb);
    if( ret )
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));
        tb->addAction(action);
    }
    return ret;
}

bool MainWindow::menuAddAction(QAction*  &action, QString text, int key, QWidget *parent)
{
    bool ret = true;
    action =  new QAction(text, parent);
    if( action != NULL )
    {
        action->setShortcut(QKeySequence(key));
        parent->addAction(action);
    }
    else
    {
        ret = false;
        delete action;
    }

    return  ret;
}

bool MainWindow::toolBarAddAction(QAction* &action, QString tip, QString icon, QWidget *parent)
{
    bool ret = true;
    action = new QAction(tip, parent);
    if( action != NULL)
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
        parent->addAction(action);
    }
    else
    {
        ret = false;
        delete action;
    }
    return ret;
}

QToolBar* MainWindow:: toolBar()
{
    QToolBar* ret = NULL;
    const QObjectList& list = children();

    for(int i=0; i<list.count(); i++)
    {
        QToolBar* tb = dynamic_cast<QToolBar*>(list[i]);

        if( tb != NULL )
        {
            ret = tb;
            break;
        }
    }

    return ret;

}

QAction* MainWindow:: findMenuBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = menuBar()->children();

    for(int i=0; i<list.count(); i++)
    {
        QMenu* menu = dynamic_cast<QMenu*>(list[i]);

        if( menu != NULL )
        {
            QList<QAction*> actions = menu->actions();

            for(int j=0; j<actions.count(); j++)
            {
                if( actions[j]->text().startsWith(text) )
                {
                    ret = actions[j];
                    break;
                }
            }
        }
    }

    return ret;

}
QAction* MainWindow:: findToolBarAction(QString text)
{
    QAction* ret = NULL;
    QList<QAction*> actions = toolBar()->actions();

    for(int j=0; j<actions.count(); j++)
    {
        if( actions[j]->toolTip().startsWith(text) )
        {
            ret = actions[j];
            break;
        }
    }

    return ret;

}

MainWindow::~MainWindow()
{
}

