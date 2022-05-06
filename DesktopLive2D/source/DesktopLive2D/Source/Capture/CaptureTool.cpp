#include "CaptureTool.h"

#include <QScreen>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>

#include <Source/Common/PathHelper.h>
#include <Source/Common/StyleSheetManager.h>

CaptureTool::CaptureTool(QWidget *parent)
    : QWidget(parent)
    , capture_editer(nullptr)
    , is_enter(false)
    , is_capture_contain_viewer(true)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    this->resize(64, 64);

    hide_in_side = new HideInSide(this, this);
    hide_in_side->SetDiaplayRatio(0.3);

    icon_normal.load(PathHelper::GetIconDirPath() + "icon_capture_normal_64.png");
    icon_active.load(PathHelper::GetIconDirPath() + "icon_capture_active_64.png");

    menu = new QMenu(this);
    menu->setStyleSheet(StyleSheetManager::GetMenuStyle());
    act_contain_viewer = menu->addAction(QString::fromLocal8Bit("截屏时包含视图"));
    act_contain_viewer->setCheckable(true);
    act_contain_viewer->setChecked(is_capture_contain_viewer);
    connect(act_contain_viewer, SIGNAL(toggled(bool)), this, SLOT(slotCaptureContainViewer(bool)));

    menu->addAction(QString::fromLocal8Bit("最小化所有视图"), this, SLOT(slotMinimizeAllViewer()));
    menu->addAction(QString::fromLocal8Bit("最大化所有视图"), this, SLOT(slotMaximizeAllViewer()));
    menu->addAction(QString::fromLocal8Bit("清空所有视图"), this, SLOT(slotClearAllViewer()));
    menu->addSeparator();
    menu->addAction(QString::fromLocal8Bit("退出"), this, SLOT(deleteLater()));
}

CaptureTool::~CaptureTool()
{
    if( capture_editer ) delete capture_editer;

    foreach( CaptureViewer* viewer, capture_viewers )
    {
        delete viewer;
    }
    capture_viewers.clear();

    qDebug("delete CaptureTool");
}

void CaptureTool::Capture()
{
    if( capture_editer == nullptr )
    {
        capture_editer = new CaptureEditer;
        connect(capture_editer, SIGNAL(sigExitCaptureEditer(bool)), this, SLOT(slotProcessCaptureEditer(bool)));
    }

    QScreen* screen = QApplication::primaryScreen();

    if( is_capture_contain_viewer == false )
    {
        for(int i = 0; i < capture_viewers.size(); i++)
        {
            capture_viewers[i]->hide();
        }
    }
    this->hide();

    /* 截屏到编辑窗口进行编辑 */
    capture_editer->SetImage(screen->grabWindow(QApplication::desktop()->winId()).toImage());
    capture_editer->ShowCaptureEditer();
}

void CaptureTool::slotProcessCaptureEditer(bool succeed)
{
    if( !is_capture_contain_viewer )
    {
        for(int i = 0; i < capture_viewers.size(); i++)
        {
            capture_viewers[i]->show();
        }
    }
    this->show();

    if( succeed )
    {
        QImage image = capture_editer->GetCaptureImage();
        QRect rect = capture_editer->GetCaptureRect();

        CaptureViewer* viewer = new CaptureViewer;
        viewer->SetImage(image);
        viewer->Show(rect);
        connect(viewer, SIGNAL(sigClose(CaptureViewer*)), this, SLOT(slotCaptureViewerClose(CaptureViewer*)));

        capture_viewers.push_back(viewer);
    }
}

void CaptureTool::slotCaptureViewerClose(CaptureViewer* object)
{
    QVector<CaptureViewer*>::iterator it;
    for(it = capture_viewers.begin(); it != capture_viewers.end(); ++it)
    {
        if( (*it) == object )
        {
            capture_viewers.erase(it);
            return;
        }
    }
}

void CaptureTool::slotCaptureContainViewer(bool on)
{
    is_capture_contain_viewer = on;
}

void CaptureTool::slotMinimizeAllViewer()
{
    for(int i = 0; i < capture_viewers.size(); i++)
    {
        capture_viewers[i]->SetBlock(true);
    }
}

void CaptureTool::slotMaximizeAllViewer()
{
    for(int i = 0; i < capture_viewers.size(); i++)
    {
        capture_viewers[i]->SetBlock(false);
    }
}

void CaptureTool::slotClearAllViewer()
{
    for(int i = 0; i < capture_viewers.size(); i++)
    {
        delete capture_viewers[i];
    }
    capture_viewers.clear();
}

void CaptureTool::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath circle;
    circle.addEllipse(this->rect());

    painter.setPen(Qt::NoPen);
    painter.setBrush(is_enter ? QColor(0, 0, 0, 200) : QColor(0, 0, 0, 150));
    painter.drawPath(circle);

    int padding = 10;
    painter.drawImage(this->rect().adjusted(padding, padding, -padding, -padding), is_enter ? icon_active : icon_normal);
}

void CaptureTool::mousePressEvent(QMouseEvent *event)
{
    mouse_origin_point = event->globalPos();
    window_origin_point = this->pos();

    is_move_window = false;
}

void CaptureTool::mouseMoveEvent(QMouseEvent *event)
{
    is_move_window = true;

    if( event->buttons() & Qt::LeftButton )
    {
        this->move(window_origin_point + event->globalPos() - mouse_origin_point);
    }
}

void CaptureTool::mouseReleaseEvent(QMouseEvent *event)
{
    if( is_move_window == false && event->button() & Qt::LeftButton )
    {
        this->Capture();
    }

    if( event->button() & Qt::RightButton )
    {
        menu->exec(QCursor::pos());
    }
    is_move_window = false;
}

void CaptureTool::enterEvent(QEvent *)
{
    is_enter = true;
    this->update();
}

void CaptureTool::leaveEvent(QEvent *)
{
    is_enter = false;
    this->update();
}
