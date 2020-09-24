#include "ZegoAudioMixingDemo.h"
#include "ui_ZegoAudioMixingDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

#include <QFileDialog>

ZegoAudioMixingDemo::ZegoAudioMixingDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoAudioMixingDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "AudioMixingRoom-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
}

ZegoAudioMixingDemo::~ZegoAudioMixingDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setAudioMixingHandler(nullptr);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoAudioMixingDemo::on_pushButton_start_publish_clicked()
{
    currentPublishStreamID = ui->lineEdit_streamID_to_publish->text().toStdString();
    engine->startPublishingStream(currentPublishStreamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoAudioMixingDemo::on_pushButton_stop_publish_clicked()
{
    engine->stopPublishingStream();
    engine->stopPreview();
}

void ZegoAudioMixingDemo::on_pushButton_loadAudioFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select audio file", ".", "audio (*.pcm)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEdit_audioFile->setText(path);

    // https://www.cnblogs.com/zhugaopeng/p/9090980.html
    std::lock_guard<std::mutex> locker(pcmBufferMutex);
#ifdef WIN32
    std::string ansiPath;
    ZegoUtilHelper::convertUtf8ToANSI(path.toStdString(), ansiPath);
    FILE* pcmFile = fopen(ansiPath.c_str(), "r");
#else
    FILE* pcmFile = fopen(path.toStdString().c_str(), "r");
#endif
    fseek(pcmFile, 0, SEEK_END);
    pcmDataLength = ftell(pcmFile);
    pcmData = std::unique_ptr<unsigned char[]>(new unsigned char[pcmDataLength]);
	rewind(pcmFile);
	fread(pcmData.get(), 1, pcmDataLength, pcmFile);
	fclose(pcmFile);

    pcmDataPos = 0;
}

void ZegoAudioMixingDemo::on_horizontalSlider_localVolume_valueChanged(int value)
{
    engine->setAudioMixingVolume(value, ZEGO_VOLUME_TYPE_LOCAL);
}

void ZegoAudioMixingDemo::on_horizontalSlider_remoteVolume_valueChanged(int value)
{
    engine->setAudioMixingVolume(value, ZEGO_VOLUME_TYPE_REMOTE);
}

void ZegoAudioMixingDemo::on_pushButton_enableAudioMixing_clicked()
{
    auto handler = std::make_shared<MyAudioMixingHandler>(this);
    engine->setAudioMixingHandler(handler);
    engine->enableAudioMixing(true);
}

void ZegoAudioMixingDemo::on_pushButton_disableAudioMixing_clicked()
{
    engine->setAudioMixingHandler(nullptr);
    engine->enableAudioMixing(false);
}

void ZegoAudioMixingDemo::on_pushButton_start_play_clicked()
{
    ZegoCanvas canvas(ZegoView(ui->frame_remote_play->winId()));
    currentPlayStreamID = ui->lineEdit_streamID_to_play->text().toStdString();
    engine->startPlayingStream(currentPlayStreamID, &canvas);
}

void ZegoAudioMixingDemo::on_pushButton_stop_play_clicked()
{
    engine->stopPlayingStream(currentPlayStreamID);
}

void ZegoAudioMixingDemo::onAudioMixingCopyData(ZegoAudioMixingData *data)
{
    std::lock_guard<std::mutex> locker(pcmBufferMutex);
    if (pcmData != nullptr)
    {
        // attention: set sampleRate and channel according to the pcm file you load
        data->param.sampleRate = ZEGO_AUDIO_SAMPLE_RATE_44K;
        data->param.channel = ZEGO_AUDIO_CHANNEL_STEREO;

        // this is enough data can meet SDK need
        if (data->audioDataLength+pcmDataPos < pcmDataLength)
        {
            memcpy(data->audioData, pcmData.get() + pcmDataPos, data->audioDataLength);
            pcmDataPos += data->audioDataLength;
        }
        // no enough data SDK needed
        else
        {
            data->audioDataLength = 0;
            pcmDataPos = 0;
        }
    }
    else
    {
        data->audioDataLength = 0;
    }
}

void ZegoAudioMixingDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoAudioMixingDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoAudioMixingDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoAudioMixingDemo::on_checkBox_muteLocalAudioMixing_clicked()
{
    engine->muteLocalAudioMixing(ui->checkBox_muteLocalAudioMixing->isChecked());
}
