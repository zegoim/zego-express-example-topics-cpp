#include "ZegoRoomMessageDemo.h"
#include "ui_ZegoRoomMessageDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"


ZegoRoomMessageDemo::ZegoRoomMessageDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoRoomMessageDemo)
{
    ui->setupUi(this);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressEngine::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    roomID = "ChatRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    ZegoRoomConfig roomConfig(12, true);
    engine->loginRoom(roomID, user, &roomConfig);
}

ZegoRoomMessageDemo::~ZegoRoomMessageDemo()
{
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

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

        if(updateType == ZEGO_UPDATE_TYPE_DEL && it != zegoUserList.end()){
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

void ZegoRoomMessageDemo::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoMessageInfo> messageList)
{
    for (const ZegoMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv message]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
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

    engine->sendBroadcastMassage(roomID, message,  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_IM_panel->append(QString("[send message]\t: roomID=%1, message=%2").arg(roomID.c_str()).arg(message.c_str()));
        }
        QString log = QString("[send message]\t: roomID=%1, message=%2, errorCode=%3").arg(roomID.c_str()).arg(message.c_str()).arg(errorCode);
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

    engine->sendCustomCommand(roomID, {user}, command,  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_IM_panel->append(QString("[send command]\t: roomID=%1, toUser=%2, command=%3").arg(roomID.c_str()).arg(user.userID.c_str()).arg(command.c_str()));
        }
        QString log = QString("[send command]\t: roomID=%1, toUser=%2, command=%3, errorCode=%4").arg(roomID.c_str()).arg(user.userID.c_str()).arg(command.c_str()).arg(errorCode);
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

    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigIMRecvBroadcastMessage, this, &ZegoRoomMessageDemo::onIMRecvBroadcastMessage);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigIMRecvCustomCommand, this, &ZegoRoomMessageDemo::onIMRecvCustomCommand);

    engine->addEventHandler(eventHandler);
}
