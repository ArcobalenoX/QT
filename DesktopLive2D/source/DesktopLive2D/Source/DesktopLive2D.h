#ifndef DESKTOPLIVE2D_H
#define DESKTOPLIVE2D_H

#include <QObject>
#include <QVector>

#include <Source/Common/FlatWidget.h>
#include <Source/Notice/PopupInfomationWidget.h>
#include <Source/Live2D/L2DRenderWidget.h>
#include <Source/Dialog/FModelSelectedWidget.h>


class DesktopLive2D : public QObject
{
    Q_OBJECT

    struct ModelInfo
    {
        QString Name;
        QString ImagePath;
        QPixmap* Pixmap;

        bool HasCover;
    };
    typedef QVector<ModelInfo> ModelInfoVector;

public:
    explicit DesktopLive2D(QObject *parent = 0);

    ~DesktopLive2D();

    bool Init();

    QWidget* GetRenderWidget() { return l2d_render_window; }

    QPixmap* GetCurrentModelPreview() { return model_preview; }

    DropdownMenu* GetDropdownMenu() { return l2d_render_window->GetDropdownMenu(); }

private:
    bool LoadModelsInfomation(ModelInfoVector& info_list);

    void CreateModelsCover();

    L2DModel* CreateModel(const QString& name, bool create_motion = false);

    void SetModel(int index);

signals:
    void sigChangedModel();

public slots:
    void slotSetModelFPS(int level);

    void slotOpenModelSelectWidget();

    void slotChangedModel(int index);

    void slotAddNewModelDatas(const QStringList& path_list);

private:
    L2DModel* l2d_model;
    L2DRenderGLWidget* l2d_render_window;
    FModelSelectedWidget* model_selected_widget;

    QPixmap* model_preview;

    ModelInfoVector model_info_list;
    int current_model_index;

    bool is_update_model_cover;
};

#endif // DESKTOPLIVE2D_H
