#include "ZegoAudioEffectPlayerDemo.h"
#include "ui_ZegoAudioEffectPlayerDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

#include <QFileDialog>

ZegoAudioEffectPlayerDemo::ZegoAudioEffectPlayerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoAudioEffectPlayerDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    audioEffectPlayer = engine->createAudioEffectPlayer();

    currentRoomID = "AudioEffectID-1";

    std::string userID = ZegoUtilHelper::getRandomString();
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
}

ZegoAudioEffectPlayerDemo::~ZegoAudioEffectPlayerDemo()
{
    engine->destroyAudioEffectPlayer(audioEffectPlayer);
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoAudioEffectPlayerDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoAudioEffectPlayerDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_startPlayImmediately_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "audio (*.mp3);;audio (*.wav);;Video (*.mp4)");
    if(path.isEmpty()){
        return;
    }

    std::string sourcePath = path.toStdString();
    audioEffectPlayer->start(getUniqueAudioEffectID(), sourcePath, nullptr);
}

void ZegoAudioEffectPlayerDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoAudioEffectPlayerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoAudioEffectPlayerDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

unsigned int ZegoAudioEffectPlayerDemo::getUniqueAudioEffectID()
{
    static unsigned int uniqueEffectID = 0;
    return ++uniqueEffectID;
}

void ZegoAudioEffectPlayerDemo::on_pushButton_loadResource_1_clicked()
{
    if(audioEffectIDFirst != 0){
        printLogToView(QString("please unload old resource first"));
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "audio (*.mp3);;audio (*.wav)");
    if(path.isEmpty()){
        return;
    }

    unsigned int newAudioEffectID = getUniqueAudioEffectID();
    audioEffectPlayer->loadResource(newAudioEffectID, path.toStdString(), [=](int errorCode){
        if(errorCode==0){
            audioEffectIDFirst = newAudioEffectID;
            ui->lineEdit_effect_1->setText(path);
            printLogToView(QString("load resource succeed path=%1").arg(path));
        }
        else{
            printLogToView(QString("load resource failed path=%1").arg(path));
        }
    });
}

void ZegoAudioEffectPlayerDemo::on_pushButton_unloadResource_1_clicked()
{
    if(audioEffectIDFirst == 0){
        printLogToView(QString("please load resource first before unload"));
        return;
    }

    ui->lineEdit_effect_1->setText("");
    audioEffectPlayer->stop(audioEffectIDFirst);
    audioEffectPlayer->unloadResource(audioEffectIDFirst);
    audioEffectIDFirst = 0;
}

void ZegoAudioEffectPlayerDemo::on_pushButton_start_1_clicked()
{
    if(audioEffectIDFirst == 0){
        printLogToView(QString("please load resource first before start"));
        return;
    }

    ZegoAudioEffectPlayConfig playConfig;
    playConfig.isPublishOut = ui->checkBox_isPublishOut_1->isChecked();
    playConfig.playCount = ui->spinBox_playCount_1->value();
    audioEffectPlayer->start(audioEffectIDFirst, std::string(), &playConfig);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_stop_1_clicked()
{
    if(audioEffectIDFirst == 0){
        printLogToView(QString("please load resource first before stop"));
        return;
    }

    audioEffectPlayer->stop(audioEffectIDFirst);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_pause_1_clicked()
{
    if(audioEffectIDFirst == 0){
        printLogToView(QString("please load resource first before pause"));
        return;
    }

    audioEffectPlayer->pause(audioEffectIDFirst);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_resume_1_clicked()
{
    if(audioEffectIDFirst == 0){
        printLogToView(QString("please load resource first before resume"));
        return;
    }

    audioEffectPlayer->resume(audioEffectIDFirst);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_stopAll_clicked()
{
    audioEffectPlayer->stopAll();
}

void ZegoAudioEffectPlayerDemo::on_pushButton_pauseAll_clicked()
{
    audioEffectPlayer->pauseAll();
}

void ZegoAudioEffectPlayerDemo::on_pushButton_ResumeAll_clicked()
{
    audioEffectPlayer->resumeAll();
}

void ZegoAudioEffectPlayerDemo::on_pushButton_loadResource_2_clicked()
{
    if(audioEffectIDSecond != 0){
        printLogToView(QString("please unload old resource first"));
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select file to play", ".", "audio (*.mp3);;audio (*.wav)");
    if(path.isEmpty()){
        return;
    }

    unsigned int newAudioEffectID = getUniqueAudioEffectID();
    audioEffectPlayer->loadResource(newAudioEffectID, path.toStdString(), [=](int errorCode){
        if(errorCode==0){
            audioEffectIDSecond = newAudioEffectID;
            ui->lineEdit_effect_2->setText(path);
            printLogToView(QString("load resource succeed path=%2").arg(path));
        }
        else{
            printLogToView(QString("load resource failed path=%2").arg(path));
        }
    });
}

void ZegoAudioEffectPlayerDemo::on_pushButton_unloadResource_2_clicked()
{
    if(audioEffectIDSecond == 0){
        printLogToView(QString("please load resource first before unload"));
        return;
    }

    ui->lineEdit_effect_2->setText("");
    audioEffectPlayer->stop(audioEffectIDSecond);
    audioEffectPlayer->unloadResource(audioEffectIDSecond);
    audioEffectIDSecond = 0;
}

void ZegoAudioEffectPlayerDemo::on_pushButton_start_2_clicked()
{
    if(audioEffectIDSecond == 0){
        printLogToView(QString("please load resource first before start"));
        return;
    }

    ZegoAudioEffectPlayConfig playConfig;
    playConfig.isPublishOut = ui->checkBox_isPublishOut_2->isChecked();
    playConfig.playCount = ui->spinBox_playCount_2->value();
    audioEffectPlayer->start(audioEffectIDSecond, std::string(), &playConfig);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_stop_2_clicked()
{
    if(audioEffectIDSecond == 0){
        printLogToView(QString("please load resource first before stop"));
        return;
    }

    audioEffectPlayer->stop(audioEffectIDSecond);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_pause_2_clicked()
{
    if(audioEffectIDSecond == 0){
        printLogToView(QString("please load resource first before pause"));
        return;
    }

    audioEffectPlayer->pause(audioEffectIDSecond);
}

void ZegoAudioEffectPlayerDemo::on_pushButton_resume_2_clicked()
{
    if(audioEffectIDSecond == 0){
        printLogToView(QString("please load resource first before resume"));
        return;
    }

    audioEffectPlayer->resume(audioEffectIDSecond);
}
