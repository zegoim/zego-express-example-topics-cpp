#ifndef ZEGOAUDIOMIXINGDEMO_H
#define ZEGOAUDIOMIXINGDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoAudioMixingDemo;
}

class IAudioMixingCallback{
public:
    virtual ~IAudioMixingCallback() = default;
    virtual void onAudioMixingCopyData(ZegoAudioMixingData* data)=0;
};

class MyAudioMixingHandler: public IZegoAudioMixingHandler{
public:
    MyAudioMixingHandler(IAudioMixingCallback *callback){
        this->callback = callback;
    }

    ~MyAudioMixingHandler() override{

    }

    void onAudioMixingCopyData(ZegoAudioMixingData* data) override
    {
        callback->onAudioMixingCopyData(data);
    }

private:
    IAudioMixingCallback* callback = nullptr;
};

class ZegoAudioMixingDemo : public QWidget, public IAudioMixingCallback
{
    Q_OBJECT

public:
    explicit ZegoAudioMixingDemo(QWidget *parent = nullptr);
    ~ZegoAudioMixingDemo() override;

    void onAudioMixingCopyData(ZegoAudioMixingData* data) override;

private slots:
    void on_pushButton_start_publish_clicked();
    void on_pushButton_stop_publish_clicked();

    void on_pushButton_loadAudioFile_clicked();
    void on_horizontalSlider_localVolume_valueChanged(int value);
    void on_horizontalSlider_remoteVolume_valueChanged(int value);
    void on_pushButton_enableAudioMixing_clicked();
    void on_pushButton_disableAudioMixing_clicked();

    void on_pushButton_start_play_clicked();
    void on_pushButton_stop_play_clicked();

    void on_checkBox_muteLocalAudioMixing_clicked();

private:
    void bindEventHandler();

private:
    Ui::ZegoAudioMixingDemo *ui;
    IZegoExpressEngine *engine = nullptr;

    std::string currentRoomID;
    std::string currentPublishStreamID;
    std::string currentPlayStreamID;

    std::mutex pcmBufferMutex;
    std::unique_ptr<unsigned char[]> pcmData = nullptr;
    unsigned int pcmDataLength = 0;
    unsigned int pcmDataPos = 0;
};

#endif // ZEGOAUDIOMIXINGDEMO_H
