#include "ZegoQuickStartDemo.h"
#include "ui_ZegoQuickStartDemo.h"


#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

#include <QScrollBar>

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
        ZegoExpressSDK::destroyEngine(engine);
    }
    delete ui;
}


void ZegoQuickStartDemo::on_pushButton_createEngine_clicked()
{    
    if(engine == nullptr){
        ZegoEngineConfig engineConfig;
        ZegoExpressSDK::setEngineConfig(engineConfig);

        auto appID = ui->label_appID->text().toUInt();
        auto appSign = ui->label_appSign->text().toStdString();
        auto isTestEnv = ui->radioButton_isTestEnv->isChecked();

        engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
        bindEventHandler();

        QString log = QString("do createEngine: testEnv=%1").arg(isTestEnv);
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

        engine->loginRoom(roomID, user);
        QString log = QString("do loginRoom");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_PublishStream_clicked()
{
    if(engine != nullptr){
        std::string streamID = ui->lineEdit_publish_streamID->text().toStdString();

        engine->startPublishingStream(streamID);

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

        ZegoCanvas canvas((void*)ui->frame_remote_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
        engine->startPlayingStream(streamID, &canvas);

        QString log = QString("do  play stream");
        printLogToView(log);
    }
}

void ZegoQuickStartDemo::on_pushButton_destroyEngine_clicked()
{
    if(engine){
        QString log = QString("do destroy engine");
        printLogToView(log);
        ZegoExpressSDK::destroyEngine(engine);
    }
}

void ZegoQuickStartDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoQuickStartDemo::bindEventHandler()
{
    if(engine != nullptr){
        auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
        engine->setEventHandler(eventHandler);
    }
}
