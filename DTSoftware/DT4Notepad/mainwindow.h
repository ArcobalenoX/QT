#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QKeySequence>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIcon>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QMimeData>
#include <QInputDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QPagedPaintDevice>
#include <QSharedPointer>
#include <QDesktopServices>
#include <QFontDialog>
#include <QFileSystemModel>
#include <QTreeView>


#include "finddialog.h"
#include "replacedialog.h"
#include "appconfig.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    QPlainTextEdit mainEditor;
    QString m_filePath;
    QLabel statusLabel;
    bool m_isTextChanged;
    findDialog* m_pFindDialog;
    replaceDialog* m_pReplaceDialog;

    MainWindow();
    MainWindow(const MainWindow&);
    MainWindow& operator = (const MainWindow&);

    bool construct();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEditor();

    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);

    bool initFileToolItem(QToolBar* tb);
    bool initEditToolItem(QToolBar* tb);
    bool initFormatToolItem(QToolBar* tb);
    bool initViewToolItem(QToolBar* tb);

    bool menuAddAction(QAction* &action, QString text, int key, QWidget* parent);
    bool toolBarAddAction(QAction* &action, QString tip, QString icon, QWidget* parent);

    QString showFileDialog(QFileDialog::AcceptMode mode, QString title);
    int showQueryMessage(QString message);
    void showErrorMessage(QString message);
    QString saveCurrentData(QString path = "");
    void preEditorChange();
    void openFileToEditor(QString path);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);

protected:
    void closeEvent(QCloseEvent* e);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();
    void onFilePrint();
    void onTextChanged();

    void onCopyAvailable(bool available);
    void onRedoAvailable(bool available);
    void onUndoAvailable(bool available);
    void onCursorPositionChanged();
    void onEditFind();
    void onEditReplace();
    void onEditDelete();
    void onEditGoto();
    void onFormatWrap();
    void onFormatFont();
    void onViewToolBar();
    void onViewStatusBar();
    void onHelpManual();
    void onHelpAbout();

public:
    static MainWindow* newInstance();
    ~MainWindow();
    QToolBar* toolBar();
    void openFile(QString path);
};
#endif // MAINWINDOW_H
