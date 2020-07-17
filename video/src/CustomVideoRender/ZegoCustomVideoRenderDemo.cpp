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

    currentRoomID = "CustomVideoRender-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    QStringList supportBufferTypes = {"ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA", "ZEGO_VIDEO_BUFFER_TYPE_ENCODED_DATA"};
    ui->comboBox_bufferType->addItems(supportBufferTypes);
    ui->radioButton_rgbSeries->setChecked(true);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();
}

ZegoCustomVideoRenderDemo::~ZegoCustomVideoRenderDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->enableCustomVideoRender(false, nullptr);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoCustomVideoRenderDemo::on_pushButton_enableCustomVideoRender_clicked()
{
    // call enableCustomVideoRender outside room
    engine->logoutRoom(currentRoomID);

    ZegoCustomVideoRenderConfig customVideoRenderConfig;
    if(ui->comboBox_bufferType->currentText() == "ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA"){
        customVideoRenderConfig.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
        customVideoRenderConfig.frameFormatSeries = ui->radioButton_rgbSeries->isChecked()? ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB : ZEGO_VIDEO_FRAME_FORMAT_SERIES_YUV;
        customVideoRenderConfig.enableEngineRender = ui->checkBox_enableEngineRender->isChecked();
    }
    if(ui->comboBox_bufferType->currentText() == "ZEGO_VIDEO_BUFFER_TYPE_ENCODED_DATA"){
        customVideoRenderConfig.bufferType = ZEGO_VIDEO_BUFFER_TYPE_ENCODED_DATA;
    }

    engine->enableCustomVideoRender(true, &customVideoRenderConfig);
    engine->setCustomVideoRenderHandler(std::make_shared<CustomVideoRenderer>(this));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
}

void ZegoCustomVideoRenderDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_internal_video->winId()));
    engine->startPreview(&canvas);
}

void ZegoCustomVideoRenderDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoCustomVideoRenderDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_internal_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoCustomVideoRenderDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoCustomVideoRenderDemo::onCapturedVideoFrameRawData(unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode)
{
    Q_UNUSED(dataLength)
    if(param.format == ZEGO_VIDEO_FRAME_FORMAT_BGRA32){
        QImage image(data[0], param.width, param.height, param.strides[0], QImage::Format_ARGB32);
        if(flipMode == ZEGO_VIDEO_FLIP_MODE_X){
            image = image.mirrored(true, false);
        }
        ui->label_custom_local_video->setPixmap(QPixmap::fromImage(image));
    }
    else{
        // render captured raw_data according the the FrameFormat
    }
}

void ZegoCustomVideoRenderDemo::onRemoteVideoFrameRawData(unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param, std::string streamID)
{
    Q_UNUSED(streamID)
    Q_UNUSED(dataLength)

    if(param.format == ZEGO_VIDEO_FRAME_FORMAT_BGRA32){
        QImage image(data[0], param.width, param.height, param.strides[0], QImage::Format_ARGB32);
        ui->label_custom_remote_video->setPixmap(QPixmap::fromImage(image));
    }
    else{
        // render remote raw_data according the the FrameFormat
    }
}


void ZegoCustomVideoRenderDemo::onRemoteVideoFrameEncodedData(const unsigned char* data, unsigned int dataLength, ZegoVideoEncodedFrameParam param, unsigned long long referenceTimeMillisecond, const std::string& streamID)
{
    Q_UNUSED(data);
    Q_UNUSED(dataLength);
    Q_UNUSED(param);
    Q_UNUSED(referenceTimeMillisecond);
    Q_UNUSED(streamID);
    // render remote encoded_data according the the FrameFormat
}

void ZegoCustomVideoRenderDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoCustomVideoRenderDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoCustomVideoRenderDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoCustomVideoRenderDemo::on_comboBox_bufferType_currentTextChanged(const QString &currentText)
{
    if(currentText == "ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA"){
        ui->checkBox_enableEngineRender->setEnabled(true);
        ui->radioButton_rgbSeries->setEnabled(true);
        ui->radioButton_yuvSeries->setEnabled(true);
    }
    if(currentText == "ZEGO_VIDEO_BUFFER_TYPE_ENCODED_DATA"){
        ui->checkBox_enableEngineRender->setEnabled(false);
        ui->radioButton_rgbSeries->setEnabled(false);
        ui->radioButton_yuvSeries->setEnabled(false);
    }
}
