QT -= gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mediaAlgoManager
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

#INCLUDEPATH += src/3rdparty/opencv/include
#LIBS += -L"$$_PRO_FILE_PWD_/src/3rdparty/opencv/lib/" \

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

#Win32{
#}

# fixme later: 1. add install to bin 
SOURCES += src/lib/main.cpp

# I have no idea where is the installed destinations
# resource_files.files += $$_PRO_FILE_PWD_/src/bin/images/copy.png
# resource_files.path = $${DESTDIR}/images
# INSTALLS += resource_files

