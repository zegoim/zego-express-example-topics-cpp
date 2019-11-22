#include "ZegoRoomMessageDemo.h"
#include "ui_ZegoRoomMessageDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerQt.h"

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

    std::string roomID = "ChatRoom-1";
    auto random = ZegoUtilHelper::getRandomString();
    ZegoUser user(random, random);
    ZegoRoomConfig roomConfig(12, true);
    engine->loginRoom(roomID, user, &roomConfig);
    QString log = QString("do loginRoom: roomID=%1, userID=%2").arg(roomID.c_str()).arg(user.userID.c_str());
    printLogToView(log);

    ui->pushButton_IM_title->setText(QString("IMRoomID: %1").arg(roomID.c_str()));
}

ZegoRoomMessageDemo::~ZegoRoomMessageDemo()
{
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoRoomMessageDemo::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoRoomMessageDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoRoomMessageDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for(const ZegoUser &user: userList){
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);


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
        printLogToView(log);
    }
}

void ZegoRoomMessageDemo::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    QString log = QString("[recv command]\t: roomID=%1, fromUser=%2, command=%3").arg(roomID.c_str()).arg(fromUser.userID.c_str()).arg(command.c_str());
    ui->textEdit_IM_panel->append(log);
    printLogToView(log);
}

void ZegoRoomMessageDemo::on_pushButton_send_broadcast_message_clicked()
{
    std::string roomID = "ChatRoom-1";
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
    std::string roomID = "ChatRoom-1";
    std::string command = ui->lineEdit_custom_command->text().toStdString();

    engine->sendCustomCommad(roomID, {user}, command,  [=](int errorCode){
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
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoRoomMessageDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoRoomMessageDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoRoomMessageDemo::onRoomUserUpdate);

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigIMRecvBroadcastMessage, this, &ZegoRoomMessageDemo::onIMRecvBroadcastMessage);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigIMRecvCustomCommand, this, &ZegoRoomMessageDemo::onIMRecvCustomCommand);

    engine->addEventHandler(eventHandler);
}
