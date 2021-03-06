#include "ZegoCDNAboutDemo.h"
#include "ui_ZegoCDNAboutDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoCDNAboutDemo::ZegoCDNAboutDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCDNAboutDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "CDNAboutRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()));
    engine->startPreview(&canvas);
}

ZegoCDNAboutDemo::~ZegoCDNAboutDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoCDNAboutDemo::on_pushButton_startPublishToZegoServer_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    engine->startPublishingStream(streamID);
}

void ZegoCDNAboutDemo::on_pushButton_addPublishToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();
    engine->addPublishCdnUrl(streamID, cdnUrl, [=](int errorCode){
        printLogToView(QString("addPublishCdnUrl: streamID=%1, cdnUrl=%2, erroCode=%3").arg(streamID.c_str()).arg(cdnUrl.c_str()).arg(errorCode));
    });
}

void ZegoCDNAboutDemo::on_pushButton_removePublishToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();
    engine->removePublishCdnUrl(streamID, cdnUrl, [=](int errorCode){
        printLogToView(QString("removePublishCdnUrl: streamID=%1, cdnUrl=%2, erroCode=%3").arg(streamID.c_str()).arg(cdnUrl.c_str()).arg(errorCode));
    });
}

void ZegoCDNAboutDemo::on_pushButton_stopPublishToZegoServer_clicked()
{
    engine->stopPublishingStream();
}

void ZegoCDNAboutDemo::on_pushButton_startPublishDirectToCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_publish->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_publish->text().toStdString();

    ZegoCDNConfig cdnConfig;
    cdnConfig.url = cdnUrl;
    engine->enablePublishDirectToCDN(true, &cdnConfig);
    engine->startPublishingStream(streamID);
}

void ZegoCDNAboutDemo::on_pushButton_stopPublishDirectToCDN_clicked()
{
    engine->stopPublishingStream();
    engine->enablePublishDirectToCDN(false, nullptr);
}

void ZegoCDNAboutDemo::on_pushButton_startPlayFromCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_play->text().toStdString();
    std::string cdnUrl = ui->lineEdit_CDNUrl_play->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()));

    ZegoCDNConfig cdnConfig;
    cdnConfig.url = cdnUrl;
    
    ZegoPlayerConfig config;
    config.cdnConfig = &cdnConfig;

    engine->startPlayingStream(streamID, &canvas, config);
}

void ZegoCDNAboutDemo::on_pushButton_stopPlayFromCDN_clicked()
{
    std::string streamID = ui->lineEdit_streamID_play->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCDNAboutDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoCDNAboutDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoCDNAboutDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}



