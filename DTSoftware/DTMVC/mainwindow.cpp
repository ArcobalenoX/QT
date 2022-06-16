#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(640,480);

    m_edit.setParent(this);
//    m_edit.move(100,100);
    m_edit.resize(500,300);
    connect(&m_fsModel,SIGNAL(directoryLoaded(QString)),this,SLOT(onDirectoryLoaded(QString)));
    m_fsModel.setRootPath(QDir::currentPath());

//    m_treeView.setParent(this);
//    m_treeView.move(10, 10);
//    m_treeView.resize(500, 300);
//    m_fsModel.setRootPath(QDir::currentPath());
//    m_treeView.setModel(&m_fsModel);
//    m_treeView.setRootIndex(m_fsModel.index(QDir::currentPath()));


}

void MainWindow::onDirectoryLoaded(const QString& path)
{
    QModelIndex root = m_fsModel.index(path);
    QByteArray array;
    QBuffer buffer(&array);
    if(buffer.open(QIODevice::WriteOnly))
    {
        QTextStream out(&buffer);
        out<< m_fsModel.isDir(root) <<endl;
        out<< m_fsModel.data(root).toString() <<endl;
        out<< root.data().toString() <<endl;
        out<< &m_fsModel <<endl;
        out << root.model() << endl;
        out << m_fsModel.filePath(root) << endl;
        out << m_fsModel.fileName(root) << endl;
        out << endl;
        for(int i=0;i<m_fsModel.rowCount(root);i++)
        {
            QModelIndex cindex  = m_fsModel.index(i,0,root);
            out << cindex.data().toString() <<endl;

        }
        out.flush();
        buffer.close();
    }

    if( buffer.open(QIODevice::ReadOnly) )
    {
        QTextStream in(&buffer);

        m_edit.insertPlainText(in.readAll());

        buffer.close();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

