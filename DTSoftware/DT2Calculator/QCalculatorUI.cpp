#include "QCalculatorUI.h"
#include <QDebug>
#include <QString>
QCalculatorUI::QCalculatorUI() : QWidget(NULL, Qt::WindowCloseButtonHint)
{
    calculator = NULL;
}

bool QCalculatorUI ::construct()
{
    bool ret = true;
    const char* btnTexts[20]=
    {
        "7","8","9","(",")",
        "4","5","6","+","-",
        "1","2","3","*","/",
        "0",".","←","=","C",
    };
    this->resize(400,300);
    LineEdit = new QLineEdit(this);
    if(LineEdit !=NULL)
    {
        LineEdit->move(100,10);
        LineEdit->resize(240,30);
        LineEdit->setReadOnly(true);
        LineEdit->setAlignment(Qt::AlignRight);
    }
    else
        ret = false;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<5;j++)
        {
            buttons[i*5+j] = new QPushButton(this);
            if( buttons[i*5+j] != NULL)
            {
                buttons[i*5+j]->resize(40, 40);
                buttons[i*5+j]->move(100+(10+40)*j, 50+(10+40)*i);
                buttons[i*5+j]->setText(btnTexts[i*5+j]);
                connect(buttons[i*5+j],SIGNAL(clicked()),this,SLOT(onButtonClicked()));
            }
            else
                ret = false;
        }

    }

    return ret;
}
QCalculatorUI* QCalculatorUI::NewInstance()
{
    QCalculatorUI* ret = new QCalculatorUI();
    if(ret == NULL || !ret->construct())
    {
        delete ret;
        ret = NULL;
    }

    return ret;
}

void QCalculatorUI::show()
{
    QWidget::show();
    setFixedSize(width(),height());

}

void QCalculatorUI::onButtonClicked()
{
    QPushButton* btn = (QPushButton*) sender();
    QString text = LineEdit->text();
    QString clicktext = btn->text();
    if(clicktext == "←")
    {
        if(text.length()>0)
        {
            LineEdit->setText(text.remove(text.length()-1,1));
        }
    }
    else if(clicktext == "C")
    {
         LineEdit->setText("");
    }
    else if( clicktext == "=" )
    {
        if( calculator != NULL )
        {
            calculator->expression(text);
            LineEdit->setText(calculator->result());
        }
    }
    else
    {
        LineEdit->setText(text+btn->text());
    }

}

void QCalculatorUI::setCalculator(ICalculator* cal)
{
    calculator = cal;
}

ICalculator* QCalculatorUI::getCalculator()
{
    return calculator;
}
QCalculatorUI::~QCalculatorUI()
{

}
