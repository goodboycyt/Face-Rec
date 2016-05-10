#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T08:10:42
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
qmake: QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = faceRec
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    zhujiemian.cpp \
    jiance.cpp \
    shibie.cpp

HEADERS  += widget.h \
    zhujiemian.h \
    jiance.h \
    shibie.h

FORMS    += widget.ui \
    zhujiemian.ui

INCLUDEPATH+=D:\Qt5.6.0\opencv\include\opencv\
             D:\Qt5.6.0\opencv\include\opencv2\
             D:\Qt5.6.0\opencv\include

LIBS+=D:\Qt5.6.0\opencv\lib\libopencv_calib3d249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_contrib249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_core249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_features2d249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_flann249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_gpu249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_highgui249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_imgproc249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_legacy249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_ml249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_objdetect249.dll.a\
  D:\Qt5.6.0\opencv\lib\libopencv_video249.dll.a

RESOURCES += \
    pic.qrc
