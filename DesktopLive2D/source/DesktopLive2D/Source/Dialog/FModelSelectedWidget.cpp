#include "FModelSelectedWidget.h"
#include "FMessageBox.h"

#include <Source/Common/Debug.h>
#include <Source/Common/PathHelper.h>
#include <Source/Common/StyleSheetManager.h>

#include <QMimeData>
#include <QStringList>
#include <QMessageBox>

FModelSelectedWidget::FModelSelectedWidget(QWidget *parent) : FlatWidget(parent)
{
    this->SetWindowTitle(QString::fromLocal8Bit("设置模型"));
    this->setAcceptDrops(true);
    this->resize(600, 400);

    QHBoxLayout* hbox_layout = new QHBoxLayout();
    QVBoxLayout* vbox_layout = new QVBoxLayout();

    standard_model = new QStandardItemModel(this);
    list_view = new QListView(this);
    list_view->setModel(standard_model);
    list_view->setMouseTracking(true);
    list_view->setEditTriggers(QListView::NoEditTriggers);

    connect(list_view, &QListView::entered, this, [=](const QModelIndex& index){
        image_view->setPixmap(*(item_vector[index.row()].pixmap));
    });

    connect(list_view, &QListView::doubleClicked, this, [=](const QModelIndex& index){
        emit sigModelSelected(index.row());
    });

    image_view = new QLabel("");
    image_view->setFixedWidth(300);
    image_view->setStyleSheet(StyleSheetManager::GetLabelStyle());

    btn_add_model = new QPushButton(QString::fromLocal8Bit("添加模型"), this);
    connect(btn_add_model, SIGNAL(pressed()), this, SLOT(slotOpenModelSelectedDialog()));

    hbox_layout->addWidget(image_view);
    hbox_layout->addWidget(list_view);

    vbox_layout->addLayout(hbox_layout);
    vbox_layout->addWidget(btn_add_model);
    this->SetLayout(vbox_layout);
}

FModelSelectedWidget::~FModelSelectedWidget()
{
    qDebug("Delete ModelSelectedWidget");
}

void FModelSelectedWidget::AppendData(const QString& name, QPixmap *pixmap)
{
    item_vector.push_back({name, pixmap});
    standard_model->appendRow(new QStandardItem(name));
}

void FModelSelectedWidget::ClearData()
{
    standard_model->clear();
    item_vector.clear();
}

void FModelSelectedWidget::slotOpenModelSelectedDialog()
{
    QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择模型文件夹"));
    if( path.isNull() ) return;

    if( this->IsAcceptDir(path) )
    {
        QStringList list;
        list << path;
        this->AddNewModels(list);
    }
}

void FModelSelectedWidget::AddNewModels(const QStringList& path_list)
{
    QDir dir;
    QStringList new_list;

    for(int i = 0; i < path_list.size(); i++)
    {
        dir.setPath(path_list[i]);
        if( QFile::exists(PathHelper::GetModelDirPath() + dir.dirName()) )
        {
            FMessageBox msgbox;
            msgbox.SetTitle(QString::fromLocal8Bit("提示"));
            msgbox.SetText(QString::fromLocal8Bit("已经存在模型: %1").arg(dir.dirName()));
            msgbox.Exec();
            continue;
        }
        new_list << path_list[i];
        this->CopyDir(path_list[i], PathHelper::GetModelDirPath());
    }

    if( new_list.size() > 0 )
    {
        /* 通知 DesktopLive2D 对象为新的模型创建封面并更新数据 */
        emit sigNewModels(new_list);
    }
}

bool FModelSelectedWidget::IsAcceptDir(const QDir& dir)
{
    /* 检测文件夹中是否存在 xxx.model.json 文件 */
    QStringList files_name = dir.entryList(QDir::Files);
    if( files_name.size() > 20 ) return false;

    foreach(QString file, files_name)
    {
        if( file.compare(dir.dirName() + ".model.json", Qt::CaseInsensitive) == 0 ) return true;
    }
    return false;
}

bool FModelSelectedWidget::CopyDir(const QString& source_path, const QString &target_path)
{
    QDir root_dir(target_path);
    QDir source_dir(source_path);
    QDir target_dir(root_dir.filePath(source_dir.dirName()));

    /* 创建文件夹 */
    if( target_dir.exists() == false )
    {
        if( root_dir.mkdir(source_dir.dirName()) == false ) return false;
    }

    /* 递归拷贝文件夹及文件 */
    QFileInfoList list = source_dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo file_info, list)
    {
        if( file_info.isDir() )
        {
            if( this->CopyDir(file_info.filePath(), target_dir.absolutePath()) == false ) return false;
        }
        else
        {
            if( QFile::copy(file_info.filePath(), target_dir.filePath(file_info.fileName())) == false ) return false;
        }
    }
    return true;
}

void FModelSelectedWidget::dragEnterEvent(QDragEnterEvent *event)
{
    /* 可以通过拖曳操作添加模型文件夹 */
    if( event->mimeData()->hasFormat("text/uri-list") == false ) return;
    QList<QUrl> urls = event->mimeData()->urls();

    /* 只接受文件夹 */
    for(int i = 0; i < urls.size(); i++)
    {
        QDir dir(urls[i].toLocalFile());
        if( dir.exists() == false || this->IsAcceptDir(dir) == false) return;
    }
    event->acceptProposedAction();
}

void FModelSelectedWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if( urls.size() > 0 )
    {
        QStringList path_list;
        for(int i = 0; i < urls.size(); i++)
        {
            path_list << urls[i].toLocalFile();
        }
        /* 将模型数据添加到程序制定文件夹中 */
        this->AddNewModels(path_list);
    }
}
