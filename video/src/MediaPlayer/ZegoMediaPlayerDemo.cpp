#include "ZegoMediaPlayerDemo.h"
#include "ui_ZegoMediaPlayerDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

#include <QFileDialog>

ZegoMediaPlayerDemo::ZegoMediaPlayerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMediaPlayerDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    mediaPlayer1 = engine->createMediaPlayer();
    mediaPlayer2 = engine->createMediaPlayer();
	bindEventHandler();

    currentRoomID = "MediaPlayerRoom-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    QStringList ZegoVoiceChangerPresetList = {
        "PRESET_NONE",
        "PRESET_MEN_TO_CHILD",
        "PRESET_MEN_TO_WOMEN",
        "PRESET_WOMEN_TO_CHILD",
        "PRESET_WOMEN_TO_MEN"
    };
    ui->comboBox_voiceChanger_1->addItems(ZegoVoiceChangerPresetList);
}

ZegoMediaPlayerDemo::~ZegoMediaPlayerDemo()
{
    engine->destroyMediaPlayer(mediaPlayer1);
    engine->destroyMediaPlayer(mediaPlayer2);
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoMediaPlayerDemo::on_pushButton_loadResource_1_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "Video (*.mp4);;audio (*.mp3)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEdit_resourcePath_1->setText(path);
    std::string sourcePath = ui->lineEdit_resourcePath_1->text().toStdString();
    mediaPlayer1->loadResource(sourcePath, [=](int errorCode){
        QString log = QString("loadResource: path=%1, errorCode=%2").arg(sourcePath.c_str()).arg(errorCode);
        printLogToView(log);

        mediaPlayer1->setProgressInterval(100);

        auto totalDuration = mediaPlayer1->getTotalDuration();
        ui->label_totalDuration_1->setText(QString::number(totalDuration/1000));

        auto playVolume = mediaPlayer1->getPlayVolume();
        ui->horizontalSlider_playVolume_1->setValue(playVolume);

        auto publishVolume = mediaPlayer1->getPublishVolume();
        ui->horizontalSlider_publishVolume_1->setValue(publishVolume);

        auto audioTrackCount = mediaPlayer1->getAudioTrackCount();
        ui->spinBox_audioTrackIndex_1->setMaximum(audioTrackCount-1);
        ui->spinBox_audioTrackIndex_1->setValue(0);
        printLogToView(QString("getAudioTrackCount: count=%1").arg(audioTrackCount));
    });
}

void ZegoMediaPlayerDemo::on_pushButton_startPlay_1_clicked()
{
    ZegoCanvas canvas(ZegoView(ui->frame_media_1->winId()));
    mediaPlayer1->setPlayerCanvas(&canvas);

    mediaPlayer1->enableRepeat(ui->checkBox_enableRepeat_1->isChecked());
    mediaPlayer1->start();
}

void ZegoMediaPlayerDemo::on_pushButton_pausePlay_1_clicked()
{
    mediaPlayer1->pause();
}

void ZegoMediaPlayerDemo::on_pushButton_resumePlay_1_clicked()
{
    mediaPlayer1->resume();
}

void ZegoMediaPlayerDemo::on_pushButton_stopPlay_1_clicked()
{
    mediaPlayer1->stop();
}

void ZegoMediaPlayerDemo::on_horizontalSlider_progress_1_valueChanged(int value)
{
    if(mediaPlayer1){
        auto seek = mediaPlayer1->getTotalDuration() * value / 100;
        mediaPlayer1->seekTo(seek, [=](int errorCode){
            QString log = QString("seekToTime: time(ms)=%1, errorCode=%2").arg(seek).arg(errorCode);
            printLogToView(log);
        });
    }
}

void ZegoMediaPlayerDemo::on_horizontalSlider_playVolume_1_valueChanged(int value)
{
    if(mediaPlayer1){
        mediaPlayer1->setPlayVolume(value);
    }
}

void ZegoMediaPlayerDemo::on_horizontalSlider_publishVolume_1_valueChanged(int value)
{
    if(mediaPlayer1){
        mediaPlayer1->setPublishVolume(value);
    }
}

void ZegoMediaPlayerDemo::on_comboBox_voiceChanger_1_currentIndexChanged(int index)
{
   ZegoVoiceChangerParam param = ZegoVoiceChangerParam(ZegoVoiceChangerPreset(index));
    mediaPlayer1->setVoiceChangerParam(ZEGO_MEDIA_PLAYER_AUDIO_CHANNEL_ALL, param);
}

void ZegoMediaPlayerDemo::on_pushButton_setAudioTrackIndex_1_clicked()
{
    mediaPlayer1->setAudioTrackIndex(ui->spinBox_audioTrackIndex_1->value());
}

void ZegoMediaPlayerDemo::onMediaPlayerStateUpdate(IZegoMediaPlayer *mediaPlayer, ZegoMediaPlayerState state, int errorCode){
    QStringList stateList = {
        "ZegoMediaPlayerStateNoPlay",
        "ZegoMediaPlayerStatePlaying",
        "ZegoMediaPlayerStatePausing",
        "ZegoMediaPlayerStatePlayEnded",
    };

    int mediaPlayerIndex = mediaPlayer==mediaPlayer1? 1:2;
    QString log = QString("onMediaPlayerStateUpdate: index=%1, state=%2, errorCode=%3").arg(mediaPlayerIndex).arg(stateList.value(state)).arg(errorCode);
    printLogToView(log);
}

