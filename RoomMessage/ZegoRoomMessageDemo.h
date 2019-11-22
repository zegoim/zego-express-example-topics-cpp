#ifndef ZEGOROOMMESSAGEDEMO_H
#define ZEGOROOMMESSAGEDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoRoomMessageDemo;
}

class ZegoRoomMessageDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoRoomMessageDemo(QWidget *parent = nullptr);
    ~ZegoRoomMessageDemo();


public slots:
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoMessageInfo> messageList);
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command);

private slots:
    void on_pushButton_send_broadcast_message_clicked();
    void on_pushButton_send_custom_command_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoRoomMessageDemo *ui;
    std::vector<ZegoUser> zegoUserList;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOROOMMESSAGEDEMO_H
