#include "ZegoAuxStreamDemo.h"
#include "ui_ZegoAuxStreamDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "AppSupport/ZegoConfigManager.h"
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

    ZegoEngineConfig engineConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    roomID = "AuxStreamRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(roomID, user);
}

ZegoAuxStreamDemo::~ZegoAuxStreamDemo()
{
    ZegoExpressSDK::destroyEngine(engine);
    delete ui;
}

void ZegoAuxStreamDemo::on_pushButton_startPublish_1_clicked()
{
    ZegoCanvas canvas((void*)ui->frame_local_preview_1->winId());
    engine->startPreview(&canvas, ZEGO_PUBLISH_CHANNEL_MAIN);

    std::string streamID = ui->lineEdit_streamID_1->text().toStdString();
    engine->startPublishing(streamID, ZEGO_PUBLISH_CHANNEL_MAIN);
}

void ZegoAuxStreamDemo::on_pushButton_stopPublish_1_clicked()
{
    engine->stopPreview(ZEGO_PUBLISH_CHANNEL_MAIN);
    engine->stopPublishing(ZEGO_PUBLISH_CHANNEL_MAIN);
}

void ZegoAuxStreamDemo::on_pushButton_startPublish_2_clicked()
{
    ZegoCanvas canvas((void*)ui->frame_local_preview_2->winId());
    engine->startPreview(&canvas, ZEGO_PUBLISH_CHANNEL_AUX);

    std::string streamID = ui->lineEdit_streamID_2->text().toStdString();
    engine->startPublishing(streamID, ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::on_pushButton_stopPublish_2_clicked()
{
    engine->stopPreview(ZEGO_PUBLISH_CHANNEL_AUX);
    engine->stopPublishing(ZEGO_PUBLISH_CHANNEL_AUX);
}

void ZegoAuxStreamDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoAuxStreamDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
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
