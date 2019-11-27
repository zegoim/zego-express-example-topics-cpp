#include "ZegoApiMonckyDemo.h"
#include "ui_ZegoApiMonckyDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerQt.h"

#include <QDebug>
#include <QThread>
#include <QScrollBar>

#define GuardEngineLoaded \
    if(engine==nullptr) { \
        QString log = QString("Engine Not loaded"); \
        printLogToView(log); \
        return; \
    }

ZegoApiMonckyDemo::ZegoApiMonckyDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoApiMonckyDemo)
{
    ui->setupUi(this);

    std::string roomID = ZegoUtilHelper::getRandomString();
    ui->lineEdit_roomID->setText(roomID.c_str());

    std::string userID = ZegoUtilHelper::getRandomString();
    ui->lineEdit_userID->setText(userID.c_str());
    ui->lineEdit_userName->setText(userID.c_str());

    bool ok = true;
    {
    QString mixerTaskContent("{\"backgroundImageURL\":\"\",\"inputList\":[{\"contentType\":1,\"layout\":{\"bottom\":100,\"left\":0,\"right\":50,\"top\":0},\"streamID\":\"stream1\"},{\"contentType\":1,\"layout\":{\"bottom\":100,\"left\":50,\"right\":100,\"top\":0},\"streamID\":\"stream2\"}],\"outputList\":[{\"target\":\"stream3\"}],\"watermark\":{\"imageURL\":\"\",\"layout\":{\"bottom\":10,\"left\":0,\"right\":10,\"top\":0}}}");
    ui->textEdit_mixer_taskContent->setText(ZegoUtilHelper::jsonObjectToString(ZegoUtilHelper::stringToJsonObject(mixerTaskContent, ok)));

    ui->textEdit_im_panel->append("Welctom to Zego IM\n");

    QString imBroadcastMessage("{\"roomID\":\"room1\",\"message\":\"message\"}");
    ui->textEdit_sendBroadcastMessage->setText(ZegoUtilHelper::jsonObjectToString(ZegoUtilHelper::stringToJsonObject(imBroadcastMessage, ok)));

    QString imCustomCommand("{\"roomID\":\"room1\",\"command\":\"command\",\"toUserList\":[{\"userID\":\"userID\",\"userName\":\"userName\"}]}");
    ui->textEdit_sendCustomCommad->setText(ZegoUtilHelper::jsonObjectToString(ZegoUtilHelper::stringToJsonObject(imCustomCommand, ok)));
    }


    QString log = QString("do createEngine");
    printLogToView(log);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();
    auto iEngine = ZegoExpressEngine::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    if(iEngine != nullptr){
        engine = iEngine;
    }
    bindEventHandler();
}

ZegoApiMonckyDemo::~ZegoApiMonckyDemo()
{
    QString log = QString("do destroyEngine");
    printLogToView(log);

    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;
    delete ui;
}

void ZegoApiMonckyDemo::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoApiMonckyDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%2").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);
}

void ZegoApiMonckyDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList streamIDs;
    for (const ZegoStream &stream : streamList) {
        streamIDs.append(QString::fromStdString(stream.streamID));
    }
    QString log = QString("onRoomStreamUpdate: roomID=%1, updateType=%2, streamIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(streamIDs.join(","));
    printLogToView(log);
}

void ZegoApiMonckyDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoApiMonckyDemo::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errCode) {
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NO_PUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING",
        "ZEGO_PUBLISHER_STATE_PUBLISHING"
    };
    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoApiMonckyDemo::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errCode)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoApiMonckyDemo::onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo> &infoList)
{
    Q_UNUSED(taskID)
    Q_UNUSED(infoList);
}

void ZegoApiMonckyDemo::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoMessageInfo> messageList)
{
    for (const ZegoMessageInfo& messageInfo : messageList) {
        QString log = QString("onIMRecvBroadcastMessage: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        ui->textEdit_im_panel->append(log);
        printLogToView(log);
    }
}

void ZegoApiMonckyDemo::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    QString log = QString("onIMRecvCustomCommand: roomID=%1, fromUser=%2, command=%3").arg(roomID.c_str()).arg(fromUser.userID.c_str()).arg(command.c_str());
    ui->textEdit_im_panel->append(log);
    printLogToView(log);
}

void ZegoApiMonckyDemo::on_pushButton_login_clicked()
{
    GuardEngineLoaded
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    ZegoUser user;
    user.userID = ui->lineEdit_userID->text().toStdString();
    user.userName = ui->lineEdit_userName->text().toStdString();

    engine->loginRoom(roomID, user, nullptr);
    QString log = QString("do loginRoom");
    printLogToView(log);

}

void ZegoApiMonckyDemo::on_pushButton_logout_clicked()
{
    GuardEngineLoaded
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    engine->logoutRoom(roomID);
    QString log = QString("do logoutRoom");
    printLogToView(log);
}

