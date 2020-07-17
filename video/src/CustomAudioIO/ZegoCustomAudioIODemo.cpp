#include "ZegoCustomAudioIODemo.h"
#include "ui_ZegoCustomAudioIODemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

#include <QFileDialog>

class CustomAudioInputDevice: public QIODevice{
    qint64 readData(char *data, qint64 maxlen) override {
        Q_UNUSED(data);
        Q_UNUSED(maxlen);
        return  0;
    }

    qint64 writeData(const char *data, qint64 len) override{
        ZegoAudioFrameParam audioFrameParam;
        audioFrameParam.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_44K;
        audioFrameParam.channel = ZEGO_AUDIO_CHANNEL_MONO;
        ZegoExpressSDK::getEngine()->sendCustomAudioCapturePCMData((unsigned char *)data, len, audioFrameParam);
        return len;
    }
};

class CustomAudioOutputDevice: public QIODevice{
    qint64 readData(char *data, qint64 maxlen) override {
        ZegoAudioFrameParam renderAudioFrameParam;
        renderAudioFrameParam.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_44K;
        renderAudioFrameParam.channel = ZEGO_AUDIO_CHANNEL_MONO;
        ZegoExpressSDK::getEngine()->fetchCustomAudioRenderPCMData((unsigned char *)data, maxlen, renderAudioFrameParam);
        return maxlen;
    }

    qint64 writeData(const char *data, qint64 len) override{
        Q_UNUSED(data);
        Q_UNUSED(len);
        return 0;
    }
};

ZegoCustomAudioIODemo::ZegoCustomAudioIODemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCustomAudioIODemo)
{
    ui->setupUi(this);

    currentRoomID = "CustomAudioIO-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();
}

ZegoCustomAudioIODemo::~ZegoCustomAudioIODemo()
{
    stopCustomAudioCapture();
    stopCustomAudioRender();

    engine->logoutRoom(currentRoomID);
    engine->enableCustomAudioIO(false, nullptr);
    engine->setEventHandler(nullptr);
    engine->setAudioDataHandler(nullptr);

    delete ui;
}

void ZegoCustomAudioIODemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoCustomAudioIODemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoCustomAudioIODemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoCustomAudioIODemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCustomAudioIODemo::on_pushButton_enableCustomAudioIO_clicked()
{
    // call enableCustomAudioIO outside room
    engine->logoutRoom(currentRoomID);

    ZegoCustomAudioConfig audioConfig;
    audioConfig.sourceType = ZEGO_AUDIO_SOURCE_TYPE_CUSTOM;
    engine->enableCustomAudioIO(true, &audioConfig);

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    {
        // Restart Custom capture Thread
        stopCustomAudioCapture();
        startCustomAudioCapture();

        // Restart Custom Render Thread
        stopCustomAudioRender();
        startCustomAudioRender();
    }
}

void ZegoCustomAudioIODemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoCustomAudioIODemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoCustomAudioIODemo::printLogToView);
    engine->setEventHandler(eventHandler);

    engine->setAudioDataHandler(std::make_shared<MyAudioDataHandler>(this));
}

void ZegoCustomAudioIODemo::startCustomAudioCapture()
{
    QAudioFormat nFormat;
    nFormat.setSampleRate(44100);
    nFormat.setSampleSize(16);
    nFormat.setChannelCount(1);
    nFormat.setCodec("audio/pcm");
    nFormat.setByteOrder(QAudioFormat::LittleEndian);
    nFormat.setSampleType(QAudioFormat::UnSignedInt);

    mCustomAudioInput = new QAudioInput(nFormat);
    mCustomAudioInputDevice = new CustomAudioInputDevice();
    mCustomAudioInputDevice->open(QIODevice::WriteOnly);
    mCustomAudioInput->start(mCustomAudioInputDevice);
}

void ZegoCustomAudioIODemo::stopCustomAudioCapture()
{
    if(mCustomAudioInputDevice){
        mCustomAudioInputDevice->close();
        delete  mCustomAudioInputDevice;
        mCustomAudioInputDevice = nullptr;
    }

    if(mCustomAudioInput){
        mCustomAudioInput->stop();
        delete mCustomAudioInput;
        mCustomAudioInput = nullptr;
    }
}

void ZegoCustomAudioIODemo::startCustomAudioRender()
{
    QAudioFormat nFormat;
    nFormat.setSampleRate(44100);
    nFormat.setSampleSize(16);
    nFormat.setChannelCount(1);
    nFormat.setCodec("audio/pcm");
    nFormat.setByteOrder(QAudioFormat::LittleEndian);
    nFormat.setSampleType(QAudioFormat::UnSignedInt);

    mCustomAudioOutput = new QAudioOutput(nFormat);
    mCustomAudioOutputDevice = new CustomAudioOutputDevice();
    mCustomAudioOutputDevice->open(QIODevice::ReadOnly);
    mCustomAudioOutput->start(mCustomAudioOutputDevice);
}

void ZegoCustomAudioIODemo::stopCustomAudioRender()
{
    if(mCustomAudioOutputDevice){
        mCustomAudioOutputDevice->close();
        delete  mCustomAudioOutputDevice;
        mCustomAudioOutputDevice = nullptr;
    }

    if(mCustomAudioOutput){
        mCustomAudioOutput->stop();
        delete mCustomAudioOutput;
        mCustomAudioOutput = nullptr;
    }
}

void ZegoCustomAudioIODemo::on_pushButton_enableAudioDataCallback_clicked()
{
    ZegoAudioFrameParam param;
    param.channel = ZEGO_AUDIO_CHANNEL_STEREO;
    param.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_8K;
    unsigned int bitmask = 0;
    if (ui->checkBox_capturedAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_CAPTURED;
    }
    if (ui->checkBox_remoteAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_REMOTE;
    }
    if (ui->checkBox_mixedAudioData->isChecked()) {
        bitmask |= ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_MIXED;
    }
    engine->enableAudioDataCallback(true, bitmask, param);
}

void ZegoCustomAudioIODemo::onCapturedAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    Q_UNUSED(dataLength)
    Q_UNUSED(param)
}

void ZegoCustomAudioIODemo::onRemoteAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    Q_UNUSED(dataLength)
    Q_UNUSED(param)
}

void ZegoCustomAudioIODemo::onMixedAudioData(const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(data)
    Q_UNUSED(dataLength)
    Q_UNUSED(param)
}












