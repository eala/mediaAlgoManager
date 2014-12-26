QT += widgets

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
SOURCES += \
    src/lib/MatToQImage.cpp \
    src/lib/main.cpp \
    src/lib/mainwindow.cpp \
    src/lib/itsobject.cpp \
    src/lib/itsFrame.cpp \
    src/lib/itsgolden.cpp \
    src/lib/itsfileinfo.cpp \
    src/lib/citsapp.cpp \
    src/lib/analyzegoldenwindow.cpp

HEADERS += \
    src/lib/MatToQImage.h \
    src/lib/mainwindow.h \
    src/lib/itsobject.h \
    src/lib/itsFrame.h \
    src/lib/itsgolden.h \
    src/lib/itsfileinfo.h \
    src/lib/citsapp.h \
    src/lib/analyzegoldenwindow.h

RESOURCES += \
    mediaalgomanager.qrc
#
# I have no idea where is the installed destinations
# resource_files.files += $$_PRO_FILE_PWD_/src/bin/images/copy.png
# resource_files.path = $${DESTDIR}/images
# INSTALLS += resource_files

FORMS += \
    src/lib/analyzegoldenwindow.ui
