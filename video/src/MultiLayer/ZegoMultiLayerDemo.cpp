#include "ZegoMultiLayerDemo.h"
#include "ui_ZegoMultiLayerDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoMultiLayerDemo::ZegoMultiLayerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMultiLayerDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "MultiLayer-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    QStringList layerList = {
        "VIDEO_LAYER_AUTO",
        "VIDEO_LAYER_BASE",
        "VIDEO_LAYER_BASE_EXTEND"
    };
    ui->comboBox_play_layer->addItems(layerList);
}

ZegoMultiLayerDemo::~ZegoMultiLayerDemo()
{
    ZegoVideoConfig config;
    engine->setVideoConfig(config);
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoMultiLayerDemo::on_pushButton_start_publish_clicked()
{
    // set codecID as SVC to enable multi-layer before start publish
    ZegoVideoConfig config(ZEGO_VIDEO_CONFIG_PRESET_1080P);
    config.codecID = ZEGO_VIDEO_CODEC_ID_SVC;
    engine->setVideoConfig(config);

    std::string streamID = ui->lineEdit_streamID_to_publish->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas((void*)ui->frame_local_preview->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPreview(&canvas);
}

void ZegoMultiLayerDemo::on_pushButton_stop_publish_clicked()
{
    engine->stopPublishingStream();
    engine->stopPreview();
}

void ZegoMultiLayerDemo::on_pushButton_start_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    ZegoCanvas canvas((void*)ui->frame_remote_play->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);

    // choose the layer for player
    ZegoPlayerConfig config;
    config.videoLayer =  ZegoPlayerVideoLayer(ui->comboBox_play_layer->currentIndex());
    engine->startPlayingStream(streamID, &canvas, config);
}

void ZegoMultiLayerDemo::on_pushButton_stop_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoMultiLayerDemo::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    Q_UNUSED(streamID)
    ui->lineEdit_videoBPS->setText(QString::number(quality.videoKBPS) + "kbps");
    ui->lineEdit_videoFPS->setText(QString::number(quality.videoRecvFPS) + "fps");
}

void ZegoMultiLayerDemo::onPlayerVideoSizeChanged(const std::string& streamID, int width, int height)
{
    Q_UNUSED(streamID)
    ui->lineEdit_resolution->setText(QString("%1x%2").arg(width).arg(height));
}

void ZegoMultiLayerDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoMultiLayerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoMultiLayerDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPlayerVideoSizeChanged, this, &ZegoMultiLayerDemo::onPlayerVideoSizeChanged);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPlayerQualityUpdate, this, &ZegoMultiLayerDemo::onPlayerQualityUpdate);
    engine->setEventHandler(eventHandler);
}
