#include "ZegoPlayDemo.h"
#include "ui_ZegoPlayDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoPlayDemo::ZegoPlayDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoPlayDemo)
{
    ui->setupUi(this);
    
    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    ui->comboBox_audioOutputDevice->blockSignals(true);
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT);
    for(auto device : audioList){
        ui->comboBox_audioOutputDevice->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_audioOutputDevice->blockSignals(false);

    ui->comboBox_viewmode->blockSignals(true);
    QStringList viewModeList = {
        "ASPECT_FIT",
        "ASPECT_FILL",
        "SCALE_TO_FILL"
    };
    ui->comboBox_viewmode->addItems(viewModeList);
    ui->comboBox_viewmode->blockSignals(false);

    ui->slider_playVolume->blockSignals(true);
    ui->slider_playVolume->setValue(100);
    ui->slider_playVolume->blockSignals(false);
}

ZegoPlayDemo::~ZegoPlayDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoPlayDemo::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    Q_UNUSED(streamID)
    ui->lineEdit_videoBPS->setText(QString::number(quality.videoKBPS) + "kbps");
    ui->lineEdit_videoFPS->setText(QString::number(quality.videoRecvFPS) + "fps");

    ui->lineEdit_audioBPS->setText(QString::number(quality.audioKBPS) + "kbps");
    ui->lineEdit_audioFPS->setText(QString::number(quality.audioRecvFPS) + "fps");
}

void ZegoPlayDemo::onPlayerVideoSizeChanged(const std::string& streamID, int width, int height)
{
    Q_UNUSED(streamID)
    ui->lineEdit_resolution->setText(QString("%1x%2").arg(width).arg(height));
}

void ZegoPlayDemo::onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo &deviceInfo)
{
    Q_UNUSED(updateType)
    Q_UNUSED(deviceType)
    Q_UNUSED(deviceInfo)
    ui->comboBox_audioOutputDevice->blockSignals(true);
    auto audioList = engine->getAudioDeviceList(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT);
    ui->comboBox_audioOutputDevice->clear();
    for(auto device : audioList){
        ui->comboBox_audioOutputDevice->addItem(QString::fromStdString(device.deviceID));
    }
    ui->comboBox_audioOutputDevice->blockSignals(false);
}

void ZegoPlayDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoPlayDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoPlayDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPlayerQualityUpdate, this, &ZegoPlayDemo::onPlayerQualityUpdate);
	connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPlayerVideoSizeChanged, this, &ZegoPlayDemo::onPlayerVideoSizeChanged);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigAudioDeviceStateChanged, this, &ZegoPlayDemo::onAudioDeviceStateChanged);
    engine->setEventHandler(eventHandler);
}

void ZegoPlayDemo::on_comboBox_audioOutputDevice_currentIndexChanged(const QString &arg1)
{
    engine->useAudioDevice(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT, arg1.toStdString());
    ui->slider_speakerVolume->setValue(engine->getAudioDeviceVolume(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT, arg1.toStdString()));
}

void ZegoPlayDemo::on_comboBox_viewmode_currentIndexChanged(int index)
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()), ZegoViewMode(index));

    engine->startPlayingStream(streamID, &canvas);
}

void ZegoPlayDemo::on_checkBox_mutePlayStreamAudio_clicked(bool checked)
{
    engine->mutePlayStreamAudio(ui->lineEdit_streamID->text().toStdString(), checked);
}

void ZegoPlayDemo::on_checkBox_mutePlayStreamVideo_clicked(bool checked)
{
    engine->mutePlayStreamVideo(ui->lineEdit_streamID->text().toStdString(), checked);
}

void ZegoPlayDemo::on_pushButton_startPlay_clicked()
{
    std::string userID = ui->lineEdit_userID->text().toStdString();
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    currentRoomID = ui->lineEdit_roomID->text().toStdString();

    ZegoUser user;
    user.userID = userID;
    user.userName = userID;
    engine->loginRoom(currentRoomID, user);

    ZegoCanvas canvas(ZegoView(ui->frame_remote_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoPlayDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
    engine->logoutRoom(currentRoomID);

    ui->slider_playVolume->setValue(100);
    ui->checkBox_mutePlayStreamAudio->setChecked(false);
    ui->checkBox_mutePlayStreamVideo->setChecked(false);
}

void ZegoPlayDemo::on_slider_playVolume_valueChanged(int value)
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->setPlayVolume(streamID, value);
}

void ZegoPlayDemo::on_slider_speakerVolume_valueChanged(int value)
{
    engine->setAudioDeviceVolume(ZEGO_AUDIO_DEVICE_TYPE_OUTPUT, ui->comboBox_audioOutputDevice->currentText().toStdString(), value);
}
