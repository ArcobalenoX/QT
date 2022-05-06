#ifndef PATHHELPER_H
#define PATHHELPER_H

#include <QObject>

class PathHelper
{
public:
    PathHelper();

    static QString GetConfigFilePath();

    static QString GetIconDirPath();

    static QString GetXFDataDirPath();

    static QString GetResourcePath();

    static QString GetModelDirPath();

    static QString GetSoundDirPath();

    static QString GetStyleSheetPath();

    static QString Normalize(const QString& string);

    static QString GetFileName(const QString &name);

private:
    QString application_home_path;

    QString config_file_name;

    QString resource_dir_name;
    QString icons_dir_name;
    QString xfdata_dir_name;
    QString model_dir_name;
    QString sound_dir_name;
};

#endif // PATHHELPER_H
