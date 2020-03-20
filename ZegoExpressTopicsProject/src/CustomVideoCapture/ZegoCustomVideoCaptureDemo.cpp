#include "ZegoCustomVideoCaptureDemo.h"
#include "ui_ZegoCustomVideoCaptureDemo.h"
#include <QFileDialog>
#include <QImage>
#include <QTimer>
#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

void CustomVideoCapturer::onStart(ZegoPublishChannel channel)
{
    Q_UNUSED(channel)
    if (!mVideoCaptureRunning)
    {
        mVideoCaptureRunning = true;
        mVideoCaptureThread = std::thread(std::bind(&CustomVideoCapturer::collectVideoFrameAndSendToEngine, this));
    }
}

void CustomVideoCapturer::onStop(ZegoPublishChannel channel)
{ 
    Q_UNUSED(channel)
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
            ZegoExpressSDK::getEngine()->sendCustomVideoCaptureRawData(videoFrame->data.get(), videoFrame->dataLength, videoFrame->param, videoFrame->referenceFimeMillsecond);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

ZegoCustomVideoCaptureDemo::ZegoCustomVideoCaptureDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCustomVideoCaptureDemo)
{
    ui->setupUi(this);

    roomID = "CustomVideoRender-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ui->pushButton_cameraSource->hide();
    ui->pushButton_screenSource->hide();
}

ZegoCustomVideoCaptureDemo::~ZegoCustomVideoCaptureDemo()
{
    if(mCustomVideoCapture){
        mCustomVideoCapture->onStop(ZEGO_PUBLISH_CHANNEL_MAIN);
    }
    if(engine){
        engine->setCustomVideoCaptureHandler(nullptr);
        ZegoExpressSDK::destroyEngine(engine);
    }
    delete ui;
}


void ZegoCustomVideoCaptureDemo::createCustomCaptureEngine(){
    ZegoCustomVideoCaptureConfig customVideoCaptureConfig;
    customVideoCaptureConfig.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;

    ZegoEngineConfig engineConfig;
    engineConfig.customVideoCaptureMainConfig = &customVideoCaptureConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();
    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);

    mCustomVideoCapture = std::make_shared<CustomVideoCapturer>();
    engine->setCustomVideoCaptureHandler(mCustomVideoCapture);

    bindEventHandler();

    ZegoUser user(userID, userID);
    engine->loginRoom(roomID, user);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_setEngineConfig_clicked()
{
    if(engine){
        if(mCustomVideoCapture){
            mCustomVideoCapture->onStop(ZEGO_PUBLISH_CHANNEL_MAIN);
        }
        ZegoExpressSDK::destroyEngine(engine, [=](){
            createCustomCaptureEngine();
        });
    } else {
        createCustomCaptureEngine();
    }
}

void ZegoCustomVideoCaptureDemo::on_pushButton_startPublish_clicked()
{
    if(engine == nullptr){
        return;
    }

    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_local_video->winId()));
    engine->startPreview(&canvas);
    engine->startPublishingStream(streamID);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_stopPublish_clicked()
{
    if(engine == nullptr){
        return;
    }
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoCustomVideoCaptureDemo::on_pushButton_startPlay_clicked()
{
    if(engine == nullptr){
        return;
    }
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_stopPlay_clicked()
{
    if(engine == nullptr){
        return;
    }
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCustomVideoCaptureDemo::on_pushButton_imageSource_clicked()
{
    if(engine == nullptr){
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select image file", ".", "pic (*.png *jpg)");
    if(path.isEmpty()){
        return;
    }

    auto currentVideoSource = mCustomVideoCapture->getVideoSource(ZegoCustomVideoSourceType_Image);
    auto theImageSource = (ZegoCustomVideoSourceImage*)currentVideoSource;
    theImageSource->setImagePath(path.toStdString());
}

void ZegoCustomVideoCaptureDemo::on_pushButton_videoSource_clicked()
{
    if(engine == nullptr){
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "Video (*.mp4)");
    if(path.isEmpty()){
        return;
    }

    auto currentVideoSource = mCustomVideoCapture->getVideoSource(ZegoCustomVideoSourceType_Media);
    auto theMediaSource = (ZegoCustomVideoSourceMedia*)currentVideoSource;
    theMediaSource->stopPlayMedia();
    theMediaSource->startPlayMedia(path.toStdString());
}

void ZegoCustomVideoCaptureDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    engine->setEventHandler(eventHandler);
}
