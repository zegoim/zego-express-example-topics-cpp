#ifndef ZEGOPLAYDEMO_H
#define ZEGOPLAYDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
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
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList);
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList);
    void onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errCode);
    void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality &quality);
    void onPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent mediaEvent);
    void onPlayerRecvFirstFrameEvent(const std::string& streamID, ZegoPlayerFirstFrameEvent firstFrameEvent);
    void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height);

private slots:
    void on_pushButton_clear_log_clicked();
    void on_comboBox_audioOutputDevice_currentIndexChanged(const QString &arg1);
    void on_comboBox_viewmode_currentIndexChanged(int index);
    void on_checkBox_mutePlayStreamAudio_clicked(bool checked);
    void on_checkBox_mutePlayStreamVideo_clicked(bool checked);
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoPlayDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOPLAYDEMO_H
