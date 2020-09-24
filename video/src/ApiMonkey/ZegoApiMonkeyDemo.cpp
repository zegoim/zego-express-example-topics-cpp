#include "ZegoApiMonkeyDemo.h"
#include "ui_ZegoApiMonkeyDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

#include <QDebug>
#include <QThread>
#include <QScrollBar>

#define GuardEngineLoaded \
    if(engine==nullptr) { \
        QString log = QString("Engine Not loaded"); \
        printLogToView(log); \
        return; \
    }

ZegoApiMonkeyDemo::ZegoApiMonkeyDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoApiMonkeyDemo)
{
    ui->setupUi(this);

    std::string roomID = ZegoUtilHelper::getRandomString();
    ui->lineEdit_roomID->setText(roomID.c_str());

    std::string userID = ZegoUtilHelper::getRandomString();
    ui->lineEdit_userID->setText(userID.c_str());
    ui->lineEdit_userName->setText(userID.c_str());

    bool ok = true;
    {
    std::string mixerTaskContent =
R"({
    "backgroundImageURL": "",
    "inputList": [{
        "contentType": 1,
        "layout": {
            "x": 0,
            "y": 0,
            "width": 320,
            "height": 360
        },
        "streamID": "stream1"
    }, {
        "contentType": 1,
        "layout": {
            "x": 320,
            "y": 0,
            "width": 320,
            "height": 360
        },
        "streamID": "stream2"
    }],
    "outputList": [{
        "target": "stream3"
    }],
    "watermark": {
        "imageURL": "",
        "layout": {
            "x": 10,
            "y": 10,
            "width": 100,
            "height": 100
        }
    }
})";

    ui->textEdit_mixer_taskContent->setText(QString::fromStdString(mixerTaskContent));

    ui->textEdit_im_panel->append("Welctom to Zego IM\n");

    QString imBroadcastMessage("{\"roomID\":\"room1\",\"message\":\"message\"}");
    ui->textEdit_sendBroadcastMessage->setText(ZegoUtilHelper::jsonObjectToString(ZegoUtilHelper::stringToJsonObject(imBroadcastMessage, ok)));

    QString imCustomCommand("{\"roomID\":\"room1\",\"command\":\"command\",\"toUserList\":[{\"userID\":\"userID\",\"userName\":\"userName\"}]}");
    ui->textEdit_sendCustomCommad->setText(ZegoUtilHelper::jsonObjectToString(ZegoUtilHelper::stringToJsonObject(imCustomCommand, ok)));
    }

    ZegoEngineConfig engineConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();
    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();
}

ZegoApiMonkeyDemo::~ZegoApiMonkeyDemo()
{
    QString log = QString("do destroyEngine");
    printLogToView(log);

    ZegoExpressSDK::destroyEngine(engine);
    delete ui;
}


void ZegoApiMonkeyDemo::on_pushButton_login_clicked()
{
    GuardEngineLoaded
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    ZegoUser user;
    user.userID = ui->lineEdit_userID->text().toStdString();
    user.userName = ui->lineEdit_userName->text().toStdString();

    engine->loginRoom(roomID, user);
    QString log = QString("do loginRoom");
    printLogToView(log);

}

void ZegoApiMonkeyDemo::on_pushButton_logout_clicked()
{
    GuardEngineLoaded
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    engine->logoutRoom(roomID);
    QString log = QString("do logoutRoom");
    printLogToView(log);
}

void ZegoApiMonkeyDemo::on_pushButton_start_publish_clicked()
{
    GuardEngineLoaded
    std::string streamID = ui->lineEdit_publish_streamID->text().toStdString();
    if(streamID == ""){
        QString log = QString("Input publish streamID needed");
        printLogToView(log);
        return;
    }

    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()));
    engine->startPreview(&canvas);

    QString log = QString("do publish stream");
    printLogToView(log);

}

