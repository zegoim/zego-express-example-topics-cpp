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
    void onRoomUserUpdate(const std::string& currentRoomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void onIMRecvBroadcastMessage(const std::string& currentRoomID, std::vector<ZegoBroadcastMessageInfo> messageList);
    void onIMRecvBarrageMessage(const std::string& currentRoomID, std::vector<ZegoBarrageMessageInfo> messageList);
    void onIMRecvCustomCommand(const std::string& currentRoomID, ZegoUser fromUser, const std::string& command);

private slots:
    void on_pushButton_send_broadcast_message_clicked();
    void on_pushButton_send_barrage_message_clicked();
    void on_pushButton_send_custom_command_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoRoomMessageDemo *ui;
    IZegoExpressEngine *engine = nullptr;

    std::vector<ZegoUser> zegoUserList;
    std::string currentRoomID;
    std::string userID;
};

#endif // ZEGOROOMMESSAGEDEMO_H
