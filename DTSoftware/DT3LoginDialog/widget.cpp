#include "widget.h"
#include "qlogindialog.h"


Widget::Widget(QWidget *parent) : QWidget(parent),
    LoginBtn(this),MessBtn(this),FileBtn(this),ColorBtn(this),InputBtn(this),
    FontBtn(this),ProgressBtn(this),PrinterBtn(this)
{
    set_btn();
    //Manual_Layout();
    //Box_Layout();
    //Grid_Layout();
    //Form_Layout();

    Stack_Layout();

    connect(&LoginBtn, SIGNAL(clicked()), this, SLOT(LoginBtn_Clicked()));
    connect(&MessBtn, SIGNAL(clicked()), this, SLOT(MessBtn_Clicked()));
    connect(&FileBtn,SIGNAL(clicked()),this,SLOT(FileBtn_Clicked()));
    connect(&ColorBtn,SIGNAL(clicked()),this,SLOT(ColorBtn_Clicked()));
    connect(&InputBtn,SIGNAL(clicked()),this,SLOT(InputBtn_Clicked()));
    connect(&FontBtn,SIGNAL(clicked()),this,SLOT(FontBtn_Clicked()));
    connect(&ProgressBtn,SIGNAL(clicked()),this,SLOT(ProgressBtn_Clicked()));
    connect(&PrinterBtn,SIGNAL(clicked()),this,SLOT(PrinterBtn_Clicked()));

}

void Widget::set_btn()
{
    LoginBtn.setText("Test Login Dialog");
    //LoginBtn.setMinimumSize(100,25);
    LoginBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MessBtn.setText("messagebox");
    MessBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    FileBtn.setText("file_dialog");
    FileBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ColorBtn.setText("color_diaglog");
    ColorBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    InputBtn.setText("input_dialog");
    InputBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    FontBtn.setText("font_dialog");
    FontBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ProgressBtn.setText("progress_dialog");
    ProgressBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    PrinterBtn.setText("printer_dialog");
    PrinterBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Widget:: Manual_Layout()
{
    setFixedSize(200, 250);

    LoginBtn.move(30,25);

    MessBtn.move(30,50);

    FileBtn.move(30,75);

    ColorBtn.move(30,100);

    InputBtn.move(30,125);

    FontBtn.move(30,150);

    ProgressBtn.move(30,175);

    PrinterBtn.move(30,200);
}

void Widget:: Box_Layout()
{

    QVBoxLayout* vbl = new QVBoxLayout();
    QHBoxLayout* hbl1 = new QHBoxLayout();
    QHBoxLayout* hbl2 = new QHBoxLayout();

    vbl->setSpacing(10);
    hbl1->setSpacing(30);
    hbl2->setSpacing(30);

    hbl1->addWidget(&LoginBtn);
    hbl1->setStretch(0,1);
    hbl1->addWidget(&MessBtn);
    hbl1->setStretch(1,2);
    hbl1->addWidget(&FileBtn);
    hbl1->setStretchFactor(&FileBtn,3);
    hbl1->addWidget(&ColorBtn);
    hbl1->setStretchFactor(&ColorBtn,4);

    hbl2->addWidget(&InputBtn);
    hbl2->addWidget(&FontBtn);
    hbl2->addWidget(&ProgressBtn);
    hbl2->addWidget(&PrinterBtn);

    vbl->addLayout(hbl1);
    vbl->addLayout(hbl2);

    setLayout(vbl);
}

void Widget:: Grid_Layout()
{
    QGridLayout* gl = new QGridLayout();
    gl->setSpacing(10);

    gl->addWidget(&LoginBtn,0,0,1,2);
    gl->addWidget(&MessBtn,1,0);
    gl->addWidget(&FileBtn,1,1);
    gl->addWidget(&ColorBtn,2,0);
    gl->addWidget(&InputBtn,2,1);
    gl->addWidget(&FontBtn,3,0);
    gl->addWidget(&ProgressBtn,3,1);
    gl->addWidget(&PrinterBtn,3,2);

    gl->setRowStretch(0,1);
    gl->setColumnStretch(0,2);
    gl->setRowStretch(1,2);
    gl->setRowStretch(2,2);
    gl->setRowStretch(3,3);

    setLayout(gl);

}
void Widget::Form_Layout()
{
    QFormLayout* fl = new QFormLayout();
    QLineEdit* line1 = new QLineEdit();
    QLineEdit* line2 = new QLineEdit();
    QLineEdit* line3 = new QLineEdit();

    fl->addRow("line1",line1);
    fl->addRow("line2",line2);
    fl->addRow("line3",line3);

    setLayout(fl);
}

QWidget* Widget::get1stPage()
{
    QWidget *widget = new QWidget();
    QGridLayout* layout = new QGridLayout();

    QLabel* label=new QLabel("This is 1st page");
    layout->addWidget(label);
    layout->addWidget(&LoginBtn);
    widget->setLayout(layout);
    return widget;
}

QWidget* Widget::get2ndPage()
{
    QWidget *widget = new QWidget();


    QGridLayout* gl = new QGridLayout();
    gl->setSpacing(10);

    QLineEdit* sLineEdit = new QLineEdit("This is 2rd page");

    gl->addWidget(sLineEdit,0,0,1,2);
    gl->addWidget(&MessBtn,1,0);
    gl->addWidget(&FileBtn,1,1);
    gl->addWidget(&ColorBtn,2,0);
    gl->addWidget(&InputBtn,2,1);

    gl->setRowStretch(0,1);
    //gl->setColumnStretch(0,2);
    gl->setRowStretch(1,2);
    gl->setRowStretch(1,2);

    widget->setLayout(gl);
    return widget;
}

QWidget* Widget::get3rdPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* PushBtn = new QPushButton("This is 3rd page");
    layout->addWidget(PushBtn);
    layout->addWidget(&FontBtn);
    layout->addWidget(&ProgressBtn);
    layout->addWidget(&PrinterBtn);

    widget->setLayout(layout);
    return widget;
}

