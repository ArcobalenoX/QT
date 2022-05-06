#include "mainwindow.h"

#include <QApplication>

#include <cstdio>
#include <iostream>
#include <string>
#include<sstream>
using namespace std;

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
//#include "opencv2/ximgproc.hpp"
//#include "ximgproc/structured_edge_detection.hpp"
using namespace cv;
//using namespace cv :: imgproc;
const char* keys =
{

    "{i || input image name}"

    "{m || model name}"

    "{o || output image name}"

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();



    /*
    //Mat imgae =  imread("./test.jpg");
    cv::Mat img;
    img = cv::imread("test.jpg");//参数1：图片路径。参数2:显示原图

    namedWindow("窗口1",CV_WINDOW_NORMAL);
    //参数1：窗口的名字
    //参数2：窗口类型，CV_WINDOW_AUTOSIZE 时表明窗口大小等于图片大小。不可以被拖动改变大小。
    //CV_WINDOW_NORMAL 时，表明窗口可以被随意拖动改变大小。

    imshow("窗口1", img);//在“窗口1”这个窗口输出图片。
    waitKey(5000);//等待5秒，程序自动退出。改为0，不自动退出。
    */

//    bool printHelp = ( argc == 1 );

//    printHelp = printHelp || ( argc == 2 && std::string(argv[1]) == "--help" );

//    printHelp = printHelp || ( argc == 2 && std::string(argv[1]) == "-h" );


//    if ( printHelp )

//    {

//        printf("\nThis sample demonstrates structured forests for fast edge detection\n"

//               "Call:\n"

//               " structured_edge_detection -i=in_image_name -m=model_name [-o=out_image_name]\n\n");

//        return 0;

//    }


//    cv::CommandLineParser parser(argc, argv, keys);

//    if ( !parser.check() )

//    {

//        parser.printErrors();

//        return -1;

//    }


//    std::string modelFilename = parser.get<std::string>("m");

//    std::string inFilename = parser.get<std::string>("i");

//    std::string outFilename = parser.get<std::string>("o");


//    cv::Mat image = cv::imread(inFilename, 1);

//    if ( image.empty() )

//    {

//        printf("Cannot read image file: %s\n", inFilename.c_str());

//        return -1;

//    }


//    image.convertTo(image, cv::DataType<float>::type, 1/255.0);


//    cv::Mat edges(image.size(), image.type());


//    cv::Ptr<StructuredEdgeDetection> pDollar =

//            createStructuredEdgeDetection(modelFilename);

//    pDollar->detectEdges(image, edges);


//    if ( outFilename == "" )

//    {

//        cv::namedWindow("edges", 1);

//        cv::imshow("edges", edges);


//        cv::waitKey(0);

//    }

//    else

//        cv::imwrite(outFilename, 255*edges);


    return a.exec();
}
