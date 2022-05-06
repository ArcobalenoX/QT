#include "L2DModelMotion.h"
#include "L2DModel.h"

#include <Source/Common.h>
#include <Source/Common/Debug.h>
#include <Source/Common/PathHelper.h>

L2DModelMotion::L2DModelMotion(L2DModel *model)
    : l2d_model(model)
{
    model_dir_path = l2d_model->GetModelDirPath();

    l2d_eye_blink       = new live2d::framework::L2DEyeBlink();
    drag_manager        = new live2d::framework::L2DTargetPoint();
    main_motion_manager = new live2d::framework::L2DMotionManager();
    expression_manager  = new live2d::framework::L2DMotionManager();

    start_time_ms = live2d::UtSystem::getUserTimeMSec();
    l2d_physics = nullptr;
}

L2DModelMotion::~L2DModelMotion()
{
    delete l2d_eye_blink;
    delete drag_manager;
    delete main_motion_manager;
    delete expression_manager;

    sound_list.clear();

    QMap<QString, MotionData>::Iterator it1;
    for(it1 = motion_map.begin(); it1 != motion_map.end(); ++it1)
    {
        delete it1.value().motion;
    }

    QMap<QString, live2d::AMotion*>::Iterator it;
    for(it = expression_map.begin(); it != expression_map.end(); ++it)
    {
        delete it.value();
    }
}

void L2DModelMotion::Init()
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();

    /* 加载表情数据 */
    if(setting_json->GetExpressionNum() > 0)
    {
        int len = setting_json->GetExpressionNum();
        for(int i = 0; i < len; i++)
        {
            QString name = setting_json->GetExpressionName(i);
            QString file = setting_json->GetExpressionFile(i);
            this->LoadExpression(name, model_dir_path + file);
        }
    }

    this->LoadMotions();
    main_motion_manager->stopAllMotions();
}

void L2DModelMotion::Update()
{
    live2d::ALive2DModel* model = l2d_model->GetLive2DModel();

    /* 执行主动作 */
    model->loadParam();
    if( main_motion_manager->isFinished() )
    {
        this->StartRandomMotion(MOTION_GROUP_IDLE, PRIORITY_IDLE);
    }
    else
    {
        if( main_motion_manager->updateParam(model) == false )
        {
            l2d_eye_blink->setParam(model);
        }
    }
    model->saveParam();

    /* 模型注视光标 */
    drag_manager->update();
    drag_x = drag_manager->getX();
    drag_y = drag_manager->getY();

    model->addToParamFloat( PARAM_ANGLE_X, drag_x *  30 , 1 );
    model->addToParamFloat( PARAM_ANGLE_Y, drag_y *  30 , 1 );
    model->addToParamFloat( PARAM_ANGLE_Z, (drag_x * drag_y) * -30 , 1 );

    model->addToParamFloat( PARAM_BODY_ANGLE_X, drag_x * 10 , 1 );

    model->addToParamFloat( PARAM_EYE_BALL_X, drag_x  , 1 );
    model->addToParamFloat( PARAM_EYE_BALL_Y, drag_y  , 1 );

    /* 自由运动 */
    LDint64	 timeMSec = live2d::UtSystem::getUserTimeMSec() - start_time_ms;
    double t = (timeMSec / 1000.0) * 2 * 3.14159;

    model->addToParamFloat( PARAM_ANGLE_X,       (float) (15 * sin(t / 6.5345)), 0.5f);
    model->addToParamFloat( PARAM_ANGLE_Y,       (float) ( 8 * sin(t / 3.5345)), 0.5f);
    model->addToParamFloat( PARAM_ANGLE_Z,       (float) (10 * sin(t / 5.5345)), 0.5f);
    model->addToParamFloat( PARAM_BODY_ANGLE_X,	 (float) ( 4 * sin(t / 15.5345)), 0.5f);
    model->setParamFloat  ( PARAM_BREATH,        (float) (0.5f + 0.5f * sin(t / 3.2345)), 1);

    /* 表情管理器 */
    if( expression_manager ) expression_manager->updateParam(model);
}


bool L2DModelMotion::LoadExpression(const QString& name, const QString& path)
{
    QString expression_name = PathHelper::GetFileName(path);

    QByteArray data = L2DModel::LoadBytes(path);
    if( data.isNull() )
    {
        Debug::Printf(QString("Can't load expression: %1").arg(expression_name));
        return false;
    }

    live2d::framework::L2DExpressionMotion* motion = live2d::framework::L2DExpressionMotion::loadJson(data.data(), data.size());
    if( L2DDefine::DEBUG_LOG )
    {
        Debug::Printf(QString("%1 - Load expression: %2").arg(l2d_model->GetModelName(), expression_name));
    }

    if( expression_map[name] != nullptr )
    {
        delete expression_map[name];
    }
    expression_map[name] = motion;

    return true;
}

void L2DModelMotion::LoadMotions()
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();
    int group_num = setting_json->GetMotionGroupNum();

    /* 读取每一组 */
    for(int i = 0; i < group_num; i++)
    {
        const char* group_name = setting_json->GetMotionGroupName(i);
        int group_motion_num = setting_json->GetMotionNum(group_name);

        /* 加载组中所有 motion 文件、语音文件 */
        for(int j = 0; j < group_motion_num; j++)
        {
            MotionData motion_data = { nullptr, -1 };

            /* 加载 motion 文件 */
            QString motion_path = setting_json->GetMotionFile(group_name, j);
            motion_data.motion = this->LoadMotion(model_dir_path + motion_path);

            /* 加载语音件 */
            QString sound_path = setting_json->GetMotionSound(group_name, j);
            if( sound_path.compare("") )
            {
                motion_data.sound = sound_list.mediaCount();
                sound_list.addMedia(QUrl::fromLocalFile(model_dir_path + sound_path));
            }
            motion_map.insert(QString("%1_%2").arg(group_name).arg(j), motion_data);
        }
    }
    sound_list.setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    player.setPlaylist(&sound_list);
}