void ZegoApiMonckyDemo::on_pushButton_start_publish_clicked()
{
    GuardEngineLoaded
    std::string streamID = ui->lineEdit_publish_streamID->text().toStdString();
    if(streamID == ""){
        QString log = QString("Input publish streamID needed");
        printLogToView(log);
        return;
    }

    engine->startPublishing(streamID);

    ZegoCanvas canvas((void *)ui->frame_local_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPreview(&canvas);

    QString log = QString("do publish stream");
    printLogToView(log);

}

void ZegoApiMonckyDemo::on_pushButton_stop_publish_clicked()
{
    GuardEngineLoaded
    engine->stopPreview();
    engine->stopPublishing();
}

void ZegoApiMonckyDemo::on_pushButton_start_play_clicked()
{
    GuardEngineLoaded
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    std::string streamID = ui->lineEdit_play_streamID->text().toStdString();

    ZegoCanvas canvas((void *)ui->frame_remote_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoApiMonckyDemo::on_pushButton_stop_play_clicked()
{
    GuardEngineLoaded
    std::string streamID = ui->lineEdit_play_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);

    ui->slider_playVolume->setValue(100);
    ui->checkBox_mutePlayStreamAudio->setChecked(false);
    ui->checkBox_mutePlayStreamVideo->setChecked(false);
}

void ZegoApiMonckyDemo::on_pushButton_start_mixer_task_clicked()
{
    GuardEngineLoaded
    std::string taskID = ui->lineEdit_mixer_taskID->text().toStdString();
    if(taskID == ""){
        printLogToView("please input taskID");
        return;
    }

    ZegoMixerTask task(taskID);

    bool valiadJson = true;
    QString taskContentStr = ui->textEdit_mixer_taskContent->toPlainText();
    QJsonObject taskContent= ZegoUtilHelper::stringToJsonObject(taskContentStr, valiadJson);
    if(!valiadJson){
        printLogToView("please check taskContent");
        return;
    }

    std::vector<ZegoMixerInput> inputList;
    for(QJsonValue inputItemValue: taskContent["inputList"].toArray()){
        ZegoMixerInput input;
        QJsonObject inputItemObject = inputItemValue.toObject();
        input.streamID = inputItemObject["streamID"].toString().toStdString();
        input.contentType = ZegoMixerInputContentType(inputItemObject["contentType"].toInt());
        input.layout.top = inputItemObject["layout"].toObject()["top"].toInt();
        input.layout.left = inputItemObject["layout"].toObject()["left"].toInt();
        input.layout.right = inputItemObject["layout"].toObject()["right"].toInt();
        input.layout.bottom = inputItemObject["layout"].toObject()["bottom"].toInt();
        inputList.push_back(input);
    }

    std::vector<ZegoMixerOutput> outputList;
    for(QJsonValue inputItemValue: taskContent["outputList"].toArray()){
        ZegoMixerOutput output;
        QJsonObject outputItemObject = inputItemValue.toObject();
        output.target = outputItemObject["target"].toString().toStdString();
        outputList.push_back(output);
    }

    task.inputList = inputList;
    task.outputList = outputList;
    engine->startMixerTask(task, [=](ZegoMixerStartResult result){
        QString log = QString("start mixer task result: errorCode=%1").arg(result.errorCode);
        printLogToView(log);
    });
}

void ZegoApiMonckyDemo::on_pushButton_stop_mixer_task_clicked()
{
    GuardEngineLoaded

    std::string taskID = ui->lineEdit_mixer_taskID->text().toStdString();
    engine->stopMixerTask(taskID);
}

void ZegoApiMonckyDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoApiMonckyDemo::bindEventHandler()
{
    GuardEngineLoaded
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoApiMonckyDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoApiMonckyDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoApiMonckyDemo::onRoomUserUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoApiMonckyDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherStateUpdate, this, &ZegoApiMonckyDemo::onPublisherStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerStateUpdate, this, &ZegoApiMonckyDemo::onPlayerStateUpdate);

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigMixerRelayCDNStateUpdate, this, &ZegoApiMonckyDemo::onMixerRelayCDNStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigIMRecvBroadcastMessage, this, &ZegoApiMonckyDemo::onIMRecvBroadcastMessage);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigIMRecvCustomCommand, this, &ZegoApiMonckyDemo::onIMRecvCustomCommand);

    engine->addEventHandler(eventHandler);
}


void ZegoApiMonckyDemo::on_pushButton_sendBroadcastMessage_clicked()
{
    GuardEngineLoaded

    bool valiadJson = true;
    QString BroadcastMessageStr = ui->textEdit_sendBroadcastMessage->toPlainText();
    QJsonObject BroadcastMessage= ZegoUtilHelper::stringToJsonObject(BroadcastMessageStr, valiadJson);
    if(!valiadJson){
        printLogToView("please check BroadcastMessageContent");
        return;
    }

    QString roomID = BroadcastMessage["roomID"].toString();
    QString message = BroadcastMessage["message"].toString();
    engine->sendBroadcastMassage(roomID.toStdString(), message.toStdString(),  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_im_panel->append(QString("send broadcast message: roomID=%1, message=%2").arg(roomID).arg(message));
        }
        QString log = QString("send broadcast message: roomID=%1, message=%2, errorCode=%3").arg(roomID).arg(message).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoApiMonckyDemo::on_pushButton_sendCustomCommand_clicked()
{
    GuardEngineLoaded

    bool valiadJson = true;
    QString CustomCommandStr = ui->textEdit_sendCustomCommad->toPlainText();
    QJsonObject CustomCommand= ZegoUtilHelper::stringToJsonObject(CustomCommandStr, valiadJson);
    if(!valiadJson){
        printLogToView("please check BroadcastMessageContent");
        return;
    }

    QString roomID = CustomCommand["roomID"].toString();
    QString command = CustomCommand["command"].toString();

    std::vector<ZegoUser> toUserList;
    QJsonArray toUserListArray = CustomCommand["toUserList"].toArray();
    for(auto userValue: toUserListArray){
        QJsonObject userObject = userValue.toObject();
        QString userID = userObject["userID"].toString();
        QString userName = userObject["userName"].toString();
        ZegoUser user(userID.toStdString(), userName.toStdString());
        toUserList.push_back(user);
    }

    engine->sendCustomCommand(roomID.toStdString(),toUserList, command.toStdString(),  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_im_panel->append(QString("send custom Command: roomID=%1, command=%2").arg(roomID).arg(command));
        }
        QString log = QString("send custom Command: roomID=%1, message=%2, errorCode=%3").arg(roomID).arg(command).arg(errorCode);
        printLogToView(log);
    });
}
