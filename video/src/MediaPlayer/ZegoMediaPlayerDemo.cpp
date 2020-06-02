#include "ZegoMediaPlayerDemo.h"
#include "ui_ZegoMediaPlayerDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

#include <QFileDialog>

ZegoMediaPlayerDemo::ZegoMediaPlayerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMediaPlayerDemo)
{

    ui->setupUi(this);

    ZegoEngineConfig engineConfig;
    ZegoExpressSDK::setEngineConfig(engineConfig);
    
    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTest = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressSDK::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
    mediaPlayer1 = engine->createMediaPlayer();
    mediaPlayer2 = engine->createMediaPlayer();
	bindEventHandler();
}

ZegoMediaPlayerDemo::~ZegoMediaPlayerDemo()
{
    engine->destroyMediaPlayer(mediaPlayer1);
    engine->destroyMediaPlayer(mediaPlayer2);
    if(engine){
        ZegoExpressSDK::destroyEngine(engine);
    }
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

        auto volume = mediaPlayer1->getVolume();
        ui->horizontalSlider_volume_1->setValue(volume);
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

void ZegoMediaPlayerDemo::on_horizontalSlider_volume_1_valueChanged(int value)
{
    if(mediaPlayer1){
        mediaPlayer1->setVolume(value);
    }
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

void ZegoMediaPlayerDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoMediaPlayerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    engine->setEventHandler(eventHandler);

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

        auto volume = mediaPlayer2->getVolume();
        ui->horizontalSlider_volume_2->setValue(volume);
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

void ZegoMediaPlayerDemo::on_horizontalSlider_volume_2_valueChanged(int value)
{
    if(mediaPlayer2){
        mediaPlayer2->setVolume(value);
    }
}
