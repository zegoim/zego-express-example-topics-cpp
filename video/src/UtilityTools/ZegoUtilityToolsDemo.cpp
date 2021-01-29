#include "ZegoUtilityToolsDemo.h"
#include "ui_ZegoUtilityToolsDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoUtilityToolsDemo::ZegoUtilityToolsDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoUtilityToolsDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "PerformanceMonitorRoom-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));


    ui->comboBox_expectedBitrate->blockSignals(true);
    QStringList ZegoResolutionList = {
        "180P" ,
        "270P" ,
        "360P" ,
        "540P" ,
        "720P",
        "1080P"
    };
    ui->comboBox_expectedBitrate->addItems(ZegoResolutionList);
    ui->comboBox_expectedBitrate->blockSignals(false);
}

ZegoUtilityToolsDemo::~ZegoUtilityToolsDemo()
{
    engine->stopPerformanceMonitor();
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoUtilityToolsDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoUtilityToolsDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoUtilityToolsDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoUtilityToolsDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoUtilityToolsDemo::on_pushButton_startPerformaceMonitor_clicked()
{
    engine->startPerformanceMonitor();
    printLogToView("startPerformaceMonitor clicked");
}

void ZegoUtilityToolsDemo::on_pushButton_stopPerformaceMonitor_clicked()
{
    printLogToView("stopPerformaceMonitor clicked");

    engine->stopPerformanceMonitor();
    ui->lineEdit_cpuUsageApp->clear();
    ui->lineEdit_cpuUsageSystem->clear();
    ui->lineEdit_memoryUsageApp->clear();
    ui->lineEdit_memoryUsageSystem->clear();
    ui->lineEdit_memoryUsedApp->clear();
}

void ZegoUtilityToolsDemo::onPerformanceStatusUpdate(const ZegoPerformanceStatus &status)
{
    /** Current CPU usage of the app, value range [0, 1] */
    ui->lineEdit_cpuUsageApp->setText(QString::number(status.cpuUsageApp));

    /** Current CPU usage of the system, value range [0, 1] */
    ui->lineEdit_cpuUsageSystem->setText(QString::number(status.cpuUsageSystem));

    /** Current memory usage of the app, value range [0, 1] */
    ui->lineEdit_memoryUsageApp->setText(QString::number(status.memoryUsageApp));

    /** Current memory usage of the system, value range [0, 1] */
    ui->lineEdit_memoryUsageSystem->setText(QString::number(status.memoryUsageSystem));

    /** Current memory used of the app, in MB */
    ui->lineEdit_memoryUsedApp->setText(QString::number(status.memoryUsedApp));
}

void ZegoUtilityToolsDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoUtilityToolsDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoUtilityToolsDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPerformanceStatusUpdate, this, &ZegoUtilityToolsDemo::onPerformanceStatusUpdate);


    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigNetworkSpeedTestError, this, &ZegoUtilityToolsDemo::onNetworkSpeedTestError);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigNetworkSpeedTestQualityUpdate, this, &ZegoUtilityToolsDemo::onNetworkSpeedTestQualityUpdate);

    engine->setEventHandler(eventHandler);
}


void ZegoUtilityToolsDemo::on_pushButton_startNetworkSpeedTest_clicked()
{
    ZegoVideoConfig videoConfig(ZegoVideoConfigPreset(ui->comboBox_expectedBitrate->currentIndex()));

    ZegoNetworkSpeedTestConfig config;
    config.testUplink = true;
    config.expectedUplinkBitrate = videoConfig.bitrate;
    config.testDownlink = true;
    config.expectedDownlinkBitrate = videoConfig.bitrate;
    engine->startNetworkSpeedTest(config);

    printLogToView("startNetworkSpeedTest clicked");
}

void ZegoUtilityToolsDemo::on_pushButton_stopNetworkSpeedTest_clicked()
{
    engine->stopNetworkSpeedTest();
    printLogToView("stopNetworkSpeedTest clicked");

    ui->lineEdit_rtt_uplink->clear();
    ui->lineEdit_packageListRate_uplink->clear();
    ui->lineEdit_connectCostUpdate_uplink->clear();

    ui->lineEdit_rtt_downlink->clear();
    ui->lineEdit_packageListRate_downlink->clear();
    ui->lineEdit_connectCostUpdate_downlink->clear();
}

void ZegoUtilityToolsDemo::onNetworkSpeedTestError(int errorCode, ZegoNetworkSpeedTestType type)
{
    printLogToView(QString("onNetworkSpeedTestError errorCode=%1 type=%2").arg(errorCode).arg(type));
}

void ZegoUtilityToolsDemo::onNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality& quality, ZegoNetworkSpeedTestType type)
{
    if(type == ZEGO_NETWORK_SPEED_TEST_TYPE_UPLINK){
        ui->lineEdit_rtt_uplink->setText(QString::number(quality.rtt));
        ui->lineEdit_packageListRate_uplink->setText(QString::number(quality.packetLostRate));
        ui->lineEdit_connectCostUpdate_uplink->setText(QString::number(quality.connectCost));
    }
    else{
        ui->lineEdit_rtt_downlink->setText(QString::number(quality.rtt));
        ui->lineEdit_packageListRate_downlink->setText(QString::number(quality.packetLostRate));
        ui->lineEdit_connectCostUpdate_downlink->setText(QString::number(quality.connectCost));
    }

    printLogToView(QString("onNetworkSpeedTestQualityUpdate packetLostRate=%1 type=%2").arg(quality.packetLostRate).arg(type));
}
