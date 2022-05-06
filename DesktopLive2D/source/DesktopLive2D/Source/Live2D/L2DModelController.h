#ifndef L2DMODELCONTROLLER_H
#define L2DMODELCONTROLLER_H

#include <QObject>

#include "L2DModelParam.h"

class L2DModel;

class L2DModelController : public QObject
{
    Q_OBJECT
public:
    explicit L2DModelController(QObject *parent = 0);

    ~L2DModelController();

    /* 为控制器添加模型参数设置的可视化窗口，及可见即所得设置 */
    void SetControlWidget(L2DModelParamWidget* widget);

    void SetModel(L2DModel* model);

    void SetHeadAngleX(float value);

    void SetHeadAngleY(float value);

    void SetHeadAngleZ(float value);

private:
    L2DModel* l2d_model;
    L2DModelParamWidget* l2d_modelParamWidget;
};

#endif // L2DMODELCONTROLLER_H
