#include "StyleSheetManager.h"

QString StyleSheetManager::normal_color = "#2ecc71";
QString StyleSheetManager::selected_color = "#27ae60";
QString StyleSheetManager::background_color = "#34495e";


StyleSheetManager::StyleSheetManager()
{

}

QString StyleSheetManager::GetMenuStyle()
{
    static QString style = QString::fromLocal8Bit(
                "QMenu{ font-size: 14px; font-family: Î¢ÈíÑÅºÚ; color: white; background-color: %1 }"
                "QMenu::item:selected{ background-color: %2 }"
                );
    return style.arg(background_color, normal_color);
}

QString StyleSheetManager::GetLabelStyle()
{
    static QString style = "QLabel{ border: 1px solid %1 }";
    return style.arg(normal_color);
}
