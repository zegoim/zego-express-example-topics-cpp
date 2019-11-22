#ifndef ZEGOAPIMONCKYDEMO_H
#define ZEGOAPIMONCKYDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoApiMonckyDemo;
}

class ZegoApiMonckyDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoApiMonckyDemo(QWidget *parent = nullptr);
    ~ZegoApiMonckyDemo();

public slots:
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errCode);
    void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errCode);

    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList);
    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoMessageInfo> messageList);
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command);

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_logout_clicked();

    void on_pushButton_start_publish_clicked();
    void on_pushButton_stop_publish_clicked();

    void on_pushButton_start_play_clicked();
    void on_pushButton_stop_play_clicked();

    void on_pushButton_start_mixer_task_clicked();
    void on_pushButton_stop_mixer_task_clicked();

    void on_pushButton_sendBroadcastMessage_clicked();
    void on_pushButton_sendCustomCommand_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoApiMonckyDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
};

#endif // ZEGOAPIMONCKYDEMO_H