void ZegoMediaPlayerDemo::onMediaPlayerNetworkEvent(IZegoMediaPlayer *mediaPlayer, ZegoMediaPlayerNetworkEvent event){
    QStringList eventList = {
        "ZegoMediaPlayerNetworkEventBufferBegin",
        "ZegoMediaPlayerNetworkEventBufferEnded",
    };

    int mediaPlayerIndex = mediaPlayer==mediaPlayer1? 1:2;
    QString log = QString("onMediaPlayerNetworkEvent: index=%1, event=%2").arg(mediaPlayerIndex).arg(eventList.value(event));
    printLogToView(log);
}

void ZegoMediaPlayerDemo::onMediaPlayerPlayingProgress(IZegoMediaPlayer *mediaPlayer, unsigned long long process){

    auto slider_progress = mediaPlayer==mediaPlayer1? ui->horizontalSlider_progress_1 : ui->horizontalSlider_progress_2;
    slider_progress->blockSignals(true);
    auto total = mediaPlayer->getTotalDuration();
	if (total == 0) {
        slider_progress->setValue(0);
	}
	else {
        slider_progress->setValue(int(100.0 * process * 1.0 / total));
	}
    slider_progress->blockSignals(false);
}

void ZegoMediaPlayerDemo::onVideoFrame(IZegoMediaPlayer *mediaPlayer, const unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param){
    Q_UNUSED(mediaPlayer)
    Q_UNUSED(data)
    Q_UNUSED(dataLength)
    Q_UNUSED(param)
}

void ZegoMediaPlayerDemo::onAudioFrame(IZegoMediaPlayer *mediaPlayer, const unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param){
    Q_UNUSED(mediaPlayer)
    Q_UNUSED(data)
    Q_UNUSED(dataLength)
    Q_UNUSED(param)
}

void ZegoMediaPlayerDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoMediaPlayerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    engine->setEventHandler(eventHandler);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoMediaPlayerDemo::printLogToView);

    auto mediaPlayerCallbackCenter = std::make_shared<ZegoExpressMediaPlayerCallbackCenter>();
    mediaPlayerCallbackCenter->setCallback(this);

    mediaPlayer1->setEventHandler(mediaPlayerCallbackCenter);
    mediaPlayer1->setAudioHandler(mediaPlayerCallbackCenter);
    mediaPlayer1->setVideoHandler(mediaPlayerCallbackCenter, ZEGO_VIDEO_FRAME_FORMAT_BGRA32);

    mediaPlayer2->setEventHandler(mediaPlayerCallbackCenter);
    mediaPlayer2->setAudioHandler(mediaPlayerCallbackCenter);
    mediaPlayer2->setVideoHandler(mediaPlayerCallbackCenter, ZEGO_VIDEO_FRAME_FORMAT_BGRA32);
}

void ZegoMediaPlayerDemo::on_pushButton_loadResource_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "Video (*.mp4);;audio (*.mp3)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEdit_resourcePath_2->setText(path);
    std::string sourcePath = ui->lineEdit_resourcePath_2->text().toStdString();
    mediaPlayer2->loadResource(sourcePath, [=](int errorCode){
        QString log = QString("loadResource: path=%1, errorCode=%2").arg(sourcePath.c_str()).arg(errorCode);
        printLogToView(log);

        mediaPlayer2->setProgressInterval(100);

        auto totalDuration = mediaPlayer2->getTotalDuration();
        ui->label_totalDuration_2->setText(QString::number(totalDuration/1000));

        auto playVolume = mediaPlayer2->getPlayVolume();
        ui->horizontalSlider_playVolume_2->setValue(playVolume);

        auto publishVolume = mediaPlayer2->getPublishVolume();
        ui->horizontalSlider_publishVolume_2->setValue(publishVolume);
    });
}

void ZegoMediaPlayerDemo::on_pushButton_startPlay_2_clicked()
{
    ZegoCanvas canvas(ZegoView(ui->frame_media_2->winId()));
    mediaPlayer2->setPlayerCanvas(&canvas);

    mediaPlayer2->enableRepeat(ui->checkBox_enableRepeat_2->isChecked());
    mediaPlayer2->start();
}

void ZegoMediaPlayerDemo::on_pushButton_pausePlay_2_clicked()
{
    mediaPlayer2->pause();
}

void ZegoMediaPlayerDemo::on_pushButton_resumePlay_2_clicked()
{
    mediaPlayer2->resume();
}

void ZegoMediaPlayerDemo::on_pushButton_stopPlay_2_clicked()
{
    mediaPlayer2->stop();
}

void ZegoMediaPlayerDemo::on_horizontalSlider_progress_2_valueChanged(int value)
{
    if(mediaPlayer2){
        auto seek = mediaPlayer2->getTotalDuration() * value / 100;
        mediaPlayer2->seekTo(seek, [=](int errorCode){
            QString log = QString("seekToTime: time(ms)=%1, errorCode=%2").arg(seek).arg(errorCode);
            printLogToView(log);
        });
    }
}

void ZegoMediaPlayerDemo::on_horizontalSlider_playVolume_2_valueChanged(int value)
{
    if(mediaPlayer2){
        mediaPlayer2->setPlayVolume(value);
    }
}

void ZegoMediaPlayerDemo::on_horizontalSlider_publishVolume_2_valueChanged(int value)
{
    if(mediaPlayer2){
        mediaPlayer2->setPublishVolume(value);
    }
}
