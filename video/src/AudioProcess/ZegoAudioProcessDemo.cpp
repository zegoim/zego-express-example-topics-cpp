#include "ZegoAudioProcessDemo.h"
#include "ui_ZegoAudioProcessDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoAudioProcessDemo::ZegoAudioProcessDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoAudioProcessDemo)
{
    ui->setupUi(this);
    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "AudioProcesRoom-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoCustomAudioProcessConfig config;
    config.channel = ZEGO_AUDIO_CHANNEL_MONO;
    config.samples = 2048;
    config.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_44K;
    engine->enableCustomAudioRemoteProcessing(true, &config);
    engine->enableCustomAudioCaptureProcessing(true, &config);
}

ZegoAudioProcessDemo::~ZegoAudioProcessDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->enableCustomAudioRemoteProcessing(false, nullptr);
    engine->enableCustomAudioCaptureProcessing(false, nullptr);
    engine->setCustomAudioProcessHandler(nullptr);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoAudioProcessDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoAudioProcessDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoAudioProcessDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoAudioProcessDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}
void ZegoAudioProcessDemo::on_horizontalSlider_remoteData_valueChanged(int value)
{
    capturedAudioDataRatio = value / 100.0;
}

void ZegoAudioProcessDemo::on_horizontalSlider_capturedData_valueChanged(int value)
{
    remoteAudioDataRatio = value / 100.0;
}

void ZegoAudioProcessDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoAudioProcessDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoAudioProcessDemo::printLogToView);
    engine->setEventHandler(eventHandler);

    engine->setCustomAudioProcessHandler(std::make_shared<MyAudioProcessHandler>(this));
}

void ZegoAudioProcessDemo::onProcessCapturedAudioData(unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam *param)
{
    Q_UNUSED(param);
    for(unsigned int i=0; i<dataLength; i++){
        data[i] = data[i] * capturedAudioDataRatio;
    }
}

void ZegoAudioProcessDemo::onProcessRemoteAudioData(unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam *param, const std::string &streamID)
{
    Q_UNUSED(param);
    Q_UNUSED(streamID);
    for(unsigned int i=0; i<dataLength; i++){
        data[i] = data[i] * remoteAudioDataRatio;
    }
}
