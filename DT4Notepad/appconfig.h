#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFont>
#include <QSize>
#include <QPoint>
class appconfig : public QObject
{
    Q_OBJECT
    QFont m_editorFont;
    QSize m_mainWindowSize;
    QPoint m_mainWindowPoint;
    bool m_isAutoWrap;
    bool m_isToolBarVisible;
    bool m_isStatusBarVisible;
    bool m_isValid;

    bool restore();

public:
    explicit appconfig(QObject *parent = nullptr);
    explicit appconfig(QFont font, QPoint point, QSize size, bool isWrap, bool tbisvisible, bool sbisvisible, QObject* parent =nullptr);
    bool store();
    QFont editorFont();
    QSize mainWindowSize();
    QPoint mainWindowPoint();
    bool isAutoWrap();
    bool isToolBarVisible();
    bool isStatusBarVisible();
    bool isValid();

signals:

};

#endif // APPCONFIG_H
