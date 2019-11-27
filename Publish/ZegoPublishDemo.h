#ifndef ZEGOPUBLISHDEMO_H
#define ZEGOPUBLISHDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
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
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errCode);
    void onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality& quality);
    void onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent firstFrameEvent);
    void onPublisherVideoSizeChanged(int width, int height);

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_clear_log_clicked();
    void on_comboBox_camera_currentIndexChanged(const QString &arg1);
    void on_comboBox_microphone_currentIndexChanged(const QString &arg1);
    void on_comboBox_viewmode_currentIndexChanged(int index);
    void on_comboBox_mirrormode_currentIndexChanged(int index);
    void on_checkBox_mutePublishStreamAudio_clicked(bool checked);
    void on_checkBox_mutePublishStreamVideo_clicked(bool checked);
    void on_slider_captureVolume_valueChanged(int value);
    void on_checkBox_enableMicrophone_clicked(bool checked);
    void on_checkBox_enableCamera_clicked(bool checked);
    void on_comboBox_videoConfig_currentIndexChanged(int index);

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoPublishDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOPUBLISHDEMO_H
