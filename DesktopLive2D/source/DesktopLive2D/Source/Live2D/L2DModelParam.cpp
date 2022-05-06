#include "L2DModelParam.h"

#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QScrollArea>


//-----------------------------------------------------------------------
// ModelParamBinder
//-----------------------------------------------------------------------
L2DModelParamBinder::L2DModelParamBinder(StandardID id, QObject *parent)
    : QObject(parent)
    , id(id)
{

}


void L2DModelParamBinder::SetModelParamFloat(float value)
{
    l2d_model->SetParamFloat(model_param_table[id], value);
}

void L2DModelParamBinder::SetModel(L2DModel* model)
{
    l2d_model = model;
}

//-----------------------------------------------------------------------
// L2DModelParamWidget
//-----------------------------------------------------------------------
L2DModelParamWidget::L2DModelParamWidget(QWidget* parent)
    : FlatWidget(parent)
    , l2d_model(nullptr)
{
    this->resize(400, 650);
    this->SetWindowTitle(QString::fromLocal8Bit("模型动作参数设置面板"));

    int padding = 5;
    int scroll_w = center_widget->width() - padding * 2;
    int scroll_h = center_widget->height() - padding * 2;

    QWidget* background = new QWidget();
    background->resize(scroll_w - 20, 1150);

    QGridLayout* layout = new QGridLayout;
    background->setLayout(layout);

    for(int i = 0; i < StandardID::Param_Count; i++)
    {
        QLabel* label = new QLabel(model_param_table[i]);
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setRange(-100, 100);

        layout->addWidget(label, i, 0);
        layout->addWidget(slider, i, 1);

        bool full = false;
        full |= (i == StandardID::Param_EyeLOpen);
        full |= (i == StandardID::Param_EyeROpen);
        if( full ) slider->setValue(100);

        full = false;
        full |= (i == StandardID::Param_MouthOpenY);
        full |= (i == StandardID::Param_Teru);
        full |= (i == StandardID::Param_Breath);
        if( full ) slider->setValue(-100);

        L2DModelParamBinder* binder = new L2DModelParamBinder((StandardID) i );
        connect(slider, SIGNAL(valueChanged(int)), binder, SLOT(ValueChanged(int)));

        vBinders.push_back(binder);
    }

    QScrollArea* scroll_area = new QScrollArea(center_widget);
    scroll_area->setWidget(background);
    scroll_area->move(padding, padding);
    scroll_area->resize(scroll_w, scroll_h);
}

L2DModelParamWidget::~L2DModelParamWidget()
{
    for(int i = 0; i < vBinders.size(); i++)
    {
        delete vBinders[i];
    }
    vBinders.clear();
}

void L2DModelParamWidget::SetModel(L2DModel* model)
{
    l2d_model = model;

    for(int i = 0; i < vBinders.size(); i++)
    {
        vBinders[i]->SetModel(l2d_model);
    }
}












