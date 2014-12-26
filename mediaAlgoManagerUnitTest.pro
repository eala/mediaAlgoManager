QT += widgets testlib

CONFIG -= app_bundle
TEMPLATE = app

#INCLUDEPATH += src/3rdparty/opencv/include
#LIBS += -L"$$_PRO_FILE_PWD_/src/3rdparty/opencv/lib/" \
#
unix{
    #CONFIG += link_pkgconfig
    #PKGCONFIG += opencv
    # directly use MacPort to install opencv first
    INCLUDEPATH +=/opt/local/include

    LIBS += -L/opt/local/lib \
        -lopencv_core \
        -lopencv_highgui\
        -lopencv_imgproc
}
#
#Win32{
#}
#
# fixme later: 1. add install to bin 

include(mediaAlgoManagerLib.pri)
SOURCES += \
    test/tester.cpp
