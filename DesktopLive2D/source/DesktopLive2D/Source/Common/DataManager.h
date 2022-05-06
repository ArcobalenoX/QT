#ifndef DATAMANAGER_H
#define DATAMANAGER_H

/******************************************************
** 数据管理类
** 程序上所有配置信息数据都通过这个对象来管理
**
*******************************************************/

#include <QMap>
#include <QObject>
#include <QSettings>

/*
** DataSet
** 数据集
*/

class DataSet
{
public:
    DataSet();
    ~DataSet();

public:
    QVariant GetValue(const QString &key);

    void AddValue(const QString &key, const QVariant &value);

    int Size();

private:
    friend class DataManager;

    void SaveData(const QString &key, QSettings *settings);

    QMap<QString, QVariant> data_map;
};


/*
** DataManager
** 数据管理器，使程序退出后能够保存必要的数据
*/

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager *GetInstance();

    void SaveDataSet();

    DataSet *GetDataSet(const QString &name);

    void Update() { emit sigUpdate(); }

signals:
    void sigUpdate();

private:
    void ReadDataSet();

private:
    DataManager();
    ~DataManager();

    QMap<QString, DataSet*> data_set_map;
    QSettings *setting;
    QString save_path;
};

#endif // DATAMANAGER_H
