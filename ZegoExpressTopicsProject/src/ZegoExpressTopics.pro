#-------------------------------------------------
#
# Project created by QtCreator 2019-10-28T09:17:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZegoExpressTopics
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
    Play/ZegoPlayDemo.cpp \
    Publish/ZegoPublishDemo.cpp \
    AppSupport/ZegoConfigManager.cpp \
    EventHandler/ZegoEventHandlerQt.cpp \
    QuickStart/ZegoQuickStartDemo.cpp \
    AppSupport/ZegoUtilHelper.cpp \
    Mixer/ZegoMixerDemo.cpp \
    RoomMessage/ZegoRoomMessageDemo.cpp \
    SoundLevel/ZegoSoundLevelDemo.cpp \
    SoundLevel/ZegoSoundFrame.cpp \
    EventHandler/ZegoEventHandlerWithLogger.cpp \
    VideoTalk/ZegoVideoTalkDemo.cpp \
    Beautify/ZegoBeautifyDemo.cpp \
    ApiMonkey/ZegoApiMonkeyDemo.cpp \
    MediaPlayer/ZegoMediaPlayerDemo.cpp

HEADERS += \
    ExpressDemo/ZegoExpressDemo.h \
    Play/ZegoPlayDemo.h \
    Publish/ZegoPublishDemo.h \
    AppSupport/ZegoConfigManager.h \
    AppSupport/ZegoCrashHelper.h \
    EventHandler/ZegoEventHandlerQt.h \
    QuickStart/ZegoQuickStartDemo.h \
    AppSupport/ZegoUtilHelper.h \
    Mixer/ZegoMixerDemo.h \
    RoomMessage/ZegoRoomMessageDemo.h \
    SoundLevel/ZegoSoundLevelDemo.h \
    SoundLevel/ZegoSoundFrame.h \
    EventHandler/ZegoEventHandlerWithLogger.h \
    VideoTalk/ZegoVideoTalkDemo.h \
    Beautify/ZegoBeautifyDemo.h \
    ApiMonkey/ZegoApiMonkeyDemo.h \
    MediaPlayer/ZegoMediaPlayerDemo.h

FORMS += \
    ExpressDemo/ZegoExpressDemo.ui \
    Play/ZegoPlayDemo.ui \
    Publish/ZegoPublishDemo.ui \
    QuickStart/ZegoQuickStartDemo.ui \
    Mixer/ZegoMixerDemo.ui \
    RoomMessage/ZegoRoomMessageDemo.ui \
    SoundLevel/ZegoSoundLevelDemo.ui \
    VideoTalk/ZegoVideoTalkDemo.ui \
    Beautify/ZegoBeautifyDemo.ui \
    ApiMonkey/ZegoApiMonkeyDemo.ui \
    MediaPlayer/ZegoMediaPlayerDemo.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../libs/ZegoExpress/include
DEPENDPATH  += $$PWD/../libs/ZegoExpress/include

msvc {
    debug {
        DESTDIR = $$PWD/../bin/debug
    }
    release {
        DESTDIR = $$PWD/../bin/release
    }

    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/../libs/ZegoExpress/lib/x86 -lZegoExpressEngine
    } else {
        LIBS += -L$$PWD/../libs/ZegoExpress/lib/x64 -lZegoExpressEngine
    }

    LIBS +=  \
        -luser32 \
        -lgdi32 \
}
