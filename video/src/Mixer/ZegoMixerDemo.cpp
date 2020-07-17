#include "ZegoMixerDemo.h"
#include "ui_ZegoMixerDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include <QScrollBar>


ZegoMixerDemo::ZegoMixerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMixerDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "MixerRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
}

ZegoMixerDemo::~ZegoMixerDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);

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

        if(updateType == ZEGO_UPDATE_TYPE_DELETE && it != zegoStreamList.end()){
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
    /*  Output
     *
     * |-----------|-------|----------------|
     * | watermark |       |                |
     * |-----------|       |                |
     * |                   |                |
     * |      Input1       |    Input2      |
     * |                   |                |
     * |                   |                |
     * |                   |                |
     * |-------------------|----------------|
    */

    // 0. MixerTask
    auto mixerTaskID = ZegoUtilHelper::getRandomString();
    ZegoMixerTask task(mixerTaskID);

    // 1 Set VideoConfig for the output
    ZegoMixerVideoConfig videoConfig;
    task.videoConfig = videoConfig;

    // 2 Set AudioConfig for the output
    ZegoMixerAudioConfig audioConfig;
    task.audioConfig = audioConfig;

    // 3. MixerTask-OutputList
    ZegoMixerOutput mixerOutput;
    std::string target = ui->lineEdit_output_streamID->text().toStdString();
    mixerOutput.target = target;
    task.outputList = {mixerOutput};

    // 4. MixerTask-InputList

    // 4.1 Set the first Input
    ZegoMixerInput mixerInput1;
    mixerInput1.streamID = ui->comboBox_input_streamID1->currentText().toStdString();
    mixerInput1.contentType = ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO;
    // the first input layout the left of the output
    mixerInput1.layout.x = 0;
    mixerInput1.layout.y = 0;
    mixerInput1.layout.width = videoConfig.width/2;
    mixerInput1.layout.height = videoConfig.height;

    // 4.2 Set the second Input
    ZegoMixerInput mixerInput2;
    mixerInput2.streamID = ui->comboBox_input_streamID2->currentText().toStdString();
    mixerInput2.contentType = ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO;
    // the second input layout the right of the output
    mixerInput2.layout.x = videoConfig.width/2;
    mixerInput2.layout.y = 0;
    mixerInput2.layout.width = videoConfig.width/2;
    mixerInput2.layout.height = videoConfig.height;

    task.inputList = {mixerInput1, mixerInput2};

    // 5. MixerTask-watermark:(option)
    watermark = std::make_shared<ZegoWatermark>();
    watermark->imageURL = "preset-id://zegowp.png";
    // the watermark layout the top-left corner of the output
    watermark->layout = ZegoRect(0, 0, 100, 100);
    task.watermark = watermark.get();

    // 6. MixerTask-backgroundImage:(option)
    task.backgroundImageURL = "preset-id://zegobg.png";

    currentTask = task;

    // start mixer task
    QString log = QString("do start mixer task: taskID=%1, target=%2").arg(task.taskID.c_str()).arg(target.c_str());
    printLogToView(log);
    engine->startMixerTask(task, [=](int errorCode, std::string extendData){
        QString log = QString("start mixer task result: taskID=%1, errorCode=%2, extendData=%3").arg(task.taskID.c_str()).arg(errorCode).arg(extendData.c_str());
        printLogToView(log);
    });
}

void ZegoMixerDemo::on_pushButton_stop_mixer_task_clicked()
{    
    if(currentTask.taskID==""){
        QString log = QString("start mixer task first, then stop it");
        printLogToView(log);
    }
    else{
        QString log = QString("do stop mixer task: taskID=%1").arg(currentTask.taskID.c_str());
        printLogToView(log);
        engine->stopMixerTask(currentTask, [=](int errorCode){
            QString log = QString("stop mixer task result: taskID=%1, errorCode=%2").arg(currentTask.taskID.c_str()).arg(errorCode);
            printLogToView(log);
        });
    }
}

void ZegoMixerDemo::on_pushButton_start_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_mixer_video->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoMixerDemo::on_pushButton_stop_play_clicked()
{
    std::string streamID = ui->lineEdit_streamID_to_play->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoMixerDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoMixerDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoMixerDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRoomStreamUpdate, this, &ZegoMixerDemo::onRoomStreamUpdate);
    engine->setEventHandler(eventHandler);
}
