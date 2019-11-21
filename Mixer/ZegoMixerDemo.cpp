#include "ZegoMixerDemo.h"
#include "ui_ZegoMixerDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerQt.h"

std::string ZegoMixerDemo::mixerTaskID;

ZegoMixerDemo::ZegoMixerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMixerDemo)
{
    ui->setupUi(this);

    auto appID = ZegoConfigManager::instance()->getAppID();
    auto appSign = ZegoConfigManager::instance()->getAppSign();
    auto isTestEnv = ZegoConfigManager::instance()->isTestEnviroment();

    engine = ZegoExpressEngine::createEngine(appID, appSign, isTestEnv, ZEGO_SCENARIO_GENERAL, nullptr);
    bindEventHandler();

    std::string roomID = "MixerRoom-1";
    auto random = ZegoUtilHelper::getRandomString();
    ZegoUser user(random, random);
    engine->loginRoom(roomID, user, nullptr);
    QString log = QString("do loginRoom: roomID=%1, userID=%2").arg(roomID.c_str()).arg(user.userID.c_str());
    printLogToView(log);

    ui->pushButton_mixer_title->setText(QString("MixerRoomID: %1").arg(roomID.c_str()));

    QStringList ZegoResolutionList = {
        "ZEGO_RESOLUTION_180x320" ,
        "ZEGO_RESOLUTION_270x480" ,
        "ZEGO_RESOLUTION_360x640" ,
        "ZEGO_RESOLUTION_540x960" ,
        "ZEGO_RESOLUTION_720x1280",
        "ZEGO_RESOLUTION_1080x1920"
    };
    ui->comboBox_mixerVideoConfig->addItems(ZegoResolutionList);
    ui->comboBox_mixerVideoConfig->setCurrentIndex(2);

    ui->spinBox_input_left1->setValue(0);
    ui->spinBox_input_top1->setValue(0);
    ui->spinBox_input_right1->setValue(360);
    ui->spinBox_input_bottom1->setValue(320);

    ui->spinBox_input_left2->setValue(0);
    ui->spinBox_input_top2->setValue(320);
    ui->spinBox_input_right2->setValue(360);
    ui->spinBox_input_bottom2->setValue(640);

}

ZegoMixerDemo::~ZegoMixerDemo()
{
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoMixerDemo::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(QString::fromStdString(funcName)).arg(QString::fromStdString(info.c_str()));
    printLogToView(log);
}

void ZegoMixerDemo::onRoomStateUpdate(const std::string &roomID, ZegoRoomState roomState, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, roomState=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(roomState)).arg(errorCode);
    printLogToView(log);
}

void ZegoMixerDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList streamIDs;
    for(const ZegoStream &stream : streamList){
        streamIDs.append(QString::fromStdString(stream.streamID));
    }
    QString log = QString("onStreamUpdate: roomID=%1, updateType=%2, streamIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(streamIDs.join(","));
    printLogToView(log);


    for_each(streamList.begin(), streamList.end(), [&](ZegoStream stream){

        auto it  = std::find_if(zegoStreamList.begin(), zegoStreamList.end(), [&](ZegoStream const &_stream){
                return _stream.streamID == stream.streamID;
        });

        if(updateType == ZEGO_UPDATE_TYPE_ADD && it == zegoStreamList.end()){
            zegoStreamList.push_back(stream);
        }

        if(updateType == ZEGO_UPDATE_TYPE_DEL && it != zegoStreamList.end()){
            zegoStreamList.erase(it);
        }

    });

    {
        QStringList streamIDs;
        for(auto &stream: zegoStreamList){
            streamIDs.append(stream.streamID.c_str());
        }
        ui->comboBox_input_streamID1->clear();
        ui->comboBox_input_streamID2->clear();
        ui->comboBox_input_streamID1->addItems(streamIDs);
        ui->comboBox_input_streamID2->addItems(streamIDs);
    }

}

void ZegoMixerDemo::onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo> &infoList)
{

}

void ZegoMixerDemo::on_pushButton_start_mixer_task_clicked()
{
    if(this->mixerTaskID!=""){
        engine->stopMixerTask(this->mixerTaskID, [=](int errorCode){

        });
    }

    // 0. MixerTask
    this->mixerTaskID = ZegoUtilHelper::getRandomString();
    ZegoMixerTask task(this->mixerTaskID);

    // 1. MixerTask-VideoConfig
    ZegoMixerVideoConfig videoConfig(ZegoResolution(ui->comboBox_mixerVideoConfig->currentIndex()));
    task.videoConfig = videoConfig;

    // 2. MixerTask-AudioConfig
    ZegoMixerAudioConfig audioConfig;
    task.audioConfig = audioConfig;

    // 3. MixerTask-InputList
    ZegoMixerInput mixerInput1;
    mixerInput1.streamID = ui->comboBox_input_streamID1->currentText().toStdString();
    mixerInput1.contentType = ZegoMixerInputContentTypeVideo;
    mixerInput1.layout.left = ui->spinBox_input_left1->value();
    mixerInput1.layout.top = ui->spinBox_input_top1->value();
    mixerInput1.layout.right = ui->spinBox_input_right1->value();
    mixerInput1.layout.bottom = ui->spinBox_input_bottom1->value();

    ZegoMixerInput mixerInput2;
    mixerInput2.streamID = ui->comboBox_input_streamID2->currentText().toStdString();
    mixerInput2.contentType = ZegoMixerInputContentTypeVideo;
    mixerInput2.layout.left = ui->spinBox_input_left2->value();
    mixerInput2.layout.top = ui->spinBox_input_top2->value();
    mixerInput2.layout.right = ui->spinBox_input_right2->value();
    mixerInput2.layout.bottom = ui->spinBox_input_bottom2->value();

    task.inputList = {mixerInput1, mixerInput2};

    // 4. MixerTask-OutputList
    ZegoMixerOutput mixerOutput;
    mixerOutput.target = ui->lineEdit_output_streamID->text().toStdString();
    task.outputList = {mixerOutput};

    // 5. MixerTask-watermask:(option)

    // 6. MixerTask-backgroundImage:(option)

    // start mixer task
    engine->startMixerTask(task, [=](ZegoMixerStartResult result){
        QString log = QString("start mixer task result: errorCode=%1").arg(result.errorCode);
        printLogToView(log);
    });
}

void ZegoMixerDemo::on_pushButton_stop_mixer_task_clicked()
{
    if(this->mixerTaskID!=""){
        engine->stopMixerTask(this->mixerTaskID, [=](int errorCode){

        });
    }
}

void ZegoMixerDemo::on_pushButton_start_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    ZegoCanvas canvas((void *)ui->frame_mixer_video->winId(), ZEGO_VIEW_MODE_ASPECT_FIT);
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoMixerDemo::on_pushButton_stop_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoMixerDemo::printLogToView(QString log)
{
    ui->textEdit_log->append(log);
}

void ZegoMixerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerQt>();

    connect(eventHandler.get(), &ZegoEventHandlerQt::sigDebugError, this, &ZegoMixerDemo::onDebugError);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStateUpdate, this, &ZegoMixerDemo::onRoomStateUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigRoomStreamUpdate, this, &ZegoMixerDemo::onRoomStreamUpdate);
    connect(eventHandler.get(), &ZegoEventHandlerQt::sigMixerRelayCDNStateUpdate, this, &ZegoMixerDemo::onMixerRelayCDNStateUpdate);

    engine->addEventHandler(eventHandler);
}
