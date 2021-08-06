#include <cstdio>
#include <iostream>
#include <string>
#include<sstream>
#include "cast_demo.h"
#include "complex.h"
#include "templatearray.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class Object
{
    string ms;
public:
    Object(string s)
    {
        cout << "Object(string s) : " << s << endl;
        ms = s;
    }
    ~Object()
    {
        cout << "~Object() : " << ms << endl;
    }
};

class Parent : public Object
{
    string ms;
public:
    Parent() : Object("Default parent ")
    {
        cout << "Parent()" << endl;
        ms = "Default";
    }
    Parent(string s) : Object(s)
    {
        cout << "Parent(string s) : " << s << endl;
        ms = s;
    }
    ~Parent()
    {
        cout << "~Parent() : " << ms << endl;
    }
};

class Child : public Parent
{
    Object mO1;
    Object mO2;
    string ms;
public:
    Child() : mO1("Default m1"), mO2("Default m2")
    {
        cout << "Child()" << endl;
        ms = "Default";
    }
    Child(string s) : Parent(s), mO1(s + " m1"), mO2(s + " m2")
    {
        cout << "Child(string s) : " << s << endl;
        ms = s;
    }
    ~Child()
    {
        cout << "~Child() " << ms << endl;
    }
};




int main()
{
    //Mat imgae =  imread("./test.jpg");
    /*Array<int, 10> arr;
    int tt;
    arr.set(1,10);
    cout<<arr.get(1,tt)<<tt<<endl;
*/
    int a[]={11,22,33,44};
    int *p;
    int i ;
    p=a;
    for(i=0;i<3;i++)
    {
        a[i]=*p;
        p++;
    }
    cout<<a[1]<<endl;
    cout<<a[2]<<endl;

        int arr[] = {1,2,3,4,5,6,7,8,9,10};
        //int *p = arr;  //定义一个整型指针变量p保存整形数组arr的首地址
        *p = 10;  //通过解引用访问arr[0]
        p++;
        *p = 20;  //p进行加一操作后重新赋值
        printf("十进制 %d,%d\n",arr[0],arr[1]);
        printf("十六进制 %08x,%08x\n",arr[0],arr[1]);


    return 0;
}

