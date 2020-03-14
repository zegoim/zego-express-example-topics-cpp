#include "ZegoCDNAboutDemo.h"
#include "ui_ZegoCDNAboutDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoCDNAboutDemo::ZegoCDNAboutDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCDNAboutDemo)
{
    ui->setupUi(this);

    ZegoEngineConfig engineConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);
    
    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    roomID = "CDNAboutRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(roomID, user);

    ZegoCanvas canvas((void*)ui->frame_local_video->winId());
    engine->startPreview(&canvas);
}

ZegoCDNAboutDemo::~ZegoCDNAboutDemo()
{
    ZegoExpressSDK::destroyEngine(engine);
    delete ui;
}

void ZegoCDNAboutDemo::on_pushButton_startPublishToZegoServer_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    engine->startPublishing(streamID);
}

void ZegoCDNAboutDemo::on_pushButton_addPublishToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();
    engine->addPublishCDNURL(streamID, cdnUrl, [=](int errorCode){
        printLogToView(QString("addPublishCDNURL: streamID=%1, cdnUrl=%2, erroCode=%3").arg(streamID.c_str()).arg(cdnUrl.c_str()).arg(errorCode));
    });
}

void ZegoCDNAboutDemo::on_pushButton_removePublishToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();
    engine->removePublishCDNURL(streamID, cdnUrl, [=](int errorCode){
        printLogToView(QString("removePublishCDNURL: streamID=%1, cdnUrl=%2, erroCode=%3").arg(streamID.c_str()).arg(cdnUrl.c_str()).arg(errorCode));
    });
}

void ZegoCDNAboutDemo::on_pushButton_stopPublishToZegoServer_clicked()
{
    engine->stopPublishing();
}

void ZegoCDNAboutDemo::on_pushButton_startPublishDirectToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();

    ZegoCDNConfig cdnConfig;
    cdnConfig.URL = cdnUrl;
    engine->enablePublishDirectToCDN(true, &cdnConfig);
    engine->startPublishing(streamID);
}

void ZegoCDNAboutDemo::on_pushButton_stopPublishDirectToCDN_clicked()
{
    engine->stopPublishing();
    engine->enablePublishDirectToCDN(false, nullptr);
}


void ZegoCDNAboutDemo::on_pushButton_startPlayFromCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_play->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_play->text().toStdString();
    ZegoCanvas canvas((void*)ui->frame_remote_video->winId());

    ZegoCDNConfig cdnConfig;
    cdnConfig.URL = cdnUrl;
    
    ZegoPlayerConfig config;
    config.CDNConfig = &cdnConfig;

    engine->startPlayingStream(streamID, &canvas, config);
}

void ZegoCDNAboutDemo::on_pushButton_stopPlayFromCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_play->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCDNAboutDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoCDNAboutDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    engine->setEventHandler(eventHandler);
}
