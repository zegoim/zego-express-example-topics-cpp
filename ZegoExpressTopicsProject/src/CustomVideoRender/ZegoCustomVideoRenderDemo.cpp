#include "ZegoCustomVideoRenderDemo.h"
#include "ui_ZegoCustomVideoRenderDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoCustomVideoRenderDemo::ZegoCustomVideoRenderDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoCustomVideoRenderDemo)
{
    ui->setupUi(this);

    roomID = "CustomVideoRender-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
}

ZegoCustomVideoRenderDemo::~ZegoCustomVideoRenderDemo()
{
    if(engine){
        ZegoExpressSDK::destroyEngine(engine);
    }
    delete ui;
}

void ZegoCustomVideoRenderDemo::on_pushButton_setEngineConfig_clicked()
{
    ZegoExpressSDK::destroyEngine(engine);

    ZegoCustomVideoRenderConfig customVideoRenderConfig;
    customVideoRenderConfig.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
    customVideoRenderConfig.frameFormatSeries = ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
    customVideoRenderConfig.enableEngineRender = ui->checkBox_enableEngineRender->isChecked();

    ZegoEngineConfig engineConfig;
    engineConfig.customVideoRenderConfig = &customVideoRenderConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();
    engine = ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    engine->setCustomVideoRenderHandler(std::make_shared<CustomVideoRenderer>(this));
    ZegoUser user(userID, userID);
    engine->loginRoom(roomID, user);
}

void ZegoCustomVideoRenderDemo::on_pushButton_startPublish_clicked()
{
    if(engine == nullptr){
        return;
    }

    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_internal_video->winId()));
    engine->startPreview(&canvas);

}

void ZegoCustomVideoRenderDemo::on_pushButton_stopPublish_clicked()
{
    if(engine == nullptr){
        return;
    }
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoCustomVideoRenderDemo::on_pushButton_startPlay_clicked()
{
    if(engine == nullptr){
        return;
    }
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_internal_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoCustomVideoRenderDemo::on_pushButton_stopPlay_clicked()
{
    if(engine == nullptr){
        return;
    }
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCustomVideoRenderDemo::onCapturedVideoFrameRawData(unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode)
{
    Q_UNUSED(flipMode)
    Q_UNUSED(dataLength)

    if(param.format != ZEGO_VIDEO_FRAME_FORMAT_BGRA32){
        return;
    }

    QImage image(data[0], param.width, param.height, param.strides[0], QImage::Format_ARGB32);
    if(flipMode == ZEGO_VIDEO_FLIP_MODE_X){
        image = image.mirrored(true, false);
    }
    ui->label_custom_local_video->setPixmap(QPixmap::fromImage(image));
}

void ZegoCustomVideoRenderDemo::onRemoteVideoFrameRawData(unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param, std::string streamID)
{
    Q_UNUSED(streamID)
    Q_UNUSED(dataLength)

    if(param.format != ZEGO_VIDEO_FRAME_FORMAT_BGRA32){
        return;
    }

    QImage image(data[0], param.width, param.height, param.strides[0], QImage::Format_ARGB32);
    ui->label_custom_remote_video->setPixmap(QPixmap::fromImage(image));


    bool greyInternalFilter = false;
    if(greyInternalFilter){
        unsigned char * src = data[0];
        unsigned char * dst = data[0];
        for (int i = 0; i < param.height; i++) {
            for (int j = 0; j < param.width; j++, src += 4, dst += 4) {
                unsigned char grey = (*src * 38 + *(src + 1) * 75 + *(src + 2) * 15) >> 7;
                *dst = grey;
                *(dst + 1) = grey;
                *(dst + 2) = grey;
            }
        }
    }
}

void ZegoCustomVideoRenderDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    engine->setEventHandler(eventHandler);
}

