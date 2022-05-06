#ifndef DATAMANAGER_H
#define DATAMANAGER_H

/******************************************************
** ���ݹ�����
** ����������������Ϣ���ݶ�ͨ���������������
**
*******************************************************/

#include <QMap>
#include <QObject>
#include <QSettings>

/*
** DataSet
** ���ݼ�
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
** ���ݹ�������ʹ�����˳����ܹ������Ҫ������
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
