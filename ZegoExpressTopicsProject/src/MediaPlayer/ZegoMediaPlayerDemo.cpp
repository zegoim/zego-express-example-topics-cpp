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

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTest = ZegoConfigManager::instance()->isTestEnviroment();


    engine = ZegoExpressSDK::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
    mediaPlayer = engine->createMediaPlayer();
	bindEventHandler();
}

ZegoMediaPlayerDemo::~ZegoMediaPlayerDemo()
{
    engine->destroyMediaPlayer(mediaPlayer);
    ZegoExpressSDK::destroyEngine(engine);
    delete ui;
}

void ZegoMediaPlayerDemo::on_pushButton_loadResource_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "Video (*.mp4);;audio (*.mp3)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEdit_resourcePath->setText(path);
    std::string sourcePath = ui->lineEdit_resourcePath->text().toStdString();
    mediaPlayer->loadResource(sourcePath, [=](int errorCode){
        QString log = QString("loadResource: path=%1, errorCode=%2").arg(sourcePath.c_str()).arg(errorCode);
        printLogToView(log);

        mediaPlayer->setProcessInterval(100);

        auto totalDuration = mediaPlayer->getTotalDuration();
        ui->label_totalDuration->setText(QString::number(totalDuration/1000));

        auto volume = mediaPlayer->getVolume();
        ui->horizontalSlider_volume->setValue(volume);
    });
}

void ZegoMediaPlayerDemo::on_pushButton_stopPlay_clicked()
{
    mediaPlayer->stop();
}

void ZegoMediaPlayerDemo::on_pushButton_resumePlay_clicked()
{
    mediaPlayer->resume();
}

void ZegoMediaPlayerDemo::on_pushButton_pausePlay_clicked()
{
    mediaPlayer->pause();
}

void ZegoMediaPlayerDemo::on_pushButton_startPlay_clicked()
{
    ZegoCanvas canvas((void *)ui->frame_media->winId());
    mediaPlayer->setPlayerCanvas(&canvas);

    mediaPlayer->enableRepeat(ui->checkBox_enableRepeat->isChecked());
    mediaPlayer->start();
}

void ZegoMediaPlayerDemo::onMediaPlayerStateUpdate(ZegoMediaPlayerState state, int errorCode){
    QStringList stateList = {
        "ZegoMediaPlayerStateNoPlay",
        "ZegoMediaPlayerStateWillPlay",
        "ZegoMediaPlayerStatePlaying",
        "ZegoMediaPlayerStatePausing",
        "ZegoMediaPlayerStatePlayEnded",
    };

    QString log = QString("onMediaPlayerStateUpdate: state=%1, errorCode=%2").arg(stateList.value(state)).arg(errorCode);
    printLogToView(log);
}

void ZegoMediaPlayerDemo::onMediaPlayerNetWorkEvent(ZegoMediaPlayerNetworkEvent event){
    QStringList eventList = {
        "ZegoMediaPlayerNetworkEventBufferBegin",
        "ZegoMediaPlayerNetworkEventBufferEnded",
    };
    QString log = QString("onMediaPlayerNetWorkEvent: event=%1").arg(eventList.value(event));
    printLogToView(log);
}

void ZegoMediaPlayerDemo::onMediaPlayerPlayingProcess(unsigned long long process){

    ui->horizontalSlider_progress->blockSignals(true);
    auto total = mediaPlayer->getTotalDuration();
	if (total == 0) {
		ui->horizontalSlider_progress->setValue(0);
	}
	else {
        ui->horizontalSlider_progress->setValue(int(100.0 * process * 1.0 / total));
	}
    ui->horizontalSlider_progress->blockSignals(false);
}

void ZegoMediaPlayerDemo::onVideoFrame(const char **buffer, ZegoVideoFrameParam param){
    Q_UNUSED(buffer)
    Q_UNUSED(param)
}

void ZegoMediaPlayerDemo::onAudioFrame(const unsigned char *buffer, ZegoAudioFrameParam param){
    Q_UNUSED(buffer)
    Q_UNUSED(param)
}

void ZegoMediaPlayerDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoMediaPlayerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    engine->addEventHandler(eventHandler);

    auto mediaPlayerCallbackCenter = std::make_shared<ZegoExpressMediaPlayerCallbackCenter>();
    mediaPlayer->setEventHandler(mediaPlayerCallbackCenter);
    mediaPlayer->setAudioHandler(mediaPlayerCallbackCenter);
    mediaPlayer->setVideoHandler(ZEGO_VIDEO_FRAME_FORMAT_I420, mediaPlayerCallbackCenter);

    mediaPlayerCallbackCenter->setCallback(this);
}

void ZegoMediaPlayerDemo::on_horizontalSlider_progress_valueChanged(int value)
{
    if(mediaPlayer){
        auto seek = mediaPlayer->getTotalDuration() * value / 100;
        mediaPlayer->seekToTime(seek, [=](int errorCode){
            QString log = QString("seekToTime: time(ms)=%1, errorCode=%2").arg(seek).arg(errorCode);
            printLogToView(log);
        });
    }
}

void ZegoMediaPlayerDemo::on_horizontalSlider_volume_valueChanged(int value)
{
    if(mediaPlayer){
        mediaPlayer->setVolume(value);
    }
}