void Widget::onPreBtnClicked()
{
    int index = ((sLayout.currentIndex() - 1) + sLayout.count()) % sLayout.count();
    sLayout.setCurrentIndex(index);
}

void Widget::onNextBtnClicked()
{
    int index = ((sLayout.currentIndex() + 1) ) % sLayout.count();
    sLayout.setCurrentIndex(index);
}

void Widget:: Stack_Layout()
{
    QHBoxLayout *hbl = new QHBoxLayout();
    QVBoxLayout *vbl =new QVBoxLayout();

    preBtn.setText("pre_page");
    preBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    preBtn.setMinimumSize(160, 50);

    nextBtn.setText("next_page");
    nextBtn.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    nextBtn.setMinimumSize(160, 50);

    connect(&preBtn, SIGNAL(clicked()), this, SLOT(onPreBtnClicked()));
    connect(&nextBtn, SIGNAL(clicked()), this, SLOT(onNextBtnClicked()));

    hbl->addWidget(&preBtn);
    hbl->addWidget(&nextBtn);

    sLayout.addWidget(get1stPage());
    sLayout.addWidget(get2ndPage());
    sLayout.addWidget(get3rdPage());

    vbl->addLayout(&sLayout);
    vbl->addLayout(hbl);

    setLayout(vbl);

/*
    QTimer* timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(Timer_timeout()));
    timer->start(2000);
*/

}

void Widget::Timer_timeout()
{
    qDebug()<<"Timer_timeout";
    qDebug()<<layout();

    int index = (sLayout.currentIndex()+1)%sLayout.count();
    sLayout.setCurrentIndex(index);
}

void Widget::LoginBtn_Clicked()
{
    QLoginDialog dlg;

    if( dlg.exec() == QDialog::Accepted )
    {
        qDebug() << "User: " + dlg.getUser();
        qDebug() << "Pwd: " + dlg.getPwd();
    }
}

void Widget::MessBtn_Clicked()
{
    QMessageBox mb;

    //mb.question(this,"title","text",QMessageBox::Ok,QMessageBox::Yes,QMessageBox:: No);
    //mb.information(this,"title","text",QMessageBox::Save,QMessageBox::Open);

    mb.setWindowTitle("Window Title");
    mb.setText("This is a detail message dialog!");
    mb.setIcon(QMessageBox::Information);
    mb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::YesToAll);

    if( mb.exec() == QMessageBox::Ok )
    {
        qDebug() << "Ok button is clicked!";
    }
}

void Widget::FileBtn_Clicked()
{
    QFileDialog fd;
    //fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setFilter(QDir::Files);
    fd.setFileMode(QFileDialog::ExistingFiles);

    if( fd.exec() == QFileDialog::Accepted )
    {
        QStringList fs = fd.selectedFiles();

        for(int i=0; i<fs.count(); i++)
        {
            qDebug() << fs[i];
        }
    }
}

void Widget::ColorBtn_Clicked()
{
    QColorDialog cd;
    cd.setWindowTitle("color editor");
    cd.setCurrentColor(QColor(128,111,222));

    if( cd.exec() == QColorDialog::Accepted )
    {
        QColor color = cd.selectedColor();

        qDebug() << color;
        qDebug() << color.red();
        qDebug() << color.green();
        qDebug() << color.blue();
        qDebug() << color.hue();
        qDebug() << color.saturation();
        qDebug() << color.value();
    }
}

void Widget::InputBtn_Clicked()
{
    QInputDialog id;
    id.setWindowTitle("input:");
    id.setInputMode(QInputDialog::IntInput);
    id.setIntRange(0,999);

    id.setLabelText("Please input an integer:");
  //  id.setInputMode(QInputDialog::TextInput);


    if( id.exec() == QInputDialog::Accepted )
    {
        //qDebug() << id.textValue();
        qDebug()<<id.intValue();
    }

}

void Widget::FontBtn_Clicked()
{
    QFontDialog fd;
    fd.setWindowTitle("font");
    if(fd.exec()==QFontDialog::Accepted)
    {
        bool ok;
        qDebug()<<fd.getFont(&ok);

    }

}
void Widget::ProgressBtn_Clicked()
{
    QProgressDialog pd(this);

    pd.setWindowTitle("Updating...");
    pd.setLabelText("Downloading update from server...");
    pd.setMinimum(0);
    pd.setMaximum(100);
    pd.setValue(35);

    // create a new thread

    pd.exec();


}
void Widget::PrinterBtn_Clicked()
{
       QPrintDialog pd(this);

       pd.setWindowTitle("Print Dialog Test");

       if( pd.exec() == QPrintDialog::Accepted )
       {

           //QPrinter* p = pd.printer();

           QTextDocument td;
           td.setPlainText("Printer object test!");
           td.setHtml("<h1>Print html object test</hl>");

           //p->setOutputFileName("test.pdf");
           //td.print(p);

       }
}


Widget::~Widget()
{

}

