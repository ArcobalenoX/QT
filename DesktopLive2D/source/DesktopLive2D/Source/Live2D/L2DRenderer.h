#ifndef L2DRENDERER_H
#define L2DRENDERER_H

#include <QObject>
#include <QMatrix4x4>

#include "../Framework/L2DMatrix44.h"


#include "L2DModel.h"

class L2DRenderer
{
public:
    L2DRenderer();

    void SetModel(L2DModel* model);

    void Draw();

    void ScaleView(float cx, float cy, float scale);

    void TranslateView(float x, float y);

    void MousePress(int x, int y);

    void MouseDrag(int x, int y);

    void MouseWheel(int delta, int x, int y);

    void UpdateViewMatrix(float dx, float dy, float cx, float cy, float scale);

    void SetWindowSize(int width, int height);

    float TransformDeviceToViewX(float device_x);

    float TransformDeviceToViewY(float device_y);

private:
    L2DModel* l2d_model;

    QMatrix4x4 mViewMatrix;
    live2d::framework::L2DMatrix44 mDeviceToScreen;
};

#endif // L2DRENDERER_H
