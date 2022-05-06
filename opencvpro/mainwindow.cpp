#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    Mat img = imread("E:\\Code\\QT\\opencvpro\\test.jpg");
//    cvtColor(img, img, cv::COLOR_BGR2RGB);
//    QImage image = QImage((const unsigned char *)(img.data),img.cols,img.rows,img.step,QImage::Format_RGB888);
//    ui->label->setPixmap(QPixmap::fromImage(image));
//    ui->label->resize(image.size());
//    ui->label->show();
    //resize(800,500);
    Mat image=imread("E:\\Code\\QT\\opencvpro\\test.jpg");//一定要使用绝对路径，其他可以回报错
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