live2d::AMotion* L2DModelMotion::LoadMotion(const QString& path)
{
    QString motion_name = PathHelper::GetFileName(path);

    QByteArray data = L2DModel::LoadBytes(path);
    if( data.isNull() )
    {
        Debug::Printf(QString("Can't load motion: %1").arg(motion_name));
        return nullptr;
    }

    /* 加载 motion 文件 */
    live2d::AMotion* motion = live2d::Live2DMotion::loadMotion(data.data(), data.size());
    if( L2DDefine::DEBUG_LOG )
    {
        Debug::Printf(QString("%1 - Load motion: %2").arg(l2d_model->GetModelName(), motion_name));
    }
    return motion;
}

int L2DModelMotion::StartRandomMotion(const char* name, int priority)
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();

    if( setting_json->GetMotionNum(name) == 0 ) return -1;
    int index = rand() % setting_json->GetMotionNum(name);

    return StartMotion(name, index, priority);
}

int L2DModelMotion::StartMotion(const char* group, int index, int priority)
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();

    if( priority == PRIORITY_FORCE )
    {
        main_motion_manager->setReservePriority(priority);
    }
    else if( main_motion_manager->reserveMotion(priority) == false )
    {
        if( L2DDefine::DEBUG_LOG ) Debug::Printf("Can't start motion.");
        return -1;
    }
    QString key = QString("%1_%2").arg(group).arg(index);

    MotionData motion_data = motion_map[key];
    motion_data.motion->setFadeIn(setting_json->GetMotionFadeIn(group, index));
    motion_data.motion->setFadeOut(setting_json->GetMotionFadeOut(group, index));

    if( L2DDefine::DEBUG_LOG )
    {
        Debug::Printf(QString("%1 - Start motion (%2 : %3)").arg(l2d_model->GetModelName(), group).arg(index));
    }

    if( Global::UserData.OpenSound && motion_data.sound != -1 )
    {
        sound_list.setCurrentIndex(motion_data.sound);
        player.play();
    }
    return main_motion_manager->startMotionPrio(motion_data.motion, false, priority);
}

void L2DModelMotion::SetRandomExpression()
{
    int index = rand() % expression_map.size();

    const char* name = l2d_model->GetModelSettingJson()->GetExpressionName(index);
    this->SetExpression(name);
}

void L2DModelMotion::SetExpression(const char* name)
{
    live2d::AMotion* motion = expression_map[name];
    if( L2DDefine::DEBUG_LOG )
    {
        Debug::Printf(QString("%1 - Start expression [%2]").arg(l2d_model->GetModelName(), name));
    }

    if( motion != nullptr )
    {
        expression_manager->startMotion(motion, false);
    }
    else
    {
        if( L2DDefine::DEBUG_LOG ) Debug::Printf(QString("Expression [%1] is null").arg(name));
    }
}

bool L2DModelMotion::HitTest(const char* pid, float x, float y)
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();

    int len = setting_json->GetHitAreasNum();
    for(int i = 0; i < len; i++)
    {
        if( strcmp(setting_json->GetHitAreaName(i), pid) == 0 )
        {
            const char* draw_id = setting_json->GetHitAreaID(i);
            return HitTestSimple(draw_id, x, y);
        }
    }
    return false;
}

bool L2DModelMotion::HitTestSimple(const char* draw_id, float test_x, float test_y)
{
    int draw_index = l2d_model->GetLive2DModel()->getDrawDataIndex(draw_id);
    if( draw_index < 0 ) return false;

    int count = 0;
    float* points = l2d_model->GetLive2DModel()->getTransformedPoints(draw_index, &count);

    float l = l2d_model->GetLive2DModel()->getCanvasWidth();
    float r = 0;
    float t = l2d_model->GetLive2DModel()->getCanvasHeight();
    float b = 0;

    for(int i = 0; i < count; i++)
    {
        float x = points[live2d::DEF::VERTEX_OFFSET + live2d::DEF::VERTEX_STEP * i + 0];
        float y = points[live2d::DEF::VERTEX_OFFSET + live2d::DEF::VERTEX_STEP * i + 1];

        if( x < l ) l = x;
        if( x > r ) r = x;
        if( y < t ) t = y;
        if( y > b ) b = y;
    }
    float tx = test_x;
    float ty = test_y;

    return (l <= tx && tx <= r && t <= ty && ty <= b);
}

void L2DModelMotion::TapEvent(float x, float y)
{
    L2DModelSettingJson* setting_json = l2d_model->GetModelSettingJson();

    if( HitTest(HIT_AREA_HEAD, x, y) )
    {
        this->SetRandomExpression();
    }
    else if( HitTest(HIT_AREA_BODY, x, y) )
    {
        //this->StartRandomMotion(MOTION_GROUP_TAP_BODY, PRIORITY_NORMAL);

        int count = setting_json->GetMotionGroupNum();
        int index = rand() % count;

        this->StartRandomMotion(setting_json->GetMotionGroupName(index), PRIORITY_NORMAL);
    }
}

void L2DModelMotion::SetDrag(float x, float y)
{
    drag_manager->set(x, y);
}
