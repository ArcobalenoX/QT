#include "L2DRenderer.h"

static const float VIEW_MAX_SCALE = 4.0f;
static const float VIEW_MIN_SCALE = 0.5f;

L2DRenderer::L2DRenderer() : l2d_model(nullptr)
{

}

void L2DRenderer::SetModel(L2DModel *model)
{
    l2d_model = model;
}

void L2DRenderer::Draw()
{
    if( l2d_model )
    {
        l2d_model->Update();
        l2d_model->Draw(mViewMatrix.data());
    }
}

void L2DRenderer::SetWindowSize(int width, int height)
{
    if( l2d_model )
    {
        float screen_w = 2.0f;

        /* 设置变换矩阵，dst = src * scale - offset
         * 将坐标从窗口坐标空间(0, w, 0, h)变换到 Live2D 使用的坐标空间(-1, 1, -1, 1)
         */
        mDeviceToScreen.identity();
        mDeviceToScreen.multTranslate(-width / 2.0f, -height / 2.0f);
        mDeviceToScreen.multScale(screen_w / width, -screen_w / width);

        float ratio = (float) width / height;

        /* 平移到左上角（窗口原点），OpenGL 默认情况下图像会上下翻转，为了正常显示 Live2D 的内容，需要翻转回来 */
        mViewMatrix.setToIdentity();
        mViewMatrix.translate(-1, 1);
        mViewMatrix.scale(2.0 / l2d_model->GetCanvasWidth(), -2.0 / l2d_model->GetCanvasHeight() * ratio);
    }
}

void L2DRenderer::MousePress(int x, int y)
{
    float vx = TransformDeviceToViewX(x);
    float vy = TransformDeviceToViewY(y);

    if( l2d_model ) l2d_model->TapEvent(vx, vy);
}

void L2DRenderer::MouseDrag(int x, int y)
{
    float vx = mDeviceToScreen.transformX(x);
    float vy = mDeviceToScreen.transformY(y);

    if( l2d_model ) l2d_model->SetDrag(vx, vy);
}

void L2DRenderer::MouseWheel(int delta, int x, int y)
{
    float x_on_screen = mDeviceToScreen.transformX(x);
    float y_on_screen = mDeviceToScreen.transformX(y);

    float scale = delta < 0 ? 1.0f / 1.4142f : 1.4142f;
    this->UpdateViewMatrix(0, 0, x_on_screen, y_on_screen, scale);
}

void L2DRenderer::UpdateViewMatrix(float dx, float dy, float cx, float cy, float scale)
{
    this->ScaleView(cx, cy, scale);

    this->TranslateView(dx, dy);
}

void L2DRenderer::ScaleView(float cx, float cy, float scale)
{
    if( l2d_model )
    {
        float base_scale = 2.0 / l2d_model->GetCanvasWidth();
        float max_scale = base_scale * VIEW_MAX_SCALE;
        float min_scale = base_scale * VIEW_MIN_SCALE;

        float src_scale = mViewMatrix.data()[0];
        float dst_scale = scale * src_scale;

        /* 将模型缩放比限制在 [0.5f, 4.0f] 间 */
        if(dst_scale < min_scale)
        {
            if(src_scale > 0) scale = min_scale / src_scale;
        }
        else if(dst_scale > max_scale)
        {
            if(src_scale > 0) scale = max_scale / src_scale;
        }

        /* 以光标坐标为锚点进行缩放 */
        QMatrix4x4 m1, m2, m3;
        m1.translate(cx, -cy);
        m2.scale(scale);
        m3.translate(-cx, cy);

        mViewMatrix = m1 * m2 * m3 * mViewMatrix;
    }
}

void L2DRenderer::TranslateView(float x, float y)
{
    QMatrix4x4 mat;
    mat.translate(x, y);

    mViewMatrix = mViewMatrix * mat;
}

float L2DRenderer::TransformDeviceToViewX(float device_x)
{
    float screen_x = mDeviceToScreen.transformX(device_x);
    return (screen_x - mViewMatrix.data()[12]) / mViewMatrix.data()[0];
}

float L2DRenderer::TransformDeviceToViewY(float device_y)
{
    float screen_y = mDeviceToScreen.transformY(device_y);
    return (screen_y - mViewMatrix.data()[13]) / mViewMatrix.data()[5];
}
