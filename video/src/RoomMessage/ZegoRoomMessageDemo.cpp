#include "ZegoRoomMessageDemo.h"
#include "ui_ZegoRoomMessageDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"


ZegoRoomMessageDemo::ZegoRoomMessageDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoRoomMessageDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "ChatRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    ZegoRoomConfig roomConfig;
    roomConfig.isUserStatusNotify = true;
    engine->loginRoom(currentRoomID, user, roomConfig);
}

ZegoRoomMessageDemo::~ZegoRoomMessageDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoRoomMessageDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    Q_UNUSED(roomID)
    for_each(userList.begin(), userList.end(), [&](ZegoUser user){
        auto it  = std::find_if(zegoUserList.begin(), zegoUserList.end(), [&](ZegoUser const &_user){
                return _user.userID == user.userID;
        });

        if(updateType == ZEGO_UPDATE_TYPE_ADD && it == zegoUserList.end()){
            zegoUserList.push_back(user);
        }

        if(updateType == ZEGO_UPDATE_TYPE_DELETE && it != zegoUserList.end()){
            zegoUserList.erase(it);
        }

    });

    {
        QStringList userIDs;
        for(auto &user: zegoUserList){
            userIDs.append(user.userID.c_str());
        }
        ui->comboBox_toUser->clear();
        ui->comboBox_toUser->addItems(userIDs);
    }
}

void ZegoRoomMessageDemo::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoBroadcastMessageInfo> messageList)
{
    for (const ZegoBroadcastMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv broadcast]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        ui->textEdit_IM_panel->append(log);
    }
}

void ZegoRoomMessageDemo::onIMRecvBarrageMessage(const std::string &roomID, std::vector<ZegoBarrageMessageInfo> messageList)
{
    for (const ZegoBarrageMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv barrage]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        ui->textEdit_IM_panel->append(log);
    }
}

void ZegoRoomMessageDemo::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    QString log = QString("[recv command]\t: roomID=%1, fromUser=%2, command=%3").arg(roomID.c_str()).arg(fromUser.userID.c_str()).arg(command.c_str());
    ui->textEdit_IM_panel->append(log);
}

void ZegoRoomMessageDemo::on_pushButton_send_broadcast_message_clicked()
{
    std::string message = ui->lineEdit_broadcast_message->text().toStdString();

    engine->sendBroadcastMessage(currentRoomID, message,  [=](int errorCode, unsigned long long messageID){
        if(errorCode==0){
            ui->textEdit_IM_panel->append(QString("[send broadcast]\t: roomID=%1, message=%2, messageID=%3").arg(currentRoomID.c_str()).arg(message.c_str()).arg(messageID));
        }
        QString log = QString("[send message]\t: roomID=%1, message=%2, errorCode=%3").arg(currentRoomID.c_str()).arg(message.c_str()).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoRoomMessageDemo::on_pushButton_send_barrage_message_clicked()
{
    std::string message = ui->lineEdit_barrage_message->text().toStdString();

    engine->sendBarrageMessage(currentRoomID, message,  [=](int errorCode, std::string messageID){
        if(errorCode==0){
            ui->textEdit_IM_panel->append(QString("[send barrage]\t: roomID=%1, message=%2, messageID=%3").arg(currentRoomID.c_str()).arg(message.c_str()).arg(messageID.c_str()));
        }
        QString log = QString("[send message]\t: roomID=%1, message=%2, errorCode=%3").arg(currentRoomID.c_str()).arg(message.c_str()).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoRoomMessageDemo::on_pushButton_send_custom_command_clicked()
{
    if(ui->comboBox_toUser->currentText().isEmpty()){
        return;
    }


    std::string toUserID = ui->comboBox_toUser->currentText().toStdString();
    auto it  = std::find_if(zegoUserList.begin(), zegoUserList.end(), [&](ZegoUser const &_user){
            return _user.userID == toUserID;
    });
    if(it==zegoUserList.end()){
        return;
    }

    ZegoUser user(*it);
    std::string command = ui->lineEdit_custom_command->text().toStdString();

    engine->sendCustomCommand(currentRoomID, command, {user},  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_IM_panel->append(QString("[send command]\t: roomID=%1, command=%2, toUser=%3").arg(currentRoomID.c_str()).arg(command.c_str()).arg(user.userID.c_str()));
        }
        QString log = QString("[send command]\t: roomID=%1,command=%2, toUser=%3,  errorCode=%4").arg(currentRoomID.c_str()).arg(command.c_str()).arg(user.userID.c_str()).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoRoomMessageDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoRoomMessageDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);

    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRoomUserUpdate, this, &ZegoRoomMessageDemo::onRoomUserUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigIMRecvBroadcastMessage, this, &ZegoRoomMessageDemo::onIMRecvBroadcastMessage);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigIMRecvBarrageMessage, this, &ZegoRoomMessageDemo::onIMRecvBarrageMessage);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigIMRecvCustomCommand, this, &ZegoRoomMessageDemo::onIMRecvCustomCommand);


    engine->setEventHandler(eventHandler);
}
