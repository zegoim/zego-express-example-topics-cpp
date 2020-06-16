#ifndef ZEGOPUBLISHDEMO_H
#define ZEGOPUBLISHDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoPublishDemo;
}

class ZegoPublishDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoPublishDemo(QWidget *parent = nullptr);
    ~ZegoPublishDemo();

public slots:
    void onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality &quality);
    void onPublisherVideoSizeChanged(int width, int height);
    void onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo);
    void onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo& deviceInfo);

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_comboBox_camera_currentIndexChanged(const QString &arg1);
    void on_comboBox_microphone_currentIndexChanged(const QString &arg1);
    void on_comboBox_viewmode_currentIndexChanged(int index);
    void on_comboBox_mirrormode_currentIndexChanged(int index);
    void on_checkBox_mutePublishStreamAudio_clicked(bool checked);
    void on_checkBox_mutePublishStreamVideo_clicked(bool checked);
    void on_slider_captureVolume_valueChanged(int value);    
    void on_checkBox_muteMicrophone_clicked(bool checked);
    void on_checkBox_enableCamera_clicked(bool checked);
    void on_comboBox_videoConfig_currentIndexChanged(int index);

    void on_comboBox_audioConfig_currentIndexChanged(int index);

private:
    void bindEventHandler();

private:
    Ui::ZegoPublishDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOPUBLISHDEMO_H
