SOURCES += \
    src/lib/MatToQImage.cpp \
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
