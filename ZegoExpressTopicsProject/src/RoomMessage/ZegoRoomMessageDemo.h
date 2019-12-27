#ifndef ZEGOROOMMESSAGEDEMO_H
#define ZEGOROOMMESSAGEDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
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
    ZEGO::EXPRESS::IZegoExpressEngine *engine;

    std::vector<ZegoUser> zegoUserList;
    std::string roomID;
    std::string userID;
};

#endif // ZEGOROOMMESSAGEDEMO_H
