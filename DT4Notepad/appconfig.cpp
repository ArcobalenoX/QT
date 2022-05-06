#include "appconfig.h"

appconfig::appconfig(QObject *parent) : QObject(parent)
{
    m_isValid = restore();
}

appconfig:: appconfig(QFont font, QPoint point, QSize size, bool isWrap, bool tbisvisible, bool sbisvisible, QObject* parent):QObject(parent)
{
    m_editorFont = font;
    m_mainWindowSize = size;
    m_mainWindowPoint = point;
    m_isAutoWrap = isWrap;
    m_isToolBarVisible = tbisvisible;
    m_isStatusBarVisible = sbisvisible;
    m_isValid = true;
}

bool appconfig::restore()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath()+"/app.config");
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_14);
        in>>m_editorFont;
        in>>m_mainWindowPoint;
        in>>m_mainWindowSize;
        in>>m_isAutoWrap;
        in>>m_isToolBarVisible;
        in>>m_isStatusBarVisible;
        file.close();
    }
    else
    {
        ret = false;
    }
    return ret;

}

bool appconfig::store()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath()+"/app.config");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_14);
        out<<m_editorFont;
        out<<m_mainWindowPoint;
        out<<m_mainWindowSize;
        out<<m_isAutoWrap;
        out<<m_isToolBarVisible;
        out<<m_isStatusBarVisible;
        file.close();
    }
    else
    {
        ret = false;
    }
    return ret;

}
QFont appconfig::editorFont()
{
    return m_editorFont;
}
QSize appconfig::mainWindowSize()
{
    return m_mainWindowSize;
}
QPoint appconfig::mainWindowPoint()
{
    return m_mainWindowPoint;
}
bool appconfig::isAutoWrap()
{
    return m_isAutoWrap;
}
bool appconfig::isToolBarVisible()
{
    return m_isToolBarVisible;
}
bool appconfig::isStatusBarVisible()
{
    return m_isStatusBarVisible;
}
bool appconfig::isValid()
{
    return m_isValid;
}
