#include "ZegoPublishDemo.h"
#include "ui_ZegoPublishDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoPublishDemo::ZegoPublishDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoPublishDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    ui->comboBox_microphone->blockSignals(true);
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_INPUT);
    for(auto device : audioList){
        ui->comboBox_microphone->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_microphone->blockSignals(false);

    ui->comboBox_camera->blockSignals(true);
    auto cameraList = engine->getVideoDeviceList();
    for(auto device : cameraList){
        ui->comboBox_camera->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_camera->blockSignals(false);


    ui->comboBox_viewmode->blockSignals(true);
    QStringList viewModeList = {
        "ASPECT_FIT",
        "ASPECT_FILL",
        "SCALE_TO_FILL"
    };
    ui->comboBox_viewmode->addItems(viewModeList);
    ui->comboBox_viewmode->blockSignals(false);


    ui->comboBox_mirrormode->blockSignals(true);
    QStringList mirroModeList = {
        "ONLY_PREVIEW_MIRROR",
        "BOTH_MIRROR",
        "NO_MIRROR",
        "ONLY_PUBLISHER_MIRROR"
    };
    ui->comboBox_mirrormode->addItems(mirroModeList);
    ui->comboBox_mirrormode->blockSignals(false);


    ui->comboBox_videoConfig->blockSignals(true);
    QStringList ZegoResolutionList = {
        "180P" ,
        "270P" ,
        "360P" ,
        "540P" ,
        "720P",
        "1080P"
    };
    ui->comboBox_videoConfig->addItems(ZegoResolutionList);
    ui->comboBox_videoConfig->setCurrentIndex(ZEGO_VIDEO_CONFIG_PRESET_360P);
    ui->comboBox_videoConfig->blockSignals(false);

    ui->comboBox_audioConfig->blockSignals(true);
    QStringList ZegoAudioConfigPresetList = {
        "BASIC_QUALITY",
        "STANDARD_QUALITY",
        "STANDARD_QUALITY_STEREO",
        "HIGH_QUALITY",
        "HIGH_QUALITY_STEREO"
    };
    ui->comboBox_audioConfig->addItems(ZegoAudioConfigPresetList);
    ui->comboBox_audioConfig->setCurrentIndex(ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY);
    ui->comboBox_audioConfig->blockSignals(false);


    ui->slider_captureVolume->blockSignals(true);
    ui->slider_captureVolume->setValue(100);
    ui->slider_captureVolume->blockSignals(false);

    ui->checkBox_enableCamera->blockSignals(true);
    ui->checkBox_enableCamera->setChecked(true);
    ui->checkBox_enableCamera->blockSignals(false);

    ui->checkBox_muteMicrophone->blockSignals(true);
    ui->checkBox_muteMicrophone->setChecked(false);
    ui->checkBox_muteMicrophone->blockSignals(false);
}

ZegoPublishDemo::~ZegoPublishDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoPublishDemo::onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) {
    Q_UNUSED(streamID)
    ui->lineEdit_videoBPS->setText(QString::number(quality.videoKBPS) + "kbps");
    ui->lineEdit_videoFPS->setText(QString::number(quality.videoSendFPS) + "fps");
    ui->lineEdit_audioBPS->setText(QString::number(quality.audioKBPS) + "kbps");
    ui->lineEdit_audioFPS->setText(QString::number(quality.audioSendFPS) + "fps");
}

void ZegoPublishDemo::onPublisherVideoSizeChanged(int width, int height)
{
    ui->lineEdit_resolution->setText(QString("%1x%2").arg(width).arg(height));
}

void ZegoPublishDemo::onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo &deviceInfo)
{
    Q_UNUSED(updateType)
    Q_UNUSED(deviceType)
    Q_UNUSED(deviceInfo)
    ui->comboBox_microphone->blockSignals(true);
    ui->comboBox_microphone->clear();
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_INPUT);
    for(auto device : audioList){
        ui->comboBox_microphone->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_microphone->blockSignals(false);
}

void ZegoPublishDemo::onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo &deviceInfo)
{
    Q_UNUSED(updateType)
    Q_UNUSED(deviceInfo)
    ui->comboBox_camera->blockSignals(true);
    ui->comboBox_camera->clear();
    auto cameraList = engine->getVideoDeviceList();
    for(auto device : cameraList){
        ui->comboBox_camera->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_camera->blockSignals(false);
}

void ZegoPublishDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoPublishDemo::bindEventHandler()
{    
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoPublishDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPublisherQualityUpdate, this, &ZegoPublishDemo::onPublisherQualityUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPublisherVideoSizeChanged, this, &ZegoPublishDemo::onPublisherVideoSizeChanged);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigVideoDeviceStateChanged, this, &ZegoPublishDemo::onVideoDeviceStateChanged);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigAudioDeviceStateChanged, this, &ZegoPublishDemo::onAudioDeviceStateChanged);
    engine->setEventHandler(eventHandler);
}

void ZegoPublishDemo::on_pushButton_startPublish_clicked()
{
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    currentRoomID = ui->lineEdit_roomID->text().toStdString();

    ZegoUser user;
    user.userID = userID;
    user.userName = userID;
    engine->loginRoom(currentRoomID, user);
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()));
    engine->startPreview(&canvas);
}

void ZegoPublishDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
    engine->logoutRoom(currentRoomID);
}

void ZegoPublishDemo::on_comboBox_camera_currentIndexChanged(const QString &arg1)
{
    engine->useVideoDevice(arg1.toStdString());
}

void ZegoPublishDemo::on_comboBox_microphone_currentIndexChanged(const QString &arg1)
{
   engine->useAudioDevice(ZEGO_AUDIO_DEVICE_TYPE_INPUT, arg1.toStdString());
}

void ZegoPublishDemo::on_comboBox_viewmode_currentIndexChanged(int index)
{
     ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()), ZegoViewMode(index));
     engine->startPreview(&canvas);
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

void ZegoPublishDemo::on_checkBox_muteMicrophone_clicked(bool checked)
{
    engine->muteMicrophone(checked);
}

void ZegoPublishDemo::on_checkBox_enableCamera_clicked(bool checked)
{
    engine->enableCamera(checked);
}

void ZegoPublishDemo::on_comboBox_videoConfig_currentIndexChanged(int index)
{
    ZegoVideoConfig videoConfig = ZegoVideoConfig(ZegoVideoConfigPreset(index));
    engine->setVideoConfig(videoConfig);
}


void ZegoPublishDemo::on_comboBox_audioConfig_currentIndexChanged(int index)
{
    ZegoAudioConfig audioConfig = ZegoAudioConfig(ZegoAudioConfigPreset(index));
    engine->setAudioConfig(audioConfig);
}
