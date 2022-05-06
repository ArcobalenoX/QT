#ifndef L2DMODELMOTION_H
#define L2DMODELMOTION_H

#include <QMap>
#include <QVector>
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QOpenGLTexture>

#include "../External/Live2D/motion/AMotion.h"
#include "../External/Live2D/Live2DModelWinGL.h"
#include "../External/Live2D/motion/Live2DMotion.h"

#include "../Framework/L2DPhysics.h"
#include "../Framework/L2DEyeBlink.h"
#include "../Framework/L2DTargetPoint.h"
#include "../Framework/L2DMotionManager.h"
#include "../Framework/L2DExpressionMotion.h"

#include "L2DDefine.h"
#include "L2DModelParam.h"
#include "L2DModelSettingJson.h"

class L2DModel;

class L2DModelMotion
{
public:
    explicit L2DModelMotion(L2DModel* model);

    ~L2DModelMotion();

    void Update();

    void Init();

    void TapEvent(float x, float y);

    void SetDrag(float x, float y);

    bool HitTest(const char* pid, float x, float y);

private:
    bool LoadExpression(const QString& name, const QString& path);

    void LoadMotions();

    live2d::AMotion* LoadMotion(const QString& path);

    int StartRandomMotion(const char* name, int priority);

    int StartMotion(const char* group, int index, int priority);

    void SetRandomExpression();

    void SetExpression(const char* name);

    bool HitTestSimple(const char* draw_id, float test_x, float test_y);

private:
    L2DModel* l2d_model;
    QString model_dir_path;

    QMediaPlayer player;

    struct MotionData
    {
        live2d::AMotion* motion;
        int sound;
    };

    QMediaPlaylist sound_list;
    QMap<QString, MotionData> motion_map;
    QMap<QString, live2d::AMotion*> expression_map;

    live2d::framework::L2DPhysics*       l2d_physics;
    live2d::framework::L2DEyeBlink*      l2d_eye_blink;
    live2d::framework::L2DTargetPoint*   drag_manager;
    live2d::framework::L2DMotionManager* main_motion_manager;
    live2d::framework::L2DMotionManager* expression_manager;

    quint64 start_time_ms;

    float drag_x;
    float drag_y;
};

#endif // L2DMODELMOTION_H
