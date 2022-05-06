#include <QApplication>
#include <QSharedMemory>
#include <QProcess>

#include <Source/Application.h>
#include <Source/Common.h>
#include <Source/Common/PathHelper.h>
#include <Source/Dialog/FMessageBox.h>

int main(int argc, char *argv[])
{
    /* ʹ�ù����ڴ�ʵ�ֵ������� */
#ifdef DEPLOY
    QSharedMemory shared_memory("UniqueDesktopLive2D Deploy");
#else
    QSharedMemory shared_memory("UniqueDesktopLive2D");
#endif

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("icon_app.ico"));

    PathHelper path_helper;

    /* ���ڹ����ڴ棬�������Ѿ����� */
    if( shared_memory.attach() )
    {
        FMessageBox msg_box(QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����Ѿ�����"), FMB_Yes);
        msg_box.resize(200, 150);
        msg_box.show();

        return app.exec();
    }
    else
    {
        shared_memory.create(1);

        Application my_app;
        if( my_app.Init() == false )
        {
            return 0;
        }
        return app.exec();
    }
}
