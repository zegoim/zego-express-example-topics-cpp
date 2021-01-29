#ifndef ZEGOAUDIODATACALLBACKDEMO_H
#define ZEGOAUDIODATACALLBACKDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoAudioDataCallbackDemo;
}

class IAudioDataCallback
{
public:
    virtual ~IAudioDataCallback() {}
    virtual void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
    virtual void onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
    virtual void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
};


class MyAudioDataHandler: public IZegoAudioDataHandler{
public:
    MyAudioDataHandler(IAudioDataCallback *callback): callback(callback){}

    void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onCapturedAudioData(data, dataLength, param);
    }

    void onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onPlaybackAudioData(data, dataLength, param);
    }

    void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onMixedAudioData(data, dataLength, param);
    }

private:
    IAudioDataCallback *callback=nullptr;
};


class ZegoAudioDataCallbackDemo : public QWidget, public IAudioDataCallback
{
    Q_OBJECT

public:
    explicit ZegoAudioDataCallbackDemo(QWidget *parent = nullptr);
    ~ZegoAudioDataCallbackDemo() override;

private slots:
    void on_pushButton_enableAudioDataCallback_clicked();

    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

private:
    void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;
    void onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;
    void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;

    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoAudioDataCallbackDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
    std::string userID;

};

#endif // ZEGOAUDIODATACALLBACKDEMO_H
