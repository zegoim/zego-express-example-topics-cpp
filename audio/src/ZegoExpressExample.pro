#-------------------------------------------------
#
# Project created by QtCreator 2020-05-27T15:18:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZegoExpressExampleAudio
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

CONFIG += c++11

SOURCES += \
        main.cpp \
    ExpressDemo/ZegoExpressDemo.cpp \
    QuickStart/ZegoQuickStartDemo.cpp \
    AppSupport/ZegoConfigManager.cpp \
    EventHandler/ZegoEventHandlerQt.cpp \
    EventHandler/ZegoEventHandlerWithLogger.cpp \
    AppSupport/ZegoUtilHelper.cpp \
    SoundLevel/ZegoSoundFrame.cpp \
    SoundLevel/ZegoSoundLevelDemo.cpp

HEADERS += \
    ExpressDemo/ZegoExpressDemo.h \
    AppSupport/ZegoConfigManager.h \
    AppSupport/ZegoCrashHelper.h \
    QuickStart/ZegoQuickStartDemo.h \
    AppSupport/ZegoUtilHelper.h \
    EventHandler/ZegoEventHandlerQt.h \
    SoundLevel/ZegoSoundFrame.h \
    SoundLevel/ZegoSoundLevelDemo.h \
    EventHandler/ZegoEventHandlerWithLogger.h

FORMS += \
    ExpressDemo/ZegoExpressDemo.ui \
    SoundLevel/ZegoSoundLevelDemo.ui \
    QuickStart/ZegoQuickStartDemo.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


debug {
    DESTDIR = $$PWD/../bin/debug
}
release {
    DESTDIR = $$PWD/../bin/release
}

msvc {
    MOC_DIR = $$PWD/../build-win/qmake_temp
    RCC_DIR = $$PWD/../build-win/qmake_temp
    UI_DIR = $$PWD/../build-win/qmake_temp
    OBJECTS_DIR = $$PWD/../build-win/qmake_temp

    INCLUDEPATH += $$PWD/../libs/ZegoExpress/win/include
    DEPENDPATH  += $$PWD/../libs/ZegoExpress/win/include

    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/../libs/ZegoExpress/win/lib/x86 -lZegoExpressEngine
    } else {
        LIBS += -L$$PWD/../libs/ZegoExpress/win/lib/x64 -lZegoExpressEngine
    }

    LIBS +=  \
        -luser32 \
        -lgdi32 \
}

macx {
    MOC_DIR = $$PWD/../build-mac/qmake_temp
    RCC_DIR = $$PWD/../build-mac/qmake_temp
    UI_DIR = $$PWD/../build-mac/qmake_temp
    OBJECTS_DIR = $$PWD/../build-mac/qmake_temp

    INCLUDEPATH += $$PWD/../libs/ZegoExpress/mac/include
    DEPENDPATH  += $$PWD/../libs/ZegoExpress/mac/include

    QMAKE_CXXFLAGS += -x objective-c++
    LIBS += -framework Foundation
    LIBS += -L$$PWD/../libs/ZegoExpress/mac/lib/ -lZegoExpressEngine

    QMAKE_INFO_PLIST = Info.plist
}
