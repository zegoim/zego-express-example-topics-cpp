#include "ZegoAudioDataCallbackDemo.h"
#include "ui_ZegoAudioDataCallbackDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoAudioDataCallbackDemo::ZegoAudioDataCallbackDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoAudioDataCallbackDemo)
{
    ui->setupUi(this);

    currentRoomID = "AudioDataCallback-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
}

ZegoAudioDataCallbackDemo::~ZegoAudioDataCallbackDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    engine->setAudioDataHandler(nullptr);

    delete ui;
}

void ZegoAudioDataCallbackDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoAudioDataCallbackDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoAudioDataCallbackDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoAudioDataCallbackDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}


void ZegoAudioDataCallbackDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoAudioDataCallbackDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoAudioDataCallbackDemo::printLogToView);
    engine->setEventHandler(eventHandler);

    engine->setAudioDataHandler(std::make_shared<MyAudioDataHandler>(this));
}


void ZegoAudioDataCallbackDemo::on_pushButton_enableAudioDataCallback_clicked()
{
    ZegoAudioFrameParam param;
    param.channel = ZEGO_AUDIO_CHANNEL_STEREO;
    param.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_8K;
    unsigned int bitmask = 0;
    if (ui->checkBox_capturedAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_CAPTURED;
    }
    if (ui->checkBox_playbackAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_PLAYBACK;
    }
    if (ui->checkBox_mixedAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_MIXED;
    }
    engine->enableAudioDataCallback(true, bitmask, param);
}

void ZegoAudioDataCallbackDemo::onCapturedAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    printLogToView(QString("onCapturedAudioData: dataLength=%1, sampleRate=%2, channel=%3").arg(dataLength).arg(param.sampleRate).arg(param.channel));
}

void ZegoAudioDataCallbackDemo::onPlaybackAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    printLogToView(QString("onPlaybackAudioData: dataLength=%1, sampleRate=%2, channel=%3").arg(dataLength).arg(param.sampleRate).arg(param.channel));
}

void ZegoAudioDataCallbackDemo::onMixedAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    printLogToView(QString("onMixedAudioData: dataLength=%1, sampleRate=%2, channel=%3").arg(dataLength).arg(param.sampleRate).arg(param.channel));
}
