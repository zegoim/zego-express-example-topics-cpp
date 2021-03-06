#-------------------------------------------------
#
# Project created by QtCreator 2019-10-28T09:17:50
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZegoExpressExampleVideo
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
    MediaPlayer/ZegoMediaPlayerDemo.cpp \
    CustomVideoRender/ZegoCustomVideoRenderDemo.cpp \
    CustomVideoCapture/ZegoCustomVideoCaptureDemo.cpp \
    CustomVideoCapture/ZegoCustomVideoSourceBase.cpp \
    CustomVideoCapture/ZegoCustomVideoSourceImage.cpp \
    CustomVideoCapture/ZegoCustomVideoSourceMedia.cpp \
    CustomVideoCapture/ZegoCustomVideoSourceContext.cpp \
    CDNAbout/ZegoCDNAboutDemo.cpp \
    AuxStream/ZegoAuxStreamDemo.cpp \
    AudioMixing/ZegoAudioMixingDemo.cpp \
    DataRecord/ZegoDataRecordDemo.cpp \
    SoundEffects/ZegoSoundEffectsDemo.cpp \
    CustomAudioIO/ZegoCustomAudioIODemo.cpp \
    MultiLayer/ZegoMultiLayerDemo.cpp \
    MultiRoom/ZegoMultiRoomDemo.cpp \
    AudioProcess/ZegoAudioProcessDemo.cpp \
    AudioEffectPlayer/ZegoAudioEffectPlayerDemo.cpp \
    AudioDataCallback/ZegoAudioDataCallbackDemo.cpp \
    UtilityTools/ZegoUtilityToolsDemo.cpp

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
    MediaPlayer/ZegoMediaPlayerDemo.h \
    CustomVideoRender/ZegoCustomVideoRenderDemo.h \
    CustomVideoCapture/ZegoCustomVideoCaptureDemo.h \
    CustomVideoCapture/ZegoCustomVideoSourceBase.h \
    CustomVideoCapture/ZegoCustomVideoSourceImage.h \
    CustomVideoCapture/ZegoCustomVideoSourceMedia.h \
    CustomVideoCapture/ZegoCustomVideoSourceContext.h \
    CDNAbout/ZegoCDNAboutDemo.h \
    AuxStream/ZegoAuxStreamDemo.h \
    AudioMixing/ZegoAudioMixingDemo.h \
    DataRecord/ZegoDataRecordDemo.h \
    SoundEffects/ZegoSoundEffectsDemo.h \
    CustomAudioIO/ZegoCustomAudioIODemo.h \
    MultiLayer/ZegoMultiLayerDemo.h \
    MultiRoom/ZegoMultiRoomDemo.h \
    AudioProcess/ZegoAudioProcessDemo.h \
    version.h \
    AudioEffectPlayer/ZegoAudioEffectPlayerDemo.h \
    AudioDataCallback/ZegoAudioDataCallbackDemo.h \
    UtilityTools/ZegoUtilityToolsDemo.h


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
    MediaPlayer/ZegoMediaPlayerDemo.ui \
    CustomVideoRender/ZegoCustomVideoRenderDemo.ui \
    CustomVideoCapture/ZegoCustomVideoCaptureDemo.ui \
    CDNAbout/ZegoCDNAboutDemo.ui \
    AuxStream/ZegoAuxStreamDemo.ui \
    AudioMixing/ZegoAudioMixingDemo.ui \
    DataRecord/ZegoDataRecordDemo.ui \
    SoundEffects/ZegoSoundEffectsDemo.ui \
    CustomAudioIO/ZegoCustomAudioIODemo.ui \
    MultiLayer/ZegoMultiLayerDemo.ui \
    MultiRoom/ZegoMultiRoomDemo.ui \
    AudioProcess/ZegoAudioProcessDemo.ui \
    AudioEffectPlayer/ZegoAudioEffectPlayerDemo.ui \
    AudioDataCallback/ZegoAudioDataCallbackDemo.ui \
    UtilityTools/ZegoUtilityToolsDemo.ui


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
    #QMAKE_CFLAGS += /utf-8
    #QMAKE_CXXFLAGS += /utf-8

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
    LIBS += -framework Foundation -framework CoreGraphics
    LIBS += -L$$PWD/../libs/ZegoExpress/mac/lib/ -lZegoExpressEngine

    QMAKE_INFO_PLIST = Info.plist
}

unix{
    MOC_DIR = $$PWD/../build-linux/qmake_temp
    RCC_DIR = $$PWD/../build-linux/qmake_temp
    UI_DIR = $$PWD/../build-linux/qmake_temp
    OBJECTS_DIR = $$PWD/../build-linux/qmake_temp

    INCLUDEPATH += $$PWD/../libs/ZegoExpress/linux/include
    DEPENDPATH  += $$PWD/../libs/ZegoExpress/linux/include

    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/../libs/ZegoExpress/linux/lib/x86 -lZegoExpressEngine
    } else {
        LIBS += -L$$PWD/../libs/ZegoExpress/linux/lib/x86_64 -lZegoExpressEngine
    }
}



