#-------------------------------------------------
#
# Project created by QtCreator 2018-05-18T10:58:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PolarStarProj
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwidj.cpp

HEADERS  += mainwidj.h

FORMS    += mainwidj.ui

##Подключение заголовочных файлов OpenCV
#INCLUDEPATH += D:\opencv_2.4.13\build\include

##Подключение библиотек (.dll и .lib файлов) OpenCV
#CONFIG(debug, debug|release) {
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_highgui2413d.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_imgproc2413d.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_core2413d.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_features2d2413d.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_objdetect2413d.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_contrib2413d.lib
#} else {
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_highgui2413.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_imgproc2413.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_core2413.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_features2d2413.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_objdetect2413.lib
#LIBS += D:\opencv_2.4.13\build\x64\vc12\lib\opencv_contrib2413.lib
#}

INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/core/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/highgui/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/objdetect/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/imgcodecs/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv_contrib-master/modules/face/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/imgproc/include
INCLUDEPATH += D:/opencv-3.2.4_github/opencv-master/modules/videoio/include

INCLUDEPATH += D:/opencv-3.2.4_github/opencv-build

INCLUDEPATH += D:/dlib-19.4

LIBS += D:/opencv-3.2.4_github/opencv-build/lib/Release/*.lib

LIBS += D:/dlib-19.4/build/dlib/Release/dlib.lib
