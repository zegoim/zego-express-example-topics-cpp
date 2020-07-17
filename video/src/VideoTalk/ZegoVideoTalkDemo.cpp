#include "ZegoVideoTalkDemo.h"
#include "ui_ZegoVideoTalkDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

ZegoVideoTalkDemo::ZegoVideoTalkDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoVideoTalkDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    videoList = {
        ui->frame_video_1,
        ui->frame_video_2,
        ui->frame_video_3,
        ui->frame_video_4,
        ui->frame_video_5,
        ui->frame_video_6,
        ui->frame_video_7,
        ui->frame_video_8,
        ui->frame_video_9,
        ui->frame_video_10,
        ui->frame_video_11,
    };

    currentRoomID = "VideoTalkRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
}

ZegoVideoTalkDemo::~ZegoVideoTalkDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoVideoTalkDemo::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
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
            engine->stopPlayingStream(stream.streamID);
            zegoStreamList.erase(it);
        }
    });


    for(auto widget : videoList){
        widget->update();
    }

    for (unsigned int i = 0; i< zegoStreamList.size(); i++) {
        auto stream = zegoStreamList.at(i);
        auto widget = videoList.value(int(i));
        if(widget){
            ZegoCanvas canvas(ZegoView(widget->winId()));
            engine->startPlayingStream(stream.streamID, &canvas);
        }
    }
}

void ZegoVideoTalkDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoVideoTalkDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoVideoTalkDemo::printLogToView);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigRoomStreamUpdate, this, &ZegoVideoTalkDemo::onRoomStreamUpdate);
    engine->setEventHandler(eventHandler);
}

void ZegoVideoTalkDemo::on_pushButton_login_clicked()
{
    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    ZegoCanvas canvas(ZegoView(ui->frame_video_0->winId()));
    engine->startPreview(&canvas);
    engine->startPublishingStream(user.userID);
}

void ZegoVideoTalkDemo::on_pushButton_logout_clicked()
{
    engine->logoutRoom(currentRoomID);
    zegoStreamList.clear();
}
