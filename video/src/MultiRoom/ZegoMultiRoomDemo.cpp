#include "ZegoMultiRoomDemo.h"
#include "ui_ZegoMultiRoomDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

ZegoMultiRoomDemo::ZegoMultiRoomDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMultiRoomDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    mainRoomID = "MultiRoom-main";
    auxRoomID = "MultiRoom-aux";
    userID = ZegoUtilHelper::getRandomString();

    ui->pushButton_roomID->setText(QString("First-RoomID: %1 \n Second-RoomID: %2").arg(mainRoomID.c_str(), auxRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));
    ui->comboBox_roomExtraInfo->addItems({mainRoomID.c_str(), auxRoomID.c_str()});
    ui->comboBox_broadcastMessage->addItems({mainRoomID.c_str(), auxRoomID.c_str()});
}

ZegoMultiRoomDemo::~ZegoMultiRoomDemo()
{
    engine->logoutRoom(auxRoomID);
    engine->logoutRoom(mainRoomID);

    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoMultiRoomDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoMultiRoomDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    engine->setEventHandler(eventHandler);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoMultiRoomDemo::printLogToView);
}

void ZegoMultiRoomDemo::on_pushButton_login_clicked()
{
    // login main room first
    ZegoUser user(userID, userID);
    engine->loginRoom(mainRoomID, user);

    // then login aux room
    engine->loginMultiRoom(auxRoomID);
}

void ZegoMultiRoomDemo::on_pushButton_logout_clicked()
{
    // logout aux room first
    engine->logoutRoom(auxRoomID);

    // then logout main room
    engine->logoutRoom(mainRoomID);
}

void ZegoMultiRoomDemo::on_pushButton_setRoomExtraInfo_clicked()
{
    std::string roomID = ui->comboBox_roomExtraInfo->currentText().toStdString();
    std::string key = ui->lineEdit_roomExtraInfo_key->text().toStdString();
    std::string value = ui->lineEdit_roomExtraInfo_value->text().toStdString();
    engine->setRoomExtraInfo(roomID, key, value, [=](int errorCode){
        printLogToView(QString("setRoomExtraInfo\t: roomID=%1, key=%2, value=%3 errorCode=%4").arg(roomID.c_str()).arg(key.c_str()).arg(value.c_str()).arg(errorCode));
    });
}

void ZegoMultiRoomDemo::on_pushButton_sendBroadcastMessage_clicked()
{
    std::string roomID = ui->comboBox_broadcastMessage->currentText().toStdString();
    std::string message = ui->lineEdit_broadcastMessage->text().toStdString();

    engine->sendBroadcastMessage(roomID, message,  [=](int errorCode, unsigned long long messageID){
        printLogToView(QString("[send broadcast]\t: roomID=%1, message=%2, messageID=%3 errorCode=%4").arg(roomID.c_str()).arg(message.c_str()).arg(messageID).arg(errorCode));
    });
}

void ZegoMultiRoomDemo::on_pushButton_startPublish_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->startPublishingStream(streamID);

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);
}

void ZegoMultiRoomDemo::on_pushButton_stopPublish_clicked()
{
    engine->stopPreview();
    engine->stopPublishingStream();
}

void ZegoMultiRoomDemo::on_pushButton_startPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    ZegoCanvas canvas(ZegoView(ui->frame_remote_view->winId()));
    engine->startPlayingStream(streamID, &canvas);
}

void ZegoMultiRoomDemo::on_pushButton_stopPlay_clicked()
{
    std::string streamID = ui->lineEdit_streamID->text().toStdString();
    engine->stopPlayingStream(streamID);
}

void ZegoMultiRoomDemo::on_pushButton_switchRoom_clicked()
{
    std::string fromRoomID = ui->lineEdit_fromRoom->text().toStdString();
    std::string toRoomID = ui->lineEdit_toRoom->text().toStdString();
    engine->switchRoom(fromRoomID, toRoomID);
}
