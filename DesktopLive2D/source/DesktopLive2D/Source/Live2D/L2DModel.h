#ifndef LIVE2DMODEL_H
#define LIVE2DMODEL_H

#include <QObject>
#include <QOpenGLTexture>

#include "L2DDefine.h"
#include "L2DModelParam.h"
#include "L2DModelSettingJson.h"

#include "../External/Live2D/Live2DModelWinGL.h"
#include "../Framework/L2DPose.h"

class L2DModelMotion;
class L2DModelController;

//----------------------------------------------------------------------------
// L2DModel
//----------------------------------------------------------------------------
class L2DModel : public QObject
{
    Q_OBJECT
public:
    explicit L2DModel(QObject *parent = 0);

    ~L2DModel();

    void Update();

    void Draw(float* matrix);

    void SetParamFloat(const char* id, float value, float weight = 1.0f);

    void CreateModelMotion();

    void LoadModel(const QString& model_file);

    void LoadPose(const QString& path);

    void TapEvent(float x, float y);

    void SetDrag(float x, float y);

    void SetModelController(L2DModelController* controller);

    QString GetModelName() { return model_path; }

    QString GetModelDirPath() { return model_dir_path; }

    live2d::ALive2DModel* GetLive2DModel() const { return live2d_model; }

    L2DModelSettingJson*  GetModelSettingJson() { return l2d_model_setting; }

    int GetCanvasWidth() const { return live2d_model->getCanvasWidth(); }

    int GetCanvasHeight() const { return live2d_model->getCanvasHeight(); }

    static QByteArray LoadBytes(const QString& path);

protected:
    L2DModelSettingJson* CreateModelSettingJson(const QString& model_file);

    void InitModel(const L2DModelSettingJson* json_file);

protected:
    L2DModelSettingJson* l2d_model_setting;
    live2d::Live2DModelWinGL* live2d_model;
    live2d::framework::L2DPose* l2d_model_pose;

    QVector<QOpenGLTexture*> textures;

    L2DModelMotion* l2d_model_motion;
    L2DModelController* l2d_model_controller;

    QString model_home_path;
    QString model_dir_path;
    QString model_path;
};


#endif // LIVE2DMODEL_H
