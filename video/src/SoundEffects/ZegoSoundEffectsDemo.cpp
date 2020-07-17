#include "ZegoSoundEffectsDemo.h"
#include "ui_ZegoSoundEffectsDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoSoundEffectsDemo::ZegoSoundEffectsDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoSoundEffectsDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "SoundEffects-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    QStringList ZegoReverbPresetList = {
        "ZEGO_REVERB_PRESET_NONE",
        "ZEGO_REVERB_PRESET_SOFT_ROOM",
        "ZEGO_REVERB_PRESET_LARGE_ROOM",
        "ZEGO_REVERB_PRESET_CONCER_HALL",
        "ZEGO_REVERB_PRESET_VALLEY"
    };
    ui->comboBox_reverbPreset->addItems(ZegoReverbPresetList);

    QStringList ZegoVoiceChangerPresetList = {
        "ZEGO_VOICE_CHANGER_PRESET_NONE",
        "ZEGO_VOICE_CHANGER_PRESET_MEN_TO_CHILD",
        "ZEGO_VOICE_CHANGER_PRESET_MEN_TO_WOMEN",
        "ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_CHILD",
        "ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_MEN"
    };
    ui->comboBox_voiceChangerPreset->addItems(ZegoVoiceChangerPresetList);
}

ZegoSoundEffectsDemo::~ZegoSoundEffectsDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoSoundEffectsDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoSoundEffectsDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoSoundEffectsDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoSoundEffectsDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoSoundEffectsDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoSoundEffectsDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoSoundEffectsDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoSoundEffectsDemo::on_pushButton_setReverbParam_clicked()
{
    ZegoReverbParam param(ZegoReverbPreset(ui->comboBox_reverbPreset->currentIndex()));
    engine->setReverbParam(param);
}

void ZegoSoundEffectsDemo::on_pushButton_setVoiceChangerParam_clicked()
{
    ZegoVoiceChangerParam param(ZegoVoiceChangerPreset(ui->comboBox_voiceChangerPreset->currentIndex()));
    engine->setVoiceChangerParam(param);
}

void ZegoSoundEffectsDemo::on_pushButton_virtualStereo_clicked()
{
    engine->enableVirtualStereo(ui->checkBox_enableVirtualStereo->isChecked(), ui->horizontalSlider_virtualStereoAngle->value());
}
