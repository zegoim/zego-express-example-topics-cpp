#include "ZegoPublishDemo.h"
#include "ui_ZegoPublishDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerQt.h"

ZegoPublishDemo::ZegoPublishDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoPublishDemo)
{
    ui->setupUi(this);

    /*
    videoConfig = ZegoVideoConfig(ZEGO_RESOLUTION_540x960);
    */

    // 创建engine
    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTest = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressEngine::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    // 初始化界面显示
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_INPUT);
    for(auto device : audioList){
        ui->comboBox_microphone->addItem(QString::fromStdString(device.deviceID));
    }

    auto cameraList = engine->getVideoDeviceList();
    for(auto device : cameraList){
        ui->comboBox_camera->addItem(QString::fromStdString(device.deviceID));
    }

    QStringList viewModeList = {
        "ZEGO_VIEW_MODE_ASPECT_FIT",
        "ZEGO_VIEW_MODE_ASPECT_FILL",
        "ZEGO_VIEW_MODE_SCALE_TO_FILL"
    };
    ui->comboBox_viewmode->addItems(viewModeList);

    QStringList mirroModeList = {
        "ZEGO_VIDEO_MORROR_MODE_ONLY_PREVIEW_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_BOTH_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_NO_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_ONLY_PUBLISHER_MIRROR"
    };
    ui->comboBox_mirrormode->addItems(mirroModeList);


    QStringList videoResolutionList = {
        "ZEGO_RESOLUTION_180x320",
        "ZEGO_RESOLUTION_270x480",
        "ZEGO_RESOLUTION_360x640",
        "ZEGO_RESOLUTION_540x960",
        "ZEGO_RESOLUTION_720x1480",
        "ZEGO_RESOLUTION_1080x1920"
    };
    ui->comboBox_videoConfig->addItems(videoResolutionList);


    ui->slider_captureVolume->setValue(100);
    ui->checkBox_enableCamera->setChecked(true);
    ui->checkBox_enableMicrophone->setChecked(true);

    // 开始预览
    ZegoCanvas canvas((void *)ui->frame_local_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPreview(&canvas);
}

ZegoPublishDemo::~ZegoPublishDemo()
{
    // 销毁engine
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoPublishDemo::onDebugError(int errorCode, const std::string& funcName, const std::string& info){
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoPublishDemo::onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode){
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoPublishDemo::onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) {
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList streamIDs;
    for(const ZegoStream &stream : streamList){
        streamIDs.append(QString::fromStdString(stream.streamID));
    }
    QString log = QString("onStreamUpdate: roomID=%1, updateType=%2, streamIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(streamIDs.join(","));
    printLogToView(log);
}

void ZegoPublishDemo::onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList){
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%2").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);
}

void ZegoPublishDemo::onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errCode) {
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NOPUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISHING",
        "ZEGO_PUBLISHER_STATE_PUBLISHED"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errCode);
    printLogToView(log);
}

void ZegoPublishDemo::onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) {
    //printLogToView(QString("onPublisherQualityUpdate: streamID=%1").arg(QString::fromStdString(streamID)));
    Q_UNUSED(streamID)
    ui->lineEdit_videoBPS->setText(QString::number(quality.videoKBPS) + "kbps");
    ui->lineEdit_videoFPS->setText(QString::number(quality.videoSendFPS) + "fps");
    ui->lineEdit_resolution->setText(QString("%1x%2").arg(quality.width).arg(quality.height));

    ui->lineEdit_audioBPS->setText(QString::number(quality.audioKBPS) + "kbps");
    ui->lineEdit_audioFPS->setText(QString::number(quality.audioSendFPS) + "fps");
}

void ZegoPublishDemo::onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent firstFrameEvent) {
    QStringList firstFrameEventExplain = {
        "ZEGO_PUBLISHER_FIRST_FRAME_EVENT_AUDIO_CAPTURED",
        "ZEGO_PUBLISHER_FIRST_FRAME_EVENT_VIDEO_CAPTURED"
    };

    QString log = QString("onPublisherFirstFrameEvent: firstFrameEvent=%1").arg(firstFrameEventExplain.value(firstFrameEvent));
    printLogToView(log);
}


