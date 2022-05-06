#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <QSize>

#define MODEL_WINDOW_SIZE 300

//#define DEPLOY

struct UserData_
{
    /* È«¾Ö */
    bool RunAfterStartUp;

    int FPS;
    QColor BackgroundColor;
    bool HoverBackground;
    bool DisplayBackground;
    bool OpenSound;
    bool ResizeModelWindow;
    bool WindowStaysOnTop;

    QSize ModelWindowSize;
    QPoint ModelWindowPos;

    QString CurrentModelName;

    /* XF */
    bool XFEnable;
    bool XFMovable;
    bool XFHideInSide;

    QPoint XFViewPoint;
};


class Global
{
public:
    static UserData_ UserData;
};

#endif // COMMON_H
