#include "Debug.h"

void Debug::FormatPrintf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    static char buffer[512];
    vsprintf(buffer, format, ap);

    Debug::Printf(buffer);

    va_end(ap);
}

void Debug::Printf(const char* string)
{
    qDebug(string);
}

void Debug::Printf(const QString& string)
{
    qDebug(string.toStdString().c_str());
}

void Debug::Printf(const QPoint& point)
{
    qDebug("x: %d - y: %d", point.x(), point.y());
}

void Debug::Printf(const QSize& size)
{
    qDebug("w: %d - h: %d", size.width(), size.height());
}

void Debug::Printf(bool b)
{
    qDebug("%s", b ? "true" : "false");
}

void Debug::Printf(int value)
{
    qDebug("%d", value);
}

void Debug::Printf(float value)
{
    qDebug("%f", value);
}

void Debug::Printf(double value)
{
    qDebug("%f", value);
}
