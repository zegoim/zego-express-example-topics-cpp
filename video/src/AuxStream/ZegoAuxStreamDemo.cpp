#include "ZegoAuxStreamDemo.h"
#include "ui_ZegoAuxStreamDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoAuxStreamDemo::ZegoAuxStreamDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoAuxStreamDemo)
{
    ui->setupUi(this);

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

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    ui->comboBox_camera->blockSignals(true);
    auto cameraList = engine->getVideoDeviceList();
    for(auto device : cameraList){
        ui->comboBox_camera->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_camera->blockSignals(false);

    currentRoomID = "AuxStreamRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
}

ZegoAuxStreamDemo::~ZegoAuxStreamDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoAuxStreamDemo::on_pushButton_startPublish_1_clicked()
{
    ZegoCanvas canvas((void*)ui->frame_local_preview_1->winId());
    engine->startPreview(&canvas, ZEGO_PUBLISH_CHANNEL_MAIN);

    std::string streamID = ui->lineEdit_streamID_1->text().toStdString();
    engine->startPublishingStream(streamID, ZEGO_PUBLISH_CHANNEL_MAIN);
}

void ZegoAuxStreamDemo::on_pushButton_stopPublish_1_clicked()
{
    engine->stopPreview(ZEGO_PUBLISH_CHANNEL_MAIN);
    engine->stopPublishingStream(ZEGO_PUBLISH_CHANNEL_MAIN);
}

void ZegoAuxStreamDemo::on_pushButton_startPublish_2_clicked()
{
    ZegoCanvas canvas((void*)ui->frame_local_preview_2->winId());
    engine->startPreview(&canvas, ZEGO_PUBLISH_CHANNEL_AUX);

    std::string streamID = ui->lineEdit_streamID_2->text().toStdString();
    engine->startPublishingStream(streamID, ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_pushButton_stopPublish_2_clicked()
{
    engine->stopPreview(ZEGO_PUBLISH_CHANNEL_AUX);
    engine->stopPublishingStream(ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoAuxStreamDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoAuxStreamDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigVideoDeviceStateChanged, this, &ZegoAuxStreamDemo::onVideoDeviceStateChanged);
    engine->setEventHandler(eventHandler);
}

void ZegoAuxStreamDemo::on_comboBox_mirrormode_currentIndexChanged(int index)
{
    engine->setVideoMirrorMode(ZegoVideoMirrorMode(index), ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_comboBox_videoConfig_currentIndexChanged(int index)
{
    ZegoVideoConfig config = ZegoVideoConfig(ZegoVideoConfigPreset(index));
    engine->setVideoConfig(config, ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_checkBox_mutePublishStreamVideo_clicked(bool checked)
{
    engine->mutePublishStreamVideo(checked, ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_checkBox_mutePublishStreamAudio_clicked(bool checked)
{
    engine->mutePublishStreamAudio(checked, ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_comboBox_camera_currentIndexChanged(const QString &arg1)
{
    engine->useVideoDevice(arg1.toStdString(), ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo &deviceInfo)
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
