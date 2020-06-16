#ifndef ZEGOPLAYDEMO_H
#define ZEGOPLAYDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoPlayDemo;
}

class ZegoPlayDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoPlayDemo(QWidget *parent = nullptr);
    ~ZegoPlayDemo();

public slots:
       void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality &quality);
	   void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height);
       void onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo);

private slots:
    void on_comboBox_audioOutputDevice_currentIndexChanged(const QString &arg1);
    void on_comboBox_viewmode_currentIndexChanged(int index);
    void on_checkBox_mutePlayStreamAudio_clicked(bool checked);
    void on_checkBox_mutePlayStreamVideo_clicked(bool checked);
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();
    void on_slider_playVolume_valueChanged(int value);

private:
    void bindEventHandler();

private:
    Ui::ZegoPlayDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOPLAYDEMO_H
