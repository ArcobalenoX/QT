#include "L2DModelController.h"
#include "L2DModel.h"

#include <Source/Common/Debug.h>

L2DModelController::L2DModelController(QObject *parent)
    : QObject(parent)
    , l2d_modelParamWidget(nullptr)
{

}

L2DModelController::~L2DModelController()
{

}

void L2DModelController::SetControlWidget(L2DModelParamWidget* widget)
{
    l2d_modelParamWidget = widget;
    if( l2d_modelParamWidget ) l2d_modelParamWidget->SetModel(l2d_model);
}

void L2DModelController::SetModel(L2DModel* model)
{
    l2d_model = model;
    if( l2d_modelParamWidget ) l2d_modelParamWidget->SetModel(l2d_model);
}

void L2DModelController::SetHeadAngleX(float value)
{
    l2d_model->SetParamFloat(model_param_table[StandardID::Param_AngleX], value);
}

void L2DModelController::SetHeadAngleY(float value)
{
    l2d_model->SetParamFloat(model_param_table[StandardID::Param_AngleY], value);
}

void L2DModelController::SetHeadAngleZ(float value)
{
    l2d_model->SetParamFloat(model_param_table[StandardID::Param_AngleZ], value);
}
