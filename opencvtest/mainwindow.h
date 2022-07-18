#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QThread>

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void read_image (void);
    void convert (void);
    void HandleSignal(QString);

private:
    Ui::MainWindow *ui;
    Mat* src_image;
    Mat* dst_image;


};




class MyThread : public QThread
{
     Q_OBJECT
    public:
    MyThread(){};
    protected:
        void run();
    signals:
        void MySignal(QString);
};







#endif // MAINWINDOW_H
