#ifndef STYLESHEETMANAGER_H
#define STYLESHEETMANAGER_H

#include <QObject>

class StyleSheetManager
{
public:
    explicit StyleSheetManager();

    static QString GetMenuStyle();

    static QString GetLabelStyle();

private:
    static QString normal_color;
    static QString selected_color;
    static QString background_color;
};

#endif // STYLESHEETMANAGER_H
