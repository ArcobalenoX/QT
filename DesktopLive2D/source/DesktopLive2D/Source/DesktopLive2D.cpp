#include "DesktopLive2D.h"

#include <QDir>
#include <QImage>
#include <QFileInfoList>

#include <Source/Common.h>
#include <Source/Common/PathHelper.h>
#include <Source/Common/DataManager.h>
#include <Source/Dialog/FMessageBox.h>

DesktopLive2D::DesktopLive2D(QObject *parent)
    : QObject(parent)
    , l2d_model(nullptr)
    , model_preview(nullptr)
    , l2d_render_window(nullptr)
    , model_selected_widget(nullptr)
    , current_model_index(-1)
{

}

DesktopLive2D::~DesktopLive2D()
{
    if( model_info_list.isEmpty() == false )
    {
        delete model_preview;
        delete l2d_render_window;
        live2d::Live2D::dispose();
    }
}

bool DesktopLive2D::Init()
{
    /* 加载所有模型的信息 */
    is_update_model_cover = this->LoadModelsInfomation(model_info_list);

    /* 不存在模型数据？ */
    if( model_info_list.isEmpty() )
    {
        FMessageBox msg_box(QString::fromLocal8Bit("提示"),
                            QString::fromLocal8Bit("没有可加载的模型数据\n将退出程序!"));
        msg_box.resize(200, 150);
        msg_box.Exec();
        return false;
    }

    /* 初始化 live2d */
    live2d::Live2D::init();

    /* 创建 live2d 渲染窗口 */
    l2d_render_window = new L2DRenderGLWidget();
    l2d_render_window->move(Global::UserData.ModelWindowPos);
    l2d_render_window->resize(Global::UserData.ModelWindowSize);
    l2d_render_window->show();

    model_preview = new QPixmap;

    /* 设置模型窗口刷新 fps */
    this->slotSetModelFPS(Global::UserData.FPS);

    /* 加载指定模型，不存在指定的模型则加载第一个模型 */
    int index = 0;

    for(int i = 0; i < model_info_list.size(); i++)
    {
        if( model_info_list[i].Name.compare(Global::UserData.CurrentModelName) == 0 )
        {
            index = i; break;
        }
    }
    this->SetModel(index);

    return true;
}

bool DesktopLive2D::LoadModelsInfomation(ModelInfoVector &info_list)
{
    QString root_path = PathHelper::GetModelDirPath();

    /* 遍历存放模型的文件夹 */
    QDir dir(root_path);
    QStringList list = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

    bool load_cover = false;
    foreach(QString name, list)
    {
        ModelInfo model_info;

        /* 获取模型文件夹名称 */
        model_info.Name = name;
        model_info.ImagePath = root_path + name + ".png";
        model_info.HasCover  = QFile::exists(model_info.ImagePath);

        if( model_info.HasCover == false )
        {
            load_cover = true;
        }
        info_list << model_info;
    }
    return load_cover;
}

void DesktopLive2D::CreateModelsCover()
{
    l2d_render_window->BeginGetModelCover();

    /* 渲染模型，获取模型封面 */
    for(int i = 0; i < model_info_list.size(); i++ )
    {
        ModelInfo* info = &model_info_list[i];
        if( info->HasCover == false )
        {
            L2DModel* model = this->CreateModel(info->Name, false);
            l2d_render_window->SetModel(model);
            l2d_render_window->repaint();

            QImage image = l2d_render_window->GetModelCover();
            image.save(info->ImagePath);

            info->HasCover = true;
            delete model;
        }
    }
    l2d_render_window->SetModel(nullptr);
    l2d_render_window->EndGetModelCover();

    /* 恢复原来的模型 */
    l2d_render_window->SetModel(l2d_model);
}

L2DModel* DesktopLive2D::CreateModel(const QString& name, bool create_motion)
{
    L2DModel* model = new L2DModel;
    model->LoadModel(QString("%1/%2.model.json").arg(name, name));

    if( create_motion )
    {
        model->CreateModelMotion();
    }
    return model;
}

