#ifndef L2DMODELPARAM_H
#define L2DMODELPARAM_H

#include "L2DModel.h"

#include <Source/Common/Debug.h>
#include <Source/Common/FlatWidget.h>

#include <QVector>

enum StandardID
{
    Hit_AreaPreFix,
    Hit_AreaHead,
    Hit_AreaBody,
    Parts_IDCore,
    Parts_ArmPrefix,
    Parts_ArmLPrefix,
    Parts_ArmRPrefix,
    Param_AngleX,
    Param_AngleY,
    Param_AngleZ,
    Param_EyeLOpen,     // ��������Ƥ
    Param_EyeROpen,     // ��������Ƥ
    Param_EyeLSmile,    // ��������Ƥ
    Param_EyeRSmile,    // ��������Ƥ
    Param_EyeBallX,     // ���� X
    Param_EyeBallY,     // ���� Y
    Param_EyeBallForm,  // �������
    Param_BrowLX,       // ��üˮƽ�ƶ�
    Param_BrowLY,       // ��ü��ֱ�ƶ�
    Param_BrowLAngle,   // ��ü��ת�Ƕ�
    Param_BrowLForm,    // ��ü����
    Param_BrowRX,       // ��üˮƽ�ƶ�
    Param_BrowRY,       // ��ü��ֱ�ƶ�
    Param_BrowRAngle,   // ��ü��ת�Ƕ�
    Param_BrowRForm,    // ��ü����
    Param_MouthOpenY,   // �ڿ���
    Param_MouthForm,    // ����
    Param_Smile,
    Param_Teru,         // ����
    Param_BodyAngleX,
    Param_BodyAngleZ,
    Param_Breath,       // ����
    Param_HairFront,    // ����
    Param_HairSide,     // �����ͷ��
    Param_HairBack,     // ������ͷ��
    Param_HairFuwa,
    Param_ShoulderX,
    Param_BustX,        // �ز�ˮƽ����
    Param_BustY,        // �ز���ֱ����
    Param_BaseX,
    Param_BaseY,
    Param_Count
};

static const char* model_param_table[StandardID::Param_Count] = {
    "D_REF",
    "D_REF_HEAD",
    "D_REF_BODY",
    "PARTS_01_CORE",
    "PARTS_01_ARM_",
    "PARTS_01_ARM_L_",
    "PARTS_01_ARM_R_",
    "PARAM_ANGLE_X",
    "PARAM_ANGLE_Y",
    "PARAM_ANGLE_Z",
    "PARAM_EYE_L_OPEN",
    "PARAM_EYE_R_OPEN",
    "PARAM_EYE_L_SMILE",
    "PARAM_EYE_R_SMILE",
    "PARAM_EYE_BALL_X",
    "PARAM_EYE_BALL_Y",
    "PARAM_EYE_BALL_FORM",
    "PARAM_BROW_L_X",
    "PARAM_BROW_L_Y",
    "PARAM_BROW_L_ANGLE",
    "PARAM_BROW_L_FORM",
    "PARAM_BROW_R_X",
    "PARAM_BROW_R_Y",
    "PARAM_BROW_R_ANGLE",
    "PARAM_BROW_R_FORM",
    "PARAM_MOUTH_OPEN_Y",
    "PARAM_MOUTH_FORM",
    "PARAM_SMILE",
    "PARAM_TERE",
    "PARAM_BODY_ANGLE_X",
    "PARAM_BODY_ANGLE_Z",
    "PARAM_BREATH",
    "PARAM_HAIR_FRONT",
    "PARAM_HAIR_SIDE",
    "PARAM_HAIR_BACK",
    "PARAM_HAIR_FUWA",
    "PARAM_SHOULDER_X",
    "PARAM_BUST_X",
    "PARAM_BUST_Y",
    "PARAM_BASE_X",
    "PARAM_BASE_Y"
};

class L2DModel;

//-----------------------------------------------------------------------
// ModelParamBinder
//-----------------------------------------------------------------------
class L2DModelParamBinder : public QObject
{
    Q_OBJECT
public:
    L2DModelParamBinder(StandardID id, QObject* parent = nullptr);

    void SetModel(L2DModel* model);

public slots:
    void ValueChanged(int value)
    {
        float param = value / 100.0f;

        switch (id) {
        case Param_EyeBallX:
        case Param_EyeBallY:
        case Param_BrowLX:
        case Param_BrowLY:
        case Param_BrowLAngle:
        case Param_BrowLForm:
        case Param_BrowRX:
        case Param_BrowRY:
        case Param_BrowRAngle:
        case Param_BrowRForm:
        case Param_MouthForm:
        case Param_Smile:
        case Param_HairFront:
        case Param_HairSide:
        case Param_HairBack:
        case Param_HairFuwa:
        case Param_BustX:
        case Param_BustY:
        case Param_BaseX:
        case Param_BaseY:
            param = param * 1;
            break;
        case Param_EyeLOpen:
        case Param_EyeROpen:
        case Param_MouthOpenY:
        case Param_Teru:
        case Param_Breath:
            param = (param + 1) * 0.5f;
            break;
        case Param_BodyAngleX:
        case Param_BodyAngleZ:
            param = param * 10;
            break;
        case Param_AngleX:
        case Param_AngleY:
        case Param_AngleZ:
            param = param * 30;
            break;
        default:
            param = param * 100;
        }

        this->SetModelParamFloat(param);
    }

    void SetModelParamFloat(float value);

private:
    L2DModel* l2d_model;
    StandardID id;
};


//-----------------------------------------------------------------------
// L2DModelParamWidget
//-----------------------------------------------------------------------
class L2DModelParamWidget : public FlatWidget
{
public:
    L2DModelParamWidget(QWidget* parent = nullptr);

    ~L2DModelParamWidget();

    void SetModel(L2DModel* model);

private:
    L2DModel* l2d_model;
    QVector<L2DModelParamBinder*> vBinders;
};



#endif // L2DMODELPARAM_H
