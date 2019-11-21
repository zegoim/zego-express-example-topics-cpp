#include "ZegoQuickStartDemo.h"
#include "ui_ZegoQuickStartDemo.h"


#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerQt.h"

ZegoQuickStartDemo::ZegoQuickStartDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoQuickStartDemo)
{
    ui->setupUi(this);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    ui->label_appID->setText(QString::number(appID));
    ui->label_appSign->setText(appSign.c_str());
    ui->radioButton_isTestEnv->setChecked(isTestEnv);

    ui->label_roomID->setText("QuickStartRoom-1");

    std::string userID = ZegoUtilHelper::getRandomString();
    ui->label_userID->setText(userID.c_str());
    ui->label_userName->setText(userID.c_str());
}

ZegoQuickStartDemo::~ZegoQuickStartDemo()
{
    if(engine){
        ZegoExpressEngine::destroyEngine(engine);
        engine = nullptr;
    }
    delete ui;
}

void ZegoQuickStartDemo::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoQuickStartDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%2").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);
}

void ZegoQuickStartDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList streamIDs;
    for (const ZegoStream &stream : streamList) {
        streamIDs.append(QString::fromStdString(stream.streamID));
    }
    QString log = QString("onRoomStreamUpdate: roomID=%1, updateType=%2, streamIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(streamIDs.join(","));
    printLogToView(log);
}

void ZegoQuickStartDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoQuickStartDemo::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errCode) {
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NOPUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISHING",
        "ZEGO_PUBLISHER_STATE_PUBLISHED"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoQuickStartDemo::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errCode)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoQuickStartDemo::on_pushButton_createEngine_clicked()
{    
    if(engine == nullptr){
        auto appID = ui->label_appID->text().toUInt();
        auto appSign = ui->label_appSign->text().toStdString();
        auto isTestEnv = ui->radioButton_isTestEnv->isChecked();

        engine = ZegoExpressEngine::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
        bindEventHandler();

        QString log = QString("do createEngine");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_loginRoom_clicked()
{
    if(engine != nullptr){
        std::string roomID = ui->label_roomID->text().toStdString();

        ZegoUser user;
        user.userID = ui->label_userID->text().toStdString();
        user.userName = ui->label_userName->text().toStdString();

        engine->loginRoom(roomID, user, nullptr);
        QString log = QString("do loginRoom");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_PublishStream_clicked()
{
    if(engine != nullptr){
        std::string streamID = ui->lineEdit_publish_streamID->text().toStdString();
        if(streamID == ""){
            QString log = QString("Input publish streamID needed");
            printLogToView(log);
            return;
        }

        engine->startPublishing(streamID);

        ZegoCanvas canvas((void*)ui->frame_local_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
        engine->startPreview(&canvas);

        QString log = QString("do publish stream");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_PlayStream_clicked()
{
    if(engine != nullptr){
        std::string streamID = ui->lineEdit_play_streamID->text().toStdString();
        if(streamID == ""){
            QString log = QString("Input play streamID needed");
            printLogToView(log);
            return;
        }

        ZegoCanvas canvas((void*)ui->frame_remote_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
        engine->startPlayingStream(streamID, &canvas);

        QString log = QString("do  play stream");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_destroyEngine_clicked()
{
    if(engine){
        // fix here
        // engine->stopPreview();
        QString log = QString("do destroy engine");
        printLogToView(log);
        ZegoExpressEngine::destroyEngine(engine);
        engine = nullptr;
    }
}

void ZegoQuickStartDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoQuickStartDemo::bindEventHandler()
{
    if(engine != nullptr){
        auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

        connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoQuickStartDemo::onDebugError);
        connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoQuickStartDemo::onRoomStateUpdate);
        connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoQuickStartDemo::onRoomUserUpdate);
        connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoQuickStartDemo::onRoomStreamUpdate);
        connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherStateUpdate, this, &ZegoQuickStartDemo::onPublisherStateUpdate);
        connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerStateUpdate, this, &ZegoQuickStartDemo::onPlayerStateUpdate);

        engine->addEventHandler(eventHandler);
    }
}
