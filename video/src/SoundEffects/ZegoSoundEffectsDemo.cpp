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
        "ZEGO_REVERB_PRESET_VALLEY",
        "ZEGO_REVERB_PRESET_RECORDING_STUDIO",
        "ZEGO_REVERB_PRESET_BASEMENT",
        "ZEGO_REVERB_PRESET_KTV",
        "ZEGO_REVERB_PRESET_POPULAR",
        "ZEGO_REVERB_PRESET_ROCK",
        "ZEGO_REVERB_PRESET_VOCAL_CONCERT"
    };
    ui->comboBox_reverbPreset->addItems(ZegoReverbPresetList);

    QStringList ZegoVoiceChangerPresetList = {
        "ZEGO_VOICE_CHANGER_PRESET_NONE",
        "ZEGO_VOICE_CHANGER_PRESET_MEN_TO_CHILD",
        "ZEGO_VOICE_CHANGER_PRESET_MEN_TO_WOMEN",
        "ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_CHILD",
        "ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_MEN",
        "ZEGO_VOICE_CHANGER_PRESET_FOREIGNER",
        "ZEGO_VOICE_CHANGER_PRESET_OPTIMUS_PRIME",
        "ZEGO_VOICE_CHANGER_PRESET_ANDROID",
        "ZEGO_VOICE_CHANGER_PRESET_ETHEREAL",
        "ZEGO_VOICE_CHANGER_PRESET_MALE_MAGNETIC",
        "ZEGO_VOICE_CHANGER_PRESET_FEMALE_FRESH"
    };
    ui->comboBox_voiceChangerPreset->addItems(ZegoVoiceChangerPresetList);

    auto audioConfig = engine->getAudioConfig();
    audioConfig.channel = ZEGO_AUDIO_CHANNEL_STEREO;
    engine->setAudioConfig(audioConfig);
}

ZegoSoundEffectsDemo::~ZegoSoundEffectsDemo()
{
    auto audioConfig = engine->getAudioConfig();
    audioConfig.channel = ZEGO_AUDIO_CHANNEL_MONO;
    engine->setAudioConfig(audioConfig);

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

void ZegoSoundEffectsDemo::updateReverbAdvanceParam()
{
    auto roomSize = ui->horizontalSlider_roomSize->value();
    auto reverberance = ui->horizontalSlider_reverberance->value();
    auto damping = ui->horizontalSlider_damping->value();
    auto wetOnly = ui->checkBox_wetOnly->isChecked();
    auto wetGain = ui->horizontalSlider_wetGain->value();
    auto dryGain = ui->horizontalSlider_dryGain->value();
    auto toneLow = ui->horizontalSlider_toneLow->value();
    auto toneHigh = ui->horizontalSlider_toneHigh->value();
    auto preDelay = ui->horizontalSlider_preDelay->value();
    auto stereoWidth = ui->horizontalSlider_stereoWidth->value();
    
    ZegoReverbAdvancedParam advanceParam;
    advanceParam.roomSize = roomSize;
    advanceParam.reverberance = reverberance;
    advanceParam.damping = damping;
    advanceParam.wetOnly = wetOnly;
    advanceParam.wetGain = wetGain;
    advanceParam.dryGain = dryGain;
    advanceParam.toneLow = toneLow;
    advanceParam.toneHigh = toneHigh;
    advanceParam.preDelay = preDelay;
    advanceParam.stereoWidth = stereoWidth;
    
    engine->setReverbAdvancedParam(advanceParam);
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

void ZegoSoundEffectsDemo::on_pushButton_setReverbPreset_clicked()
{
    engine->setReverbPreset(ZegoReverbPreset(ui->comboBox_reverbPreset->currentIndex()));
}

void ZegoSoundEffectsDemo::on_pushButton_setVoiceChangerPreset_clicked()
{
    engine->setVoiceChangerPreset(ZegoVoiceChangerPreset(ui->comboBox_voiceChangerPreset->currentIndex()));
}

void ZegoSoundEffectsDemo::on_checkBox_enableVirtualStereo_clicked()
{
    bool enableVirtualStereo = ui->checkBox_enableVirtualStereo->isChecked();
    engine->enableVirtualStereo(enableVirtualStereo, ui->horizontalSlider_virtualStereoAngle->value());
    ui->horizontalSlider_virtualStereoAngle->setEnabled(enableVirtualStereo);
}

void ZegoSoundEffectsDemo::on_horizontalSlider_virtualStereoAngle_valueChanged(int value)
{
    engine->enableVirtualStereo(ui->checkBox_enableVirtualStereo->isChecked(), value);
}

void ZegoSoundEffectsDemo::on_horizontalSlider_roomSize_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_reverberance_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_damping_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_wetGain_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_dryGain_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_toneLow_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_toneHigh_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_preDelay_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}

void ZegoSoundEffectsDemo::on_horizontalSlider_stereoWidth_valueChanged(int value)
{
    Q_UNUSED(value);
    updateReverbAdvanceParam();
}
