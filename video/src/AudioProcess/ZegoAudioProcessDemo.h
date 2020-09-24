#ifndef ZEGOAUDIOPROCESSDEMO_H
#define ZEGOAUDIOPROCESSDEMO_H

#include <QWidget>

#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoAudioProcessDemo;
}

class IAudioProcessCallback {
public:
    virtual ~IAudioProcessCallback() {}
    virtual void onProcessCapturedAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param) = 0;
    virtual void onProcessRemoteAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param, const std::string& streamID) = 0;
};

class MyAudioProcessHandler: public IZegoCustomAudioProcessHandler{
public:
    MyAudioProcessHandler(IAudioProcessCallback *callback): callback(callback){}

    void onProcessCapturedAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param) override{
        callback->onProcessCapturedAudioData(data, dataLength, param);
    }

    void onProcessRemoteAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param, const std::string& streamID) override {
        callback->onProcessRemoteAudioData(data, dataLength, param, streamID);
    }

private:
    IAudioProcessCallback *callback=nullptr;
};

class ZegoAudioProcessDemo : public QWidget, public IAudioProcessCallback
{
    Q_OBJECT

public:
    explicit ZegoAudioProcessDemo(QWidget *parent = nullptr);
    ~ZegoAudioProcessDemo() override;

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();

    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_horizontalSlider_remoteData_valueChanged(int value);
    void on_horizontalSlider_capturedData_valueChanged(int value);

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

    void onProcessCapturedAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param) override;
    void onProcessRemoteAudioData(unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam* param, const std::string& streamID) override;

private:
    Ui::ZegoAudioProcessDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;

    std::atomic<float> capturedAudioDataRatio = {1.0};
    std::atomic<float> remoteAudioDataRatio = {1.0};
};

#endif // ZEGOAUDIOPROCESSDEMO_H
