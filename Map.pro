#-------------------------------------------------
#
# Project created by QtCreator 2020-10-15T16:33:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Map
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    drawv1.cpp \
        main.cpp \
        mainwindow.cpp \
    maper.cpp \
    gps2utm.cpp \
    utm2pixel.cpp \
    abstructedmap.cpp \
    picdialog.cpp \
    degminsecond.cpp
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               /usr/local/include/opencv4
LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so
HEADERS += \
        mainwindow.h \
    maper.h \
    Gps2UTM.h \
    utm2pixel.h \
    util.h \
    abstructedmap.h \
    drawv1.h \
    tinycomponent.h \
    jsonhelper.h \
    picdialog.h \
    degminsecond.h

FORMS += \
        mainwindow.ui \
    picdialog.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lreadGPS
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lreadGPS
else:unix: LIBS += -L$$PWD/libs/ -lreadGPS

INCLUDEPATH += $$PWD/libs $$PWD/headers
DEPENDPATH += $$PWD/libs
