#include "ZegoCustomVideoCaptureDemo.h"
#include "ui_ZegoCustomVideoCaptureDemo.h"
#include <QFileDialog>
#include <QImage>
#include <QTimer>
#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

CustomVideoCapturer::CustomVideoCapturer(ZegoVideoBufferType bufferType, ZegoPublishChannel channel)
    : mVideoBufferType(bufferType), mPublishChannel(channel)
{

}

CustomVideoCapturer::~CustomVideoCapturer()
{
    onStop(mPublishChannel);
}

void CustomVideoCapturer::onStart(ZegoPublishChannel channel)
{
    if(mPublishChannel != channel)
    {
        return;
    }

    if (!mVideoCaptureRunning)
    {
        mVideoCaptureRunning = true;
        mVideoCaptureThread = std::thread(std::bind(&CustomVideoCapturer::collectVideoFrameAndSendToEngine, this));
    }
}

void CustomVideoCapturer::onStop(ZegoPublishChannel channel)
{ 
    if(mPublishChannel != channel)
    {
        return;
    }

    if (mVideoCaptureRunning)
    {
        mVideoCaptureRunning = false;
        mVideoCaptureThread.join();
    }
}

void CustomVideoCapturer::collectVideoFrameAndSendToEngine()
{
    while (true)
    {
        if(!mVideoCaptureRunning){
            break;
        }

        std::shared_ptr<ZegoCustomVideoFrame> videoFrame;
        this->getVideoFrame(videoFrame);

        if (videoFrame)
        {
            ZegoExpressSDK::getEngine()->sendCustomVideoCaptureRawData(videoFrame->data.get(), videoFrame->dataLength, videoFrame->param, videoFrame->referenceTimeMillsecond, mPublishChannel);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

ZegoCustomVideoCaptureDemo::ZegoCustomVideoCaptureDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCustomVideoCaptureDemo)
{
    ui->setupUi(this);

    currentRoomID = "CustomVideoCapture-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ui->pushButton_cameraSource->hide();
    ui->pushButton_screenSource->hide();

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    ui->comboBox_customVideoCaptureType->blockSignals(true);
    QStringList mirroModeList = {
        "UNKNOWN",
        "RAW_DATA",
        //"ENCODED_DATA",
    };
    ui->comboBox_customVideoCaptureType->addItems(mirroModeList);
    ui->comboBox_customVideoCaptureType->setCurrentIndex(1);
    ui->comboBox_customVideoCaptureType->blockSignals(false);
}

ZegoCustomVideoCaptureDemo::~ZegoCustomVideoCaptureDemo()
{
    // Manual stop custom video capture and logoutroom
    // make sure to call enableCustomVideoCapture outside room
    {
        engine->setCustomVideoCaptureHandler(nullptr);
        mCustomVideoCapture = nullptr;
        engine->logoutRoom(currentRoomID);
    }
    engine->enableCustomVideoCapture(false, nullptr);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoCustomVideoCaptureDemo::on_pushButton_enableCustomVideoCapture_clicked()
{
    // Manual stop custom video canpture and logoutroom
    // make sure to call enableCustomVideoCapture outside room
    {
        engine->setCustomVideoCaptureHandler(nullptr);
        mCustomVideoCapture = nullptr;
        engine->logoutRoom(currentRoomID);
    }

    ZegoVideoBufferType bufferTypeSelected = ZegoVideoBufferType(ui->comboBox_customVideoCaptureType->currentIndex());
    ZegoCustomVideoCaptureConfig customVideoCaptureConfig;
    customVideoCaptureConfig.bufferType = bufferTypeSelected;
    engine->enableCustomVideoCapture(true, &customVideoCaptureConfig);

    mCustomVideoCapture = std::make_shared<CustomVideoCapturer>(bufferTypeSelected);
    engine->setCustomVideoCaptureHandler(mCustomVideoCapture);

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()));
    engine->startPreview(&canvas);

    // set video config (reslution) according to the video data sending to SDK
    engine->setVideoConfig(ZegoVideoConfig(ZEGO_VIDEO_CONFIG_PRESET_540P));
    engine->startPublishingStream(streamID);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoCustomVideoCaptureDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_imageSource_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select image file", ".", "pic (*.png *jpg)");
    if(path.isEmpty()){
        return;
    }

    auto currentVideoSource = mCustomVideoCapture->getVideoSource(ZegoCustomVideoSourceType_Image);
    auto theImageSource =static_cast<ZegoCustomVideoSourceImage*>(currentVideoSource);
    theImageSource->setImagePath(path.toStdString());
}

void ZegoCustomVideoCaptureDemo::on_pushButton_mediaSource_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "Video (*.mp4)");
    if(path.isEmpty()){
        return;
    }

    auto currentVideoSource = mCustomVideoCapture->getVideoSource(ZegoCustomVideoSourceType_Media);
    auto theMediaSource = static_cast<ZegoCustomVideoSourceMedia*>(currentVideoSource);
    theMediaSource->stopPlayMedia();
    theMediaSource->startPlayMedia(path.toStdString());
}

void ZegoCustomVideoCaptureDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoCustomVideoCaptureDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoCustomVideoCaptureDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}
