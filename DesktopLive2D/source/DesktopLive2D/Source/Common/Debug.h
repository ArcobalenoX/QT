#ifndef DEBUG_H
#define DEBUG_H

#include <QObject>
#include <QPoint>
#include <QSize>

#include <stdio.h>
#include <stdarg.h>

class Debug
{
public:
    static const bool DEBUG_LIVE2D = true;

    static const bool DownloadDebugInfo = true;

    static void FormatPrintf(const char* format, ...);

    static void Printf(const QString& string);

    static void Printf(const char* string);

    static void Printf(const QPoint& point);

    static void Printf(const QSize& size);

    static void Printf(bool b);

    static void Printf(int value);

    static void Printf(float value);

    static void Printf(double value);
};


#endif // DEBUG_H
