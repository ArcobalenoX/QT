#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T21:54:14
#
#-------------------------------------------------

QT       += core gui multimedia network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopLive2D
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS L2D_TARGET_WIN_GL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    Source/Framework/L2DBaseModel.cpp \
    Source/Framework/L2DExpressionMotion.cpp \
    Source/Framework/L2DEyeBlink.cpp \
    Source/Framework/L2DMatrix44.cpp \
    Source/Framework/L2DModelMatrix.cpp \
    Source/Framework/L2DMotionManager.cpp \
    Source/Framework/L2DPhysics.cpp \
    Source/Framework/L2DPose.cpp \
    Source/Framework/L2DTargetPoint.cpp \
    Source/Framework/L2DTextureDesc.cpp \
    Source/Framework/L2DViewMatrix.cpp \
    Source/Framework/Live2DFramework.cpp \
    Source/Live2D/L2DModel.cpp \
    Source/Live2D/L2DModelMotion.cpp \
    Source/Live2D/L2DModelParam.cpp \
    Source/Live2D/L2DRenderer.cpp \
    Source/Live2D/L2DRenderWidget.cpp \
    Source/Live2D/L2DmodelController.cpp \
    Source/Dialog/FMessageBox.cpp \
    Source/Dialog/FModelSelectedWidget.cpp \
    Source/Dialog/FSettingDialog.cpp \
    Source/Capture/CaptureTool.cpp \
    Source/Capture/AreaSelectedBox.cpp \
    Source/Capture/CaptureViewer.cpp \
    Source/Capture/CaptureEditer.cpp \
    Source/Common/DataManager.cpp \
    Source/Common/FlatWidget.cpp \
    Source/Common/PathHelper.cpp \
    Source/Common/StyleSheetManager.cpp \
    Source/Common.cpp \
    Source/Common/FramelessWidget.cpp \
    Source/Common/HideInSide.cpp \
    Source/Common/Debug.cpp \
    Source/Common/DropdownMenu.cpp \
    Source/Bilibili/XFSelectedDialog.cpp \
    Source/Bilibili/XFSpider.cpp \
    Source/Bilibili/XFManager.cpp \
    Source/Bilibili/XFViewer.cpp \
    Source/Notice/NoticeManager.cpp \
    Source/Notice/PopupInfomationWidget.cpp \
    Source/DesktopLive2D.cpp \
    Source/Application.cpp \


HEADERS  += \
    Source/Framework/IPlatformManager.h \
    Source/Framework/L2DBaseModel.h \
    Source/Framework/L2DExpressionMotion.h \
    Source/Framework/L2DEyeBlink.h \
    Source/Framework/L2DMatrix44.h \
    Source/Framework/L2DModelMatrix.h \
    Source/Framework/L2DMotionManager.h \
    Source/Framework/L2DPhysics.h \
    Source/Framework/L2DPose.h \
    Source/Framework/L2DStandardID.h \
    Source/Framework/L2DTargetPoint.h \
    Source/Framework/L2DTextureDesc.h \
    Source/Framework/L2DViewMatrix.h \
    Source/Framework/Live2DFramework.h \
    Source/Live2D/L2DDefine.h \
    Source/Live2D/L2DModel.h \
    Source/Live2D/L2DModelMotion.h \
    Source/Live2D/L2DModelParam.h \
    Source/Live2D/L2DModelSettingJson.h \
    Source/Live2D/L2DRenderer.h \
    Source/Live2D/L2DRenderWidget.h \
    Source/Live2D/L2DModelController.h \
    Source/Dialog/FMessageBox.h \
    Source/Dialog/FModelSelectedWidget.h \
    Source/Capture/CaptureTool.h \
    Source/Capture/AreaSelectedBox.h \
    Source/Capture/CaptureViewer.h \
    Source/Capture/CaptureEditer.h \
    Source/Common/DataManager.h \
    Source/Common/Debug.h \
    Source/Common/FlatWidget.h \
    Source/Common/PathHelper.h \
    Source/Common/StyleSheetManager.h \
    Source/Common/FramelessWidget.h \
    Source/Common/HideInSide.h \
    Source/Common/DropdownMenu.h \
    Source/Bilibili/XFSelectedDialog.h \
    Source/Bilibili/XFSpider.h \
    Source/Bilibili/XFManager.h \
    Source/Bilibili/XFViewer.h \
    Source/Dialog/UISettingDialog.h \
    Source/Dialog/FSettingDialog.h \
    Source/Notice/NoticeManager.h \
    Source/Notice/PopupInfomationWidget.h \
    Source/Common.h \
    Source/Application.h \
    Source/DesktopLive2D.h


INCLUDEPATH +=  ./External \

QMAKE_LIBDIR = ./Lib


CONFIG(debug, debug|release){
        LIBS += live2dDebug/live2d_opengl.lib
} else {
        LIBS += live2dRelease/live2d_opengl.lib
}

#CONFIG(debug, debug|release): LIBS += live2dDebug/live2d_opengl.lib
#CONFIG(release, debug|release): LIBS += live2dRelease/live2d_opengl.lib

LIBS += zlib.lib opengl32.lib


RESOURCES += \
    Icons.qrc

RC_FILE = icon.rc

FORMS += \
    form.ui
