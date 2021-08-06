#include "QCalculatorDec.h"
#include <QDebug>
QCalculatorDec::QCalculatorDec()
{
    m_exp = "";
    m_result = "";
    /*
    QQueue<QString> exp = split("9+(3-1)*5");
    for(int i=0;i<exp.length();i++)
    {
        qDebug()<<exp[i];
    }
    qDebug()<<match(exp);

    QQueue<QString> output;
    transform(exp,output);
    for(int i=0;i<output.length();i++)
    {
        qDebug()<<output[i];
    }
    qDebug()<<calculate(output);
    */
}

QCalculatorDec::~QCalculatorDec()
{

}

QString QCalculatorDec::result()
{
    return m_result;
}

bool QCalculatorDec::isDigitaOrDot(QChar c)
{
    return ( ( ('0'<=c) &&(c<='9') ) || (c=='.') );
}

bool QCalculatorDec::isSymbol(QChar c)
{
    return isOperator(c) || (c=='(') || (c==')');
}

bool QCalculatorDec::isSign(QChar c)
{
    return (c=='+') || (c=='-');
}

bool QCalculatorDec::isNumber(QString s)
{
    bool ret =false;
    s.toDouble(&ret);
    return ret;
}

bool QCalculatorDec::isOperator(QString s)
{
    return (s=='+') || (s=='-') || (s=='*') || (s=='/');
}

bool QCalculatorDec::isLeft(QString s)
{
    return (s=='(') ;
}

bool QCalculatorDec::isRight(QString s)
{
    return (s==')') ;
}

int QCalculatorDec::priority(QString s)
{
    int ret=0;
    if( (s=='+')||(s=='-'))
    {
        ret =1;
    }
    else if( (s=='*')||(s=='/'))
    {
        ret =2;
    }
    return ret;
}

/*分离中缀表达式，数字和符号分离*/
QQueue<QString> QCalculatorDec::split(const QString& exp)
{
    QQueue<QString> ret;
    QString num="";
    QString pre="";
    for(int i=0;i<exp.length();i++)
    {
        if(isDigitaOrDot((exp[i])))//数字或小数点
        {
            num+=exp[i];
            pre = exp[i];
        }
        else if(isSymbol(exp[i]))//符号
        {
            if(!num.isEmpty())
            {
                ret.enqueue((num));//分离并保存运算数
                num.clear();
            }
            if(isSign(exp[i]) && ((pre=="")||(pre=="(")||isOperator(pre)))//正负号
            {
                num+=exp[i];//符号位累计
            }
            else
            {
                ret.enqueue(exp[i]);//分离并保存运算符
            }
            pre  = exp[i];

        }
    }
    if(num.isEmpty())
    {
        ret.enqueue(num);
    }
    return ret;
}
/*判断中缀表达式的合法性，括号匹配运算*/
bool QCalculatorDec :: match(QQueue<QString>& exp)
{
    bool ret = true;
    int len = exp.length();
    QStack<QString> stack;
    for(int i=0;i<len;i++)
    {
        if( isLeft(exp[i]) )//左括号入栈
        {
            stack.push(exp[i]);
        }
        else if( isRight(exp[i]))//右括号
        {
            if( !stack.isEmpty() && isLeft(stack.top()))//出栈栈顶的左括号
            {
                stack.pop();
            }
            else
            {
                ret = false;
                break;
            }
        }
    }
    return ret && stack.isEmpty();
}

/*中缀表达式转换为后缀表达式*/
bool QCalculatorDec::transform(QQueue<QString>& exp, QQueue<QString>& output)
{
    bool ret = match(exp);
    QStack<QString> stack;
    output.clear();
    while (ret && ! exp.isEmpty())
    {
        QString e = exp.dequeue();
        if( isNumber(e) )
        {
            output.enqueue(e);//输出数字
        }
        else if( isOperator(e) )//运算符
        {
            while ( !stack.isEmpty() && priority(e) <= priority(stack.top()) )//与栈顶运算符比较优先级
            {
                output.enqueue(stack.pop());//小于等于，输出栈顶元素
            }
            stack.push(e);//入栈当前元素
        }
        else if ( isLeft(e) )//左括号入栈
        {
            stack.push(e);

        }
        else if ( isRight(e) )//右括号
        {
            while ( !stack.isEmpty() && !isLeft(stack.top()) )//栈顶元素出栈，直到栈顶元素为左括号
            {
                output.enqueue(stack.pop());
            }
            if( !stack.isEmpty() )
            {
                stack.pop();//栈顶的左括号出栈
            }

        }
        else
        {
            ret = false;
        }

    }
    while( !stack.isEmpty() )
    {
        output.enqueue(stack.pop());
    }
    if( !ret )
    {
        output.clear();
    }
    return  ret;
}

/*根据操作符计算两个操作数*/
QString QCalculatorDec::calculate(QString ln,QString op,QString rn)
{
    QString ret="Error";
    if ( isNumber(ln) && isNumber(rn))
    {
        double lp = ln.toDouble();
        double rp = rn.toDouble();
        if( op == "+" )
        {
            ret.asprintf("%f", lp + rp);
        }
        else if( op == "-" )
        {
            ret.asprintf("%f", lp - rp);
        }
        else if( op == "*" )
        {
            ret.asprintf("%f", lp * rp);
        }
        else if( op == "/" )
        {
            const double P = 0.000000000000001;

            if( (-P < rp) && (rp < P) )
            {
                ret = "Error";
            }
            else
            {
                ret.asprintf("%f", lp / rp);
            }

        }
        else
        {
            ret = "Error";
        }

    }
    return ret;
}

/*计算后缀表达式*/
QString QCalculatorDec::calculate(QQueue<QString>& exp)
{
    QString ret = "Error";
    QStack<QString> stack;
    while ( !exp.isEmpty())
    {
        QString e = exp.dequeue();
        if ( isNumber(e) )//数字入栈
        {
            stack.push(e);
        }
        else if ( isOperator(e) )//运算符
        {
            QString rp = !stack.isEmpty() ? stack.pop() : "";//出栈右操作数
            QString lp = !stack.isEmpty() ? stack.pop() : "";//出栈左操作数
            QString result = calculate(lp,e,rp);//双目运算
            if (result!= "Error")
            {
               stack.push(result);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (exp.isEmpty() && stack.size()==1 && isNumber(stack.top()))
    {
        ret = stack.pop();
    }
    return ret;
}

/*计算输入的中缀表达式*/
bool QCalculatorDec::expression(const QString& exp)
{
    bool ret = false;
    QQueue<QString> spExp = split(exp);
    QQueue<QString> postExp;

    m_exp = exp;

    if( transform(spExp, postExp) )
    {
        m_result = calculate(postExp);

        ret = (m_result != "Error");
    }
    else
    {
        m_result = "Error";
    }
    return ret;
}


