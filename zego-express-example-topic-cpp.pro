#-------------------------------------------------
#
# Project created by QtCreator 2019-10-28T09:17:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zego-express-example-topic-cpp
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
        ZegoExpressDemo.cpp \
    Play/ZegoPlayDemo.cpp \
    Publish/ZegoPublishDemo.cpp \
    VideoCommunication/ZegoVideoCommunicationDemo.cpp \
    AppSupport/ZegoConfigManager.cpp \
    EventHandler/ZegoEventHandlerQt.cpp

HEADERS += \
        ZegoExpressDemo.h \
    Play/ZegoPlayDemo.h \
    Publish/ZegoPublishDemo.h \
    VideoCommunication/ZegoVideoCommunicationDemo.h \
    AppSupport/ZegoConfigManager.h \
    AppSupport/ZegoCrashHelper.h \
    EventHandler/ZegoEventHandlerQt.h
FORMS += \
        zegoexpressdemo.ui \
    Play/ZegoPlayDemo.ui \
    Publish/ZegoPublishDemo.ui \
    VideoCommunication/ZegoVideoCommunicationDemo.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/deps/ZegoExpress/include
DEPENDPATH  += $$PWD/deps/ZegoExpress/include

debug {
    DESTDIR = $$PWD/builds/debug
}

release {
    DESTDIR = $$PWD/builds/release
}

msvc {
    QMAKE_CXXFLAGS += -execution-charset:utf-8
    QMAKE_CXXFLAGS += -source-charset:utf-8
	
    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/deps/ZegoExpress/lib/x86 -lZegoExpressEngine
    } else {
        LIBS += -L$$PWD/deps/ZegoExpress/lib/x64 -lZegoExpressEngine
    }
	

    LIBS +=  \
        -luser32 \
        -lgdi32 \
}
