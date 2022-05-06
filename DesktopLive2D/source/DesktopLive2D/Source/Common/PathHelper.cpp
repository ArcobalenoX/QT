#include "PathHelper.h"

#include <QDir>
#include <QCoreApplication>

#include "Debug.h"
#include <Source/Common.h>

static PathHelper* ph = nullptr;

PathHelper::PathHelper()
{
    ph = this;

#if defined(DEPLOY) && defined(QT_NO_DEBUG)
    application_home_path = QCoreApplication::applicationDirPath();
#else
    application_home_path = QDir::currentPath();
#endif

    icons_dir_name    = "Icons";
    xfdata_dir_name   = "XFData";
    model_dir_name    = "Live2D";
    sound_dir_name    = "Sound";
    resource_dir_name = "Resources";

    config_file_name  = "Config.ini";
}

QString PathHelper::GetConfigFilePath()
{
    return ph->application_home_path + "/" + ph->config_file_name;
}

QString PathHelper::GetResourcePath()
{
    return ph->application_home_path + "/" + ph->resource_dir_name + "/";
}

QString PathHelper::GetIconDirPath()
{
    return GetResourcePath() + ph->icons_dir_name + "/";
}

QString PathHelper::GetXFDataDirPath()
{
    return GetResourcePath() + ph->xfdata_dir_name + "/";
}

QString PathHelper::GetModelDirPath()
{
    return GetResourcePath() + ph->model_dir_name + "/";
}

QString PathHelper::GetSoundDirPath()
{
    return GetResourcePath() + ph->sound_dir_name + "/";
}

QString PathHelper::GetStyleSheetPath()
{
    return ph->application_home_path + "/";
}

QString PathHelper::Normalize(const QString& string)
{
    QString tmp = string;
    return tmp.replace("\\", "/");
}

QString PathHelper::GetFileName(const QString& name)
{
    QStringList list = name.split("/");
    return list[list.size() - 1];
}
