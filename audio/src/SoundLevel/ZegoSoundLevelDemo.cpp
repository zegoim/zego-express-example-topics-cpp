#include "ZegoSoundLevelDemo.h"
#include "ZegoSoundLevelDemo.h"
#include "ui_ZegoSoundLevelDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoSoundLevelDemo::ZegoSoundLevelDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoSoundLevelDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();
    
    currentRoomID = "SoundLevelRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);
    engine->startPublishingStream(userID);

    soundFrameLayout = new QVBoxLayout(ui->frame_content);
    addSoundFrame(userID);

    engine->startSoundLevelMonitor();
    engine->startAudioSpectrumMonitor();
    ui->checkBox_soundLevelMonitor->setChecked(true);
    ui->checkBox_audioSpectrumMonitor->setChecked(true);
}

ZegoSoundLevelDemo::~ZegoSoundLevelDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoSoundLevelDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    Q_UNUSED(roomID)

    for_each(streamList.begin(), streamList.end(), [&](ZegoStream stream){

        auto it  = std::find_if(zegoStreamList.begin(), zegoStreamList.end(), [&](ZegoStream const &_stream){
                return _stream.streamID == stream.streamID;
        });

        if(updateType == ZEGO_UPDATE_TYPE_ADD && it == zegoStreamList.end()){
            zegoStreamList.push_back(stream);
            engine->startPlayingStream(stream.streamID);
            addSoundFrame(stream.streamID);
        }

        if(updateType == ZEGO_UPDATE_TYPE_DELETE && it != zegoStreamList.end()){
            zegoStreamList.erase(it);
            engine->stopPlayingStream(stream.streamID);
            removeSoundFrame(stream.streamID);
        }
    });
}

void ZegoSoundLevelDemo::onCapturedSoundLevelUpdate(float soundLevel)
{
    if(!soundFrameMap.contains(userID)){
        return;
    }

    ZegoSoundFrame* soundFrame = soundFrameMap.value(userID);
    soundFrame->updateSoundLevel(soundLevel);
}

void ZegoSoundLevelDemo::onPlayerSoundLevelUpdate(const std::unordered_map<std::string, float> &soundLevels)
{
    for(auto iter=soundLevels.begin(); iter!=soundLevels.end(); iter++){
        if(!soundFrameMap.contains(iter->first)){
            continue;
        }

        ZegoSoundFrame* soundFrame = soundFrameMap.value(iter->first);
        soundFrame->updateSoundLevel(iter->second);
    }
}

void ZegoSoundLevelDemo::onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum &audioSpectrum)
{
    if(!soundFrameMap.contains(userID)){
        return;
    }

    ZegoSoundFrame* soundFrame = soundFrameMap.value(userID);
    soundFrame->updateAudioSpectrum(audioSpectrum);
}

void ZegoSoundLevelDemo::onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum> &audioSpectrums)
{
    for(auto iter=audioSpectrums.begin(); iter!=audioSpectrums.end(); iter++){
        if(!soundFrameMap.contains(iter->first)){
            continue;
        }

        ZegoSoundFrame* soundFrame = soundFrameMap.value(iter->first);
        soundFrame->updateAudioSpectrum(iter->second);
    }
}

void ZegoSoundLevelDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);

    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRoomStreamUpdate, this, &ZegoSoundLevelDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigCapturedSoundLevelUpdate, this, &ZegoSoundLevelDemo::onCapturedSoundLevelUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRemoteSoundLevelUpdate, this, &ZegoSoundLevelDemo::onPlayerSoundLevelUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigCapturedAudioSpectrumUpdate, this, &ZegoSoundLevelDemo::onCapturedAudioSpectrumUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRemoteAudioSpectrumUpdate, this, &ZegoSoundLevelDemo::onRemoteAudioSpectrumUpdate);

    engine->setEventHandler(eventHandler);
}

void ZegoSoundLevelDemo::addSoundFrame(std::string streamID)
{
    if(soundFrameMap.contains(streamID)){
        return;
    }

    ZegoSoundFrame *soundFrame = new ZegoSoundFrame(nullptr, streamID);
    soundFrameMap.insert(streamID, soundFrame);
    soundFrameLayout->addWidget(soundFrame);
}

void ZegoSoundLevelDemo::removeSoundFrame(std::string streamID)
{
    if(!soundFrameMap.contains(streamID)){
        return;
    }

    ZegoSoundFrame *soundFrame = soundFrameMap.take(streamID);
    soundFrameLayout->removeWidget(soundFrame);
    soundFrame->deleteLater();
}

void ZegoSoundLevelDemo::on_checkBox_soundLevelMonitor_clicked()
{
    if(ui->checkBox_soundLevelMonitor->isChecked()){
        engine->startSoundLevelMonitor();
    }
    else{
        engine->stopSoundLevelMonitor();
    }
}

void ZegoSoundLevelDemo::on_checkBox_audioSpectrumMonitor_clicked()
{
    if(ui->checkBox_audioSpectrumMonitor->isChecked()){
        engine->startAudioSpectrumMonitor();
    }
    else{
        engine->stopAudioSpectrumMonitor();
    }
}