void DesktopLive2D::SetModel(int index)
{
    /* 不能设置相同的模型 */
    if( current_model_index == index ) return;

    /* 删除原来的模型 */
    if( l2d_model )
    {
        l2d_render_window->SetModel(nullptr);
        delete l2d_model;
    }

    /* 创建新模型 */
    current_model_index = index;
    l2d_model = this->CreateModel(model_info_list[current_model_index].Name, true);
    model_preview->load(model_info_list[current_model_index].ImagePath);

    /* 设置模型到渲染窗口 */
    l2d_render_window->SetModel(l2d_model);
    l2d_render_window->update();
}

void DesktopLive2D::slotSetModelFPS(int level)
{
    int level_table[3] = { 30, 48, 60 };
    l2d_render_window->SetModelUpdateFPS(level_table[level]);
}

void DesktopLive2D::slotChangedModel(int index)
{
    if( current_model_index == index ) return;

    /* 模型渲染窗口加载新的模型 */
    this->SetModel(index);

    /* 记录用户当前选择的模型 */
    Global::UserData.CurrentModelName = model_info_list[index].Name;
    DataManager::GetInstance()->Update();

    emit sigChangedModel();
}

void DesktopLive2D::slotAddNewModelDatas(const QStringList &path_list)
{
    /* 添加新的模型的信息并创建封面 */
    QDir directory;
    for(int i = 0; i < path_list.size(); i++)
    {
        directory.setPath(path_list[i]);

        ModelInfo model_info;
        model_info.Name      = directory.dirName();
        model_info.ImagePath = PathHelper::GetModelDirPath() + model_info.Name + ".png";
        model_info.HasCover  = false;
        model_info.Pixmap    = nullptr;

        model_info_list << model_info;
    }
    this->CreateModelsCover();

    /* 更新模型选择对话框 */
    model_selected_widget->ClearData();
    for(int i = 0; i < model_info_list.size(); i++)
    {
        if( model_info_list[i].Pixmap == nullptr )
        {
            model_info_list[i].Pixmap = new QPixmap(model_info_list[i].ImagePath);
        }
        model_selected_widget->AppendData(model_info_list[i].Name, model_info_list[i].Pixmap);
    }
    model_selected_widget->update();
}

void DesktopLive2D::slotOpenModelSelectWidget()
{
    /* 为模型创建封面图片 */
    if( is_update_model_cover )
    {
        is_update_model_cover = false;

        QString title = QString::fromLocal8Bit("模型加载");
        QString text  = QString::fromLocal8Bit("存在模型缺少封面，是否为这些模型创建封面？");

        FMessageBox msgbox(title, text, FMB_YesCancel);
        if( msgbox.Exec() ){
            this->CreateModelsCover();
        }
    }

    /* 创建模型选择窗口 */
    if( model_selected_widget == nullptr ){
        model_selected_widget = new FModelSelectedWidget();
        model_selected_widget->SetDeleteAfterClose(true);

        connect(model_selected_widget, SIGNAL(sigModelSelected(int)), this, SLOT(slotChangedModel(int)));
        connect(model_selected_widget, SIGNAL(sigNewModels(QStringList)), this, SLOT(slotAddNewModelDatas(QStringList)));

        /* 摧毁窗口时将指针置空，等待下一次创建 */
        connect(model_selected_widget, &FModelSelectedWidget::destroyed, this, [=](QObject*){
            model_selected_widget = nullptr;

            /* 退出对话框时释放图像资源 */
            for(int i = 0; i < model_info_list.size(); i++)
            {
                delete model_info_list[i].Pixmap;
                model_info_list[i].Pixmap = nullptr;
            }
        });
    }

    if( model_selected_widget->isHidden() == false ) return;

    for(int i = 0; i < model_info_list.size(); i++)
    {
        model_info_list[i].Pixmap = new QPixmap(model_info_list[i].ImagePath);
        model_selected_widget->AppendData(model_info_list[i].Name, model_info_list[i].Pixmap);
    }
    model_selected_widget->show();
}
