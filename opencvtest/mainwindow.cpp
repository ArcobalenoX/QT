#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(200,200,200,200);

    QVBoxLayout* layout = new QVBoxLayout();


    QPushButton *btn1 = new QPushButton(this);
    btn1->setText("show image");



    QPushButton *btn2 = new QPushButton(this);
    btn2->setText("bgr2gray");

    layout->addWidget(btn1);
    layout->addWidget(btn2);

    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    this->setCentralWidget(widget);

    connect(btn1, SIGNAL(clicked()), this, SLOT(read_image()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(convert()));

    MyThread * t1 = new MyThread();
    connect(t1,SIGNAL(MySignal(QString)),this,SLOT(HandleSignal(QString)));
    t1->start();

    src_image = new Mat;

}

void call(int event, int x, int y, int flags, void *userdata)
{
    static int stx,sty;

    if(event == EVENT_LBUTTONDOWN )
    {
        qDebug()<<"EVENT_LBUTTONDOWN "<<x<<"  "<<y;
        stx = x;
        sty = y;
    }
    if(event == EVENT_LBUTTONUP )
    {
        qDebug()<<"EVENT_LBUTTONUP "<<x<<"  "<<y;

    }
}


void MainWindow::read_image(void)
{
    qDebug()<<"read image";

    Mat image=imread("F:\\Picture\\ACGN\\BQ (276).jpg",1);
    qDebug()<<"width "<<image.cols;
    qDebug()<<"height "<<image.rows;
    //cv::resize(image, image,cv::Size(500,500));
    namedWindow( "Display", WINDOW_AUTOSIZE );
    imshow( "Display", image );
    setMouseCallback("Display",call,0);


    Rect rect = selectROI(image,true,true);
    rectangle(image,rect,Scalar(255,255,0));


    Mat roi(image, rect);
    //roi = Scalar(100,100,100);

    namedWindow( "roi", WINDOW_AUTOSIZE );
    imshow("roi",roi);


}



void MainWindow::convert(void)
{


    qDebug()<<"convert";
    setWindowProperty("Display window",WND_PROP_VISIBLE,0 );

    Mat image_gray;
    cvtColor(*src_image, image_gray, COLOR_BGR2GRAY);
    namedWindow( "Gray image", WINDOW_AUTOSIZE );
    imshow( "Gray image", image_gray );

}


void MainWindow:: HandleSignal(QString text)
{
    qDebug()<<text;


}


MainWindow::~MainWindow()
{
    delete ui;
    destroyAllWindows();
}

void MyThread::run()
{


    QString text("send");

    for(int i=0;i<5;i++)
    {
        emit MySignal(text);
    }
}