void ZegoApiMonkeyDemo::on_pushButton_stop_publish_clicked()
{
    GuardEngineLoaded
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoApiMonkeyDemo::on_pushButton_setExtraInfo_clicked()
{
    GuardEngineLoaded
    std::string extraInfo = ui->lineEdit_setExtraInfo->text().toStdString();
    engine->setStreamExtraInfo(extraInfo, [=](int errorCode){
        QString log = QString("set extra info result: errorCode=%1").arg(errorCode);
        printLogToView(log);
    });
}

void ZegoApiMonkeyDemo::on_pushButton_start_play_clicked()
{
    GuardEngineLoaded
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    std::string streamID = ui->lineEdit_play_streamID->text().toStdString();

    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoApiMonkeyDemo::on_pushButton_stop_play_clicked()
{
    GuardEngineLoaded
    std::string streamID = ui->lineEdit_play_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);

    ui->slider_playVolume->setValue(100);
    ui->checkBox_mutePlayStreamAudio->setChecked(false);
    ui->checkBox_mutePlayStreamVideo->setChecked(false);
}

void ZegoApiMonkeyDemo::on_pushButton_start_mixer_task_clicked()
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
    QJsonObject taskContentObject= ZegoUtilHelper::stringToJsonObject(taskContentStr, valiadJson);
    if(!valiadJson){
        printLogToView("please check taskContent");
        return;
    }

    if(taskContentObject.contains("inputList")){
        std::vector<ZegoMixerInput> inputList;
        for(QJsonValue inputItemValue: taskContentObject["inputList"].toArray()){
            ZegoMixerInput input;
            QJsonObject inputItemObject = inputItemValue.toObject();
            input.streamID = inputItemObject["streamID"].toString().toStdString();
            input.contentType = ZegoMixerInputContentType(inputItemObject["contentType"].toInt());
            input.layout.x = inputItemObject["layout"].toObject()["x"].toInt();
            input.layout.y = inputItemObject["layout"].toObject()["y"].toInt();
            input.layout.width = inputItemObject["layout"].toObject()["width"].toInt();
            input.layout.height = inputItemObject["layout"].toObject()["height"].toInt();
            inputList.push_back(input);
        }
        task.inputList = inputList;
    }

    if(taskContentObject.contains("inputList")){
        std::vector<ZegoMixerOutput> outputList;
        for(QJsonValue inputItemValue: taskContentObject["outputList"].toArray()){
            ZegoMixerOutput output;
            QJsonObject outputItemObject = inputItemValue.toObject();
            output.target = outputItemObject["target"].toString().toStdString();
            outputList.push_back(output);
        }
        task.outputList = outputList;
    }

    if(taskContentObject.contains("backgroundImageURL")){
        std::string backgroundImageURL = taskContentObject["backgroundImageURL"].toString().toStdString();
        task.backgroundImageURL = backgroundImageURL;
    }

    std::shared_ptr<ZegoWatermark> watermark = std::make_shared<ZegoWatermark>();
    if(taskContentObject.contains("watermark")){
        QJsonObject watermarkObject = taskContentObject["watermark"].toObject();
        watermark->imageURL = watermarkObject["imageURL"].toString().toStdString();
        watermark->layout.x = watermarkObject["layout"].toObject()["x"].toInt();
        watermark->layout.y = watermarkObject["layout"].toObject()["y"].toInt();
        watermark->layout.width = watermarkObject["layout"].toObject()["width"].toInt();
        watermark->layout.height = watermarkObject["layout"].toObject()["height"].toInt();
        task.watermark = watermark.get();
    }
    else{
        task.watermark = nullptr;
    }

    engine->startMixerTask(task, nullptr);
}

void ZegoApiMonkeyDemo::on_pushButton_stop_mixer_task_clicked()
{
    GuardEngineLoaded

    std::string taskID = ui->lineEdit_mixer_taskID->text().toStdString();
    engine->stopMixerTask(taskID,nullptr);
}

void ZegoApiMonkeyDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoApiMonkeyDemo::bindEventHandler()
{
    GuardEngineLoaded
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPlayerRecvSEI, this, &ZegoApiMonkeyDemo::onPlayerRecvSEI);
    engine->setEventHandler(eventHandler);
}

void ZegoApiMonkeyDemo::on_pushButton_sendBroadcastMessage_clicked()
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
    engine->sendBroadcastMessage(roomID.toStdString(), message.toStdString(),  [=](int errorCode, unsigned long long messageID){
        if(errorCode==0){
            ui->textEdit_im_panel->append(QString("send broadcast message: roomID=%1, message=%2, messageID").arg(roomID).arg(message).arg(messageID));
        }
        QString log = QString("send broadcast message: roomID=%1, message=%2, errorCode=%3").arg(roomID).arg(message).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoApiMonkeyDemo::on_pushButton_sendCustomCommand_clicked()
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

    engine->sendCustomCommand(roomID.toStdString(), command.toStdString(), toUserList,  [=](int errorCode){
        if(errorCode==0){
            ui->textEdit_im_panel->append(QString("send custom Command: roomID=%1, command=%2").arg(roomID).arg(command));
        }
        QString log = QString("send custom Command: roomID=%1, message=%2, errorCode=%3").arg(roomID).arg(command).arg(errorCode);
        printLogToView(log);
    });
}

void ZegoApiMonkeyDemo::on_pushButton_sendSEI_clicked()
{
    QString seiString = ui->lineEdit_sendSEI->text();
    auto seiLength = (unsigned int)(strlen(seiString.toStdString().c_str()) + 1);
    unsigned char* test2 = new u_char[seiLength];
    memcpy(test2, seiString.toStdString().c_str(), seiLength);
    engine->sendSEI(test2,  seiLength);
    delete [] test2;
}

void ZegoApiMonkeyDemo::onPlayerRecvSEI(const std::string &streamID, const unsigned char *data, unsigned int dataLength)
{
    std::string sei = (char*)data;
    if(dataLength == (strlen(sei.c_str())+1)){
        QString log = QString("onPlayerRecvSEI: streamID=%1, sei=%2").arg(streamID.c_str()).arg(sei.c_str());
        printLogToView(log);
    }
}
