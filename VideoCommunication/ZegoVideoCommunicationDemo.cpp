#include "ZegoVideoCommunicationDemo.h"
#include "ui_ZegoVideoCommunicationDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerQt.h"

ZegoVideoCommunicationDemo::ZegoVideoCommunicationDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoVideoCommunicationDemo)
{
    ui->setupUi(this);

    // 创建engine
    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTest = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressEngine::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();
}

ZegoVideoCommunicationDemo::~ZegoVideoCommunicationDemo()
{
    // 销毁engine
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoVideoCommunicationDemo::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoVideoCommunicationDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%2").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);
}

void ZegoVideoCommunicationDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    Q_UNUSED(roomID)
    if(updateType == ZEGO_UPDATE_TYPE_ADD){
        for_each(streamList.begin(), streamList.end(), [&](ZegoStream stream){

            auto it  = std::find_if(zegoStreamList.begin(), zegoStreamList.end(), [&](ZegoStream const &_stream){
                    return _stream.streamID == stream.streamID;
                } );

            if(it == zegoStreamList.end()){
                zegoStreamList.push_back(stream);
            }

        });

    }
    else {
        for_each(streamList.begin(), streamList.end(), [&](ZegoStream stream){

            engine->stopPlayingStream(stream.streamID);

            auto it  = std::find_if(zegoStreamList.begin(), zegoStreamList.end(), [&](ZegoStream const &_stream){
                    return _stream.streamID == stream.streamID;
                } );

            if(it != zegoStreamList.end()){
                zegoStreamList.erase(it);
            }

        });
    }

    // 重新布局
    QList<QWidget*> videoList = {
        ui->frame_video_1,
        ui->frame_video_2,
        ui->frame_video_3,
        ui->frame_video_4,
        ui->frame_video_5,
        ui->frame_video_6,
        ui->frame_video_7,
        ui->frame_video_8,
        ui->frame_video_9,
        ui->frame_video_10,
        ui->frame_video_11,
    };


    for(auto widget : videoList){
        widget->update();
    }


    QStringList totalStreamList;
    for (unsigned int i = 0; i< zegoStreamList.size(); i++) {
        totalStreamList.append(QString::fromStdString(zegoStreamList.at(i).streamID));
    }
    printLogToView(QString("CurrentStreamList: %1").arg(totalStreamList.join(", ")));


    for (unsigned int i = 0; i< zegoStreamList.size(); i++) {
        // 最多拉12条流
        if(i>=11){
            break;
        }
        auto stream = zegoStreamList.at(i);
        auto widget = videoList.at(int(i));
        ZegoCanvas canvas((void *)widget->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
        engine->startPlayingStream(stream.streamID, &canvas);
    }
}

void ZegoVideoCommunicationDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoVideoCommunicationDemo::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errCode) {
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NOPUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISHING",
        "ZEGO_PUBLISHER_STATE_PUBLISHED"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoVideoCommunicationDemo::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errCode)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoVideoCommunicationDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
    ui->textEdit_log->append("\n");
}

void ZegoVideoCommunicationDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoVideoCommunicationDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoVideoCommunicationDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoVideoCommunicationDemo::onRoomUserUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoVideoCommunicationDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherStateUpdate, this, &ZegoVideoCommunicationDemo::onPublisherStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerStateUpdate, this, &ZegoVideoCommunicationDemo::onPlayerStateUpdate);

    engine->addEventHandler(eventHandler);
}

void ZegoVideoCommunicationDemo::on_pushButton_login_clicked()
{
    ZegoUser user;
    user.userID = ZegoConfigManager::instance()->getRandomUserID();
    user.userName = user.userID;
    engine->loginRoom(ui->lineEdit_roomID->text().toStdString(), user,  nullptr);

    ZegoCanvas canvas((void *)ui->frame_video_0->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPreview(&canvas);
    engine->startPublishing(user.userID);
}

void ZegoVideoCommunicationDemo::on_pushButton_logout_clicked()
{
    engine->logoutRoom(ui->lineEdit_roomID->text().toStdString());
    zegoStreamList.clear();
}

void ZegoVideoCommunicationDemo::on_pushButton_clear_log_clicked()
{
    ui->textEdit_log->clear();
}
