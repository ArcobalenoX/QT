#include "L2DModel.h"
#include "L2DModelMotion.h"
#include "L2DModelController.h"

#include <Source/Common/Debug.h>
#include <Source/Common/PathHelper.h>

#include <QFile>
#include <QOpenGLFunctions>

#define CSTR(str_oj) (str_oj).toStdString().c_str()

//----------------------------------------------------------------------------
// L2DModel
//----------------------------------------------------------------------------
L2DModel::L2DModel(QObject *parent)
    : QObject(parent)
    , live2d_model(nullptr)
    , l2d_model_setting(nullptr)
    , l2d_model_pose(nullptr)
    , l2d_model_motion(nullptr)
    , l2d_model_controller(nullptr)
    , model_dir_path("")
    , model_home_path("")
    , model_path("")
{
    model_home_path = PathHelper::GetModelDirPath();
}

L2DModel::~L2DModel()
{
    if( l2d_model_pose ) delete l2d_model_pose;
    if( l2d_model_motion ) delete l2d_model_motion;
    if( l2d_model_setting ) delete l2d_model_setting;

    foreach(QOpenGLTexture* tex, textures) delete tex;

    if( L2DDefine::DEBUG_LOG ) Debug::Printf(QString("Delete Model: %1").arg(model_path));
}

void L2DModel::LoadModel(const QString& model_file)
{
    /* 创建模型设置文件  */
    l2d_model_setting = this->CreateModelSettingJson(model_file);

    /* 初始化模型（模型数据、纹理） */
    this->InitModel(l2d_model_setting);
}

void L2DModel::CreateModelMotion()
{
    if( l2d_model_motion ) return;

    l2d_model_motion = new L2DModelMotion(this);
    l2d_model_motion->Init();
}

L2DModelSettingJson* L2DModel::CreateModelSettingJson(const QString& model_file)
{
    /* model_file: model_name/model_name.model.json */
    QString path = PathHelper::Normalize(model_file);

    QString model_dir = path.split("/")[0];
    QString model_json = path.split("/")[1];
    model_dir_path = model_home_path + model_dir + "/";

    QByteArray data = this->LoadBytes(model_dir_path + model_json);
    if( data.isNull() )
    {
        Debug::Printf(QString("%1 is null.").arg(model_json));
        return nullptr;
    }

    /* 解析 model json 文件 */
    L2DModelSettingJson* model_setting = new L2DModelSettingJson(data.data(), data.size());
    model_path = model_setting->GetModelFile();

    return model_setting;
}

void L2DModel::InitModel(const L2DModelSettingJson* json_file)
{
    /* 加载模型文件 */
    if( model_path.compare("") != 0 )
    {
        if(Debug::DEBUG_LIVE2D) Debug::Printf(QString("Load model: %1").arg(model_path));

        QString name = model_dir_path + model_path;

        /* 加载模型数据 */
        live2d_model = live2d::Live2DModelWinGL::loadModel(CSTR(name));
        live2d_model->saveParam();

        /* 加载模型纹理 */
        int texture_count = l2d_model_setting->GetTextureNum();
        for(int i = 0; i < texture_count; i++)
        {
            QString texture_name = model_dir_path + l2d_model_setting->GetTextureFile(i);

            QOpenGLTexture* tex = new QOpenGLTexture(QImage(texture_name));
            tex->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::LinearMipMapLinear);
            textures.push_back(tex);

            live2d_model->setTexture(i, tex->textureId());
        }
        live2d_model->setPremultipliedAlpha(false);
    }

    /* 加载 Pose */
    if(strcmp(l2d_model_setting->GetPoseFile(), "") != 0)
    {
        QString path = l2d_model_setting->GetPoseFile();
        this->LoadPose(model_dir_path + path);
    }
}

void L2DModel::LoadPose(const QString& path)
{
    QString name = PathHelper::GetFileName(path);

    QByteArray data = this->LoadBytes(path);
    if( data.isNull() ) Debug::Printf(QString("Can't load model pose file: %1").arg(name));

    l2d_model_pose = live2d::framework::L2DPose::load(data.data(), data.size());
    if( L2DDefine::DEBUG_LOG ) Debug::Printf(QString("%1 - Load pose file: %2").arg(model_path, name));
}

void L2DModel::Update()
{
    if( l2d_model_motion && l2d_model_controller == nullptr ) l2d_model_motion->Update();
    if( l2d_model_pose ) l2d_model_pose->updateParam(live2d_model);

    live2d_model->update();
}

void L2DModel::Draw(float* matrix)
{
    live2d_model->setMatrix(matrix);
    live2d_model->draw();
}

void L2DModel::SetModelController(L2DModelController* controller)
{
    l2d_model_controller = controller;
    l2d_model_controller->SetModel(this);
}

void L2DModel::SetParamFloat(const char* id, float value, float weight)
{
    live2d_model->setParamFloat(id, value, weight);
}

void L2DModel::TapEvent(float x, float y)
{
    if( l2d_model_motion ) l2d_model_motion->TapEvent(x, y);
}

void L2DModel::SetDrag(float x, float y)
{
    if( l2d_model_motion ) l2d_model_motion->SetDrag(x, y);
}

QByteArray L2DModel::LoadBytes(const QString& path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly) == false)
    {
        Debug::Printf(QString("Can't open file: %1").arg(path));
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}
