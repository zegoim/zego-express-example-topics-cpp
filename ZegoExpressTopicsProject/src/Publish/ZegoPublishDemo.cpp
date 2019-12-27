#include "ZegoPublishDemo.h"
#include "ui_ZegoPublishDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoPublishDemo::ZegoPublishDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoPublishDemo)
{
    ui->setupUi(this);


    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
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
        "ZEGO_VIEW_MODE_ASPECT_FIT",
        "ZEGO_VIEW_MODE_ASPECT_FILL",
        "ZEGO_VIEW_MODE_SCALE_TO_FILL"
    };
    ui->comboBox_viewmode->addItems(viewModeList);
    ui->comboBox_viewmode->blockSignals(false);


    ui->comboBox_mirrormode->blockSignals(true);
    QStringList mirroModeList = {
        "ZEGO_VIDEO_MORROR_MODE_ONLY_PREVIEW_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_BOTH_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_NO_MIRROR",
        "ZEGO_VIDEO_MORROR_MODE_ONLY_PUBLISHER_MIRROR"
    };
    ui->comboBox_mirrormode->addItems(mirroModeList);
    ui->comboBox_mirrormode->blockSignals(false);


    ui->comboBox_videoConfig->blockSignals(true);
    QStringList ZegoResolutionList = {
        "ZEGO_RESOLUTION_320x180" ,
        "ZEGO_RESOLUTION_480x270" ,
        "ZEGO_RESOLUTION_640x360" ,
        "ZEGO_RESOLUTION_960x540" ,
        "ZEGO_RESOLUTION_1280x720",
        "ZEGO_RESOLUTION_1920x1080"
    };
    ui->comboBox_videoConfig->addItems(ZegoResolutionList);
    ui->comboBox_videoConfig->setCurrentIndex(ZEGO_RESOLUTION_640x360);
    ui->comboBox_videoConfig->blockSignals(false);

    ui->comboBox_audioConfig->blockSignals(true);
    QStringList ZegoAudioConfigPresetList = {
        "LOW_LATENCY_BASIC_QUALITY",
        "LOW_LATENCY_STANDARD_QUALITY",
        "LOW_LATENCY_STANDARD_QUALITY_STEREO" ,
        "LOW_LATENCY_HIGH_QUALITY",
        "LOW_LATENCY_HIGH_QUALITY_STEREO",
        "NORMAL_LATENCY_STANDARD_QUALITY",
        "NORMAL_LATENCY_STANDARD_QUALITY_STEREO",
        "NORMAL_LATENCY_HIGH_QUALITY",
        "NORMAL_LATENCY_HIGH_QUALITY_STEREO",
    };
    ui->comboBox_audioConfig->addItems(ZegoAudioConfigPresetList);
    ui->comboBox_audioConfig->setCurrentIndex(ZEGO_AUDIO_CONFIG_PRESET_LOW_LATENCY_STANDARD_QUALITY);
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
    ZegoExpressSDK::destroyEngine(engine);
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

void ZegoPublishDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPublisherQualityUpdate, this, &ZegoPublishDemo::onPublisherQualityUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPublisherVideoSizeChanged, this, &ZegoPublishDemo::onPublisherVideoSizeChanged);
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


    ZegoCanvas canvas((void *)ui->frame_local_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPreview(&canvas);
}

void ZegoPublishDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishing();
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
     ZegoCanvas canvas((void *)ui->frame_local_video->winId(), ZegoViewMode(index));
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
    ZegoVideoConfig videoConfig = ZegoVideoConfig(ZegoResolution(index));
    engine->setVideoConfig(videoConfig);
}


void ZegoPublishDemo::on_comboBox_audioConfig_currentIndexChanged(int index)
{
    ZegoAudioConfig audioConfig = ZegoAudioConfig(ZegoAudioConfigPreset(index));
    engine->setAudioConfig(audioConfig);

}
