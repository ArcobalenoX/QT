#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFileSystemModel>
#include <QTreeView>
#include <QModelIndex>
#include <QByteArray>
#include <QBuffer>
#include <QTextStream>
#include <QDir>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPlainTextEdit m_edit;
    QFileSystemModel m_fsModel;
    QTreeView m_treeView;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void onDirectoryLoaded(const QString& path);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
