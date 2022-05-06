#include "DataManager.h"
#include "PathHelper.h"

/*
** DataSet
** 数据集
*/

DataSet::DataSet()
{

}

DataSet::~DataSet()
{

}

void DataSet::AddValue(const QString &key, const QVariant &value)
{
    data_map[key] = value;
}

QVariant DataSet::GetValue(const QString &key)
{
    QMap<QString, QVariant>::iterator it = data_map.find(key);
    if( it != data_map.end() ) return it.value();

    return QVariant();
}

void DataSet::SaveData(const QString &key, QSettings *settings)
{
    if( data_map.empty() ) return;
    settings->beginGroup(key);

    for(QMap<QString, QVariant>::iterator it = data_map.begin(); it != data_map.end(); ++it){
        settings->setValue(it.key(), it.value());
    }
    settings->endGroup();
}

int DataSet::Size()
{
    return data_map.size();
}

/*
** DataManager
** 数据管理器
**
*/

DataManager::DataManager() : QObject(0)
{
    setting = new QSettings(PathHelper::GetConfigFilePath(), QSettings::IniFormat);
    this->ReadDataSet();
}

DataManager::~DataManager()
{
    this->SaveDataSet();

    QMap<QString, DataSet*>::Iterator it;
    for(it = data_set_map.begin(); it != data_set_map.end(); ++it)
    {
        delete it.value();
    }
}

DataManager *DataManager::GetInstance()
{
    static DataManager dataManager;
    return &dataManager;
}

DataSet *DataManager::GetDataSet(const QString &name)
{
    DataSet *dataset = NULL;
    QMap<QString, DataSet*>::Iterator it = data_set_map.find(name);

    if( it == data_set_map.end() )
    {
        dataset = new DataSet();
        data_set_map.insert(name, dataset);
    }
    else
    {
        dataset = it.value();
    }
    return dataset;
}

void DataManager::SaveDataSet()
{
    QMap<QString, DataSet*>::const_iterator it;
    for(it = data_set_map.constBegin(); it != data_set_map.constEnd(); ++it)
    {
        it.value()->SaveData(it.key(), setting);
    }
}

void DataManager::ReadDataSet()
{
    QStringList groups = setting->childGroups();

    /* 读取数据并创建数据集 */
    foreach(QString group, groups)
    {
        DataSet *ds = new DataSet;

        /* 读取每一组数据 */
        setting->beginGroup(group);
        QStringList keys = setting->childKeys();
        foreach(QString key, keys)
        {
            ds->AddValue(key, setting->value(key));
        }
        setting->endGroup();

        data_set_map.insert(group, ds);
    }
}

