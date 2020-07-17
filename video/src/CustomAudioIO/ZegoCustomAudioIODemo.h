#ifndef ZEGOCUSTOMAUDIOIODEMO_H
#define ZEGOCUSTOMAUDIOIODEMO_H

#include <QWidget>
#include <QAudioOutput>
#include <QAudioInput>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoCustomAudioIODemo;
}

class IAudioDataCallback
{
public:
    virtual ~IAudioDataCallback() {}
    virtual void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
    virtual void onRemoteAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
    virtual void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) =0;
};

class MyAudioDataHandler: public IZegoAudioDataHandler{
public:
    MyAudioDataHandler(IAudioDataCallback *callback): callback(callback){}

    void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onCapturedAudioData(data, dataLength, param);
    }

    void onRemoteAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onRemoteAudioData(data, dataLength, param);
    }

    void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override{
        callback->onMixedAudioData(data, dataLength, param);
    }

private:
    IAudioDataCallback *callback=nullptr;
};

class CustomAudioInputDevice;
class CustomAudioOutputDevice;
class ZegoCustomAudioIODemo : public QWidget,  public IAudioDataCallback
{
    Q_OBJECT

public:
    explicit ZegoCustomAudioIODemo(QWidget *parent = nullptr);
    ~ZegoCustomAudioIODemo() override;

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_enableAudioDataCallback_clicked();
    void on_pushButton_enableCustomAudioIO_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

    void startCustomAudioCapture();
    void stopCustomAudioCapture();
    void startCustomAudioRender();
    void stopCustomAudioRender();

    void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;
    void onRemoteAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;
    void onMixedAudioData(const unsigned char* data, unsigned int dataLength, ZegoAudioFrameParam param) override;

private:
    Ui::ZegoCustomAudioIODemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
    std::string userID;
	
    QAudioInput  *mCustomAudioInput = nullptr;
    CustomAudioInputDevice *mCustomAudioInputDevice = nullptr;

    QAudioOutput *mCustomAudioOutput = nullptr;
    CustomAudioOutputDevice *mCustomAudioOutputDevice = nullptr;
};

#endif // ZEGOCUSTOMAUDIOIODEMO_H
