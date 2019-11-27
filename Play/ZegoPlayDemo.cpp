#include "ZegoPlayDemo.h"
#include "ui_ZegoPlayDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerQt.h"

#include <QScrollBar>

ZegoPlayDemo::ZegoPlayDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoPlayDemo)
{
    ui->setupUi(this);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTest = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressEngine::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    ui->comboBox_audioOutputDevice->blockSignals(true);
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT);
    for(auto device : audioList){
        ui->comboBox_audioOutputDevice->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_audioOutputDevice->blockSignals(false);

    ui->comboBox_viewmode->blockSignals(true);
    QStringList viewModeList = {
        "ZEGO_VIEW_MODE_ASPECT_FIT",
        "ZEGO_VIEW_MODE_ASPECT_FILL",
        "ZEGO_VIEW_MODE_SCALE_TO_FILL"
    };
    ui->comboBox_viewmode->addItems(viewModeList);
    ui->comboBox_viewmode->blockSignals(false);

    ui->slider_playVolume->blockSignals(true);
    ui->slider_playVolume->setValue(100);
    ui->slider_playVolume->blockSignals(false);
}

ZegoPlayDemo::~ZegoPlayDemo()
{
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoPlayDemo::onDebugError(int errorCode, const std::string& funcName, const std::string& info){
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoPlayDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode){
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoPlayDemo::onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errCode)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoPlayDemo::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    //printLogToView(QString("onPlayerQualityUpdate: streamID=%1").arg(QString::fromStdString(streamID)));
    Q_UNUSED(streamID)
    ui->lineEdit_videoBPS->setText(QString::number(quality.videoKBPS) + "kbps");
    ui->lineEdit_videoFPS->setText(QString::number(quality.videoRecvFPS) + "fps");
    ui->lineEdit_resolution->setText(QString("%1x%2").arg(quality.width).arg(quality.height));

    ui->lineEdit_audioBPS->setText(QString::number(quality.audioKBPS) + "kbps");
    ui->lineEdit_audioFPS->setText(QString::number(quality.audioRecvFPS) + "fps");
}

void ZegoPlayDemo::onPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent mediaEvent)
{
    Q_UNUSED(streamID);
    Q_UNUSED(mediaEvent);
}

void ZegoPlayDemo::onPlayerRecvFirstFrameEvent(const std::string &streamID, ZegoPlayerFirstFrameEvent firstFrameEvent)
{
    Q_UNUSED(streamID)

    QStringList firstFrameEventExplain = {
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_AUDIO_RCV",
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RCV",
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDER"
    };
    QString log = QString("onPlayerFirstFrameEvent: firstFrameEvent=%1").arg(firstFrameEventExplain.value(firstFrameEvent));
    printLogToView(log);
}

void ZegoPlayDemo::onPlayerVideoSizeChanged(const std::string &streamID, int width, int height)
{
    QString log = QString("onPlayerVideoSizeChanged: streamID=%1, width=%2, height=%3").arg(streamID.c_str()).arg(width).arg(height);
    printLogToView(log);
}

void ZegoPlayDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList) {
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList streamIDs;
    for(const ZegoStream &stream : streamList){
        streamIDs.append(QString::fromStdString(stream.streamID));
    }
    QString log = QString("onStreamUpdate: roomID=%1, updateType=%2, streamIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(streamIDs.join(","));
    printLogToView(log);
}

void ZegoPlayDemo::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList){
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);
}

void ZegoPlayDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoPlayDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoPlayDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoPlayDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoPlayDemo::onRoomUserUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoPlayDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerStateUpdate, this, &ZegoPlayDemo::onPlayerStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerQualityUpdate, this, &ZegoPlayDemo::onPlayerQualityUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerMediaEvent, this, &ZegoPlayDemo::onPlayerMediaEvent);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerRecvFirstFrameEvent, this, &ZegoPlayDemo::onPlayerRecvFirstFrameEvent);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPlayerVideoSizeChanged, this, &ZegoPlayDemo::onPlayerVideoSizeChanged);

    engine->addEventHandler(eventHandler);
}

void ZegoPlayDemo::on_pushButton_clear_log_clicked()
{
    ui->textEdit_log->clear();
}

void ZegoPlayDemo::on_comboBox_audioOutputDevice_currentIndexChanged(const QString &arg1)
{
    engine->useAudioDevice(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT, arg1.toStdString());
}

void ZegoPlayDemo::on_comboBox_viewmode_currentIndexChanged(int index)
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas((void *)ui->frame_remote_video->winId(), ZegoViewMode(index));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoPlayDemo::on_checkBox_mutePlayStreamAudio_clicked(bool checked)
{
    engine->mutePlayStreamAudio(ui->lineEdit_streamID->text().toStdString(), checked);
}

void ZegoPlayDemo::on_checkBox_mutePlayStreamVideo_clicked(bool checked)
{
    engine->mutePlayStreamVideo(ui->lineEdit_streamID->text().toStdString(), checked);
}

void ZegoPlayDemo::on_pushButton_startPlay_clicked()
{
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    std::string streamID = ui->lineEdit_streamID->text().toStdString();

    ZegoUser user;
    user.userID = userID;
    user.userName = userID;
    engine->loginRoom(roomID, user,  nullptr);

    ZegoCanvas canvas((void *)ui->frame_remote_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoPlayDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);

    ui->slider_playVolume->setValue(100);
    ui->checkBox_mutePlayStreamAudio->setChecked(false);
    ui->checkBox_mutePlayStreamVideo->setChecked(false);
}
