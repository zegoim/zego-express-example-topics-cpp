#include "ZegoMixerDemo.h"
#include "ui_ZegoMixerDemo.h"

#include "AppSupport/ZegoConfigManager.h"
#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include <QScrollBar>

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

    roomID = "MixerRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ZegoUser user(userID, userID);
    engine->loginRoom(roomID, user, nullptr);

    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(roomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    QStringList ZegoResolutionList = {
        "ZEGO_RESOLUTION_320x180" ,
        "ZEGO_RESOLUTION_480x270" ,
        "ZEGO_RESOLUTION_640x360" ,
        "ZEGO_RESOLUTION_960x540" ,
        "ZEGO_RESOLUTION_1280x720",
        "ZEGO_RESOLUTION_1920x1080"
    };
    ui->comboBox_mixerVideoConfig->addItems(ZegoResolutionList);
    ui->comboBox_mixerVideoConfig->setCurrentIndex(ZEGO_RESOLUTION_640x360);
}

ZegoMixerDemo::~ZegoMixerDemo()
{
    ZegoExpressEngine::destroyEngine(engine);
    engine = nullptr;

    delete ui;
}

void ZegoMixerDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    Q_UNUSED(roomID)
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

void ZegoMixerDemo::on_pushButton_start_mixer_task_clicked()
{
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
    mixerInput1.layout.x = 0;
    mixerInput1.layout.y = 0;
    mixerInput1.layout.width = videoConfig.width/2;
    mixerInput1.layout.height = videoConfig.height;

    ZegoMixerInput mixerInput2;
    mixerInput2.streamID = ui->comboBox_input_streamID2->currentText().toStdString();
    mixerInput2.contentType = ZegoMixerInputContentTypeVideo;
    mixerInput2.layout.x = videoConfig.width/2;
    mixerInput2.layout.y = 0;
    mixerInput2.layout.width = videoConfig.width/2;
    mixerInput2.layout.height = videoConfig.height;

    task.inputList = {mixerInput1, mixerInput2};

    // 4. MixerTask-OutputList
    ZegoMixerOutput mixerOutput;
    std::string target = ui->lineEdit_output_streamID->text().toStdString();
    mixerOutput.target = target;
    task.outputList = {mixerOutput};

    // 5. MixerTask-watermark:(option)
    ZegoWatermark watermark;
    watermark.imageURL = "preset-id://zegowp.png";
    watermark.layout = ZegoRect(0, 0, 100, 100);
    task.watermark = &watermark;


    // 6. MixerTask-backgroundImage:(option)
    task.backgroundImageURL = "preset-id://zegobg.png";

    // start mixer task
    QString log = QString("do start mixer task: taskID=%1, target=%2").arg(task.taskID.c_str()).arg(target.c_str());
    printLogToView(log);
    engine->startMixerTask(task, [=](ZegoMixerStartResult result){
        QString log = QString("start mixer task result: taskID=%1, errorCode=%2").arg(task.taskID.c_str()).arg(result.errorCode);
        printLogToView(log);
    });
}

void ZegoMixerDemo::on_pushButton_stop_mixer_task_clicked()
{
    if(this->mixerTaskID==""){
        QString log = QString("start mixer task first, then stop it");
        printLogToView(log);
    }
    else{
        QString log = QString("do stop mixer task: taskID=%1").arg(this->mixerTaskID.c_str());
        printLogToView(log);
        engine->stopMixerTask(this->mixerTaskID);
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
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoMixerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>(ui->textEdit_log);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRoomStreamUpdate, this, &ZegoMixerDemo::onRoomStreamUpdate);
    engine->addEventHandler(eventHandler);
}