void ZegoPublishDemo::onPublisherVideoSizeChanged(int width, int height) {
    QString log = QString("onPublisherVideoSizeChanged: width=%1, height=%2").arg(width).arg(height);
    printLogToView(log);
}

void ZegoPublishDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
    ui->textEdit_log->append("\n");
}

void ZegoPublishDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoPublishDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoPublishDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoPublishDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomUserUpdate, this, &ZegoPublishDemo::onRoomUserUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherStateUpdate, this, &ZegoPublishDemo::onPublisherStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherQualityUpdate, this, &ZegoPublishDemo::onPublisherQualityUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherRecvFirstFrameEvent, this, &ZegoPublishDemo::onPublisherRecvFirstFrameEvent);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigPublisherVideoSizeChanged, this, &ZegoPublishDemo::onPublisherVideoSizeChanged);

    engine->addEventHandler(eventHandler);
}

void ZegoPublishDemo::on_pushButton_startPublish_clicked()
{
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string roomID = ui->lineEdit_roomID->text().toStdString();
    std::string streamID = ui->lineEdit_streamID->text().toStdString();

    ZegoUser user;
    user.userID = userID;
    user.userName = userID;
    engine->loginRoom(roomID, user,  nullptr);

    engine->startPublishing(streamID);
}

void ZegoPublishDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPublishing();
}

void ZegoPublishDemo::on_pushButton_clear_log_clicked()
{
    ui->textEdit_log->clear();
}

void ZegoPublishDemo::on_comboBox_camera_currentIndexChanged(const QString &arg1)
{
    engine->useAudioDevice(ZEGO_AUDIO_DEVICE_TYPE_INPUT, arg1.toStdString());
}

void ZegoPublishDemo::on_comboBox_microphone_currentIndexChanged(const QString &arg1)
{
    engine->useVideoDevice(arg1.toStdString());
}

void ZegoPublishDemo::on_comboBox_viewmode_currentIndexChanged(int index)
{
     ZegoCanvas canvas((void *)ui->frame_local_video->winId(), ZegoViewMode(index));
     engine->updatePreviewView(&canvas);
}

void ZegoPublishDemo::on_comboBox_mirrormode_currentIndexChanged(int index)
{
    engine->setVideoMirrorMode(ZegoVideoMirrorMode(index));
}

void ZegoPublishDemo::on_checkBox_mutePublishStreamAudio_clicked(bool checked)
{
    engine->mutePublishStreamAudio(checked);
}

void ZegoPublishDemo::on_checkBox_mutePublishStreamVideo_clicked(bool checked)
{
    engine->mutePublishStreamVideo(checked);
}

void ZegoPublishDemo::on_slider_captureVolume_valueChanged(int value)
{
    engine->setCaptureVolume(value);
}

void ZegoPublishDemo::on_checkBox_enableMicrophone_clicked(bool checked)
{
    engine->enableMicrophone(checked);
}

void ZegoPublishDemo::on_checkBox_enableCamera_clicked(bool checked)
{
    engine->enableCamera(checked);
}

void ZegoPublishDemo::on_comboBox_videoConfig_currentIndexChanged(int index)
{
    /*
    videoConfig = ZegoVideoConfig(ZegoResolutionType(index));

    engine->setVideoConfig(videoConfig);
    ui->spinBox_videoBPS->setValue(videoConfig.bitrate);
    ui->spinBox_videoFPS->setValue(videoConfig.fps);
    */
}

void ZegoPublishDemo::on_spinBox_videoFPS_valueChanged(int arg1)
{
    videoConfig.fps = arg1;
    engine->setVideoConfig(videoConfig);
}

void ZegoPublishDemo::on_spinBox_videoBPS_valueChanged(int arg1)
{
    videoConfig.bitrate = arg1;
    engine->setVideoConfig(videoConfig);
}

void ZegoPublishDemo::on_spinBox_audioBPS_valueChanged(int arg1)
{
    engine->setAudioBitrate(arg1);
}
