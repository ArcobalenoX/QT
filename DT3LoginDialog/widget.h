#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QFontDialog>
#include <QProgressDialog>
#include <QPrintDialog>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QStackedLayout>
#include <QTimer>
#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT
private:
    QPushButton LoginBtn;
    QPushButton MessBtn;
    QPushButton FileBtn;
    QPushButton ColorBtn;
    QPushButton InputBtn;
    QPushButton FontBtn;
    QPushButton ProgressBtn;
    QPushButton PrinterBtn;

    void set_btn();
    void Manual_Layout();
    void Box_Layout();
    void Grid_Layout();
    void Form_Layout();
    void Stack_Layout();

    QPushButton preBtn;
    QPushButton nextBtn;
    QStackedLayout sLayout;
    QWidget* get1stPage();
    QWidget* get2ndPage();
    QWidget* get3rdPage();

private slots:
    void LoginBtn_Clicked();
    void MessBtn_Clicked();
    void FileBtn_Clicked();
    void ColorBtn_Clicked();
    void InputBtn_Clicked();
    void FontBtn_Clicked();
    void ProgressBtn_Clicked();
    void PrinterBtn_Clicked();
    void Timer_timeout();
    void onPreBtnClicked();
    void onNextBtnClicked();

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};


#endif // WIDGET_H
