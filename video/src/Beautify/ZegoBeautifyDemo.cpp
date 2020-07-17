#include "ZegoBeautifyDemo.h"
#include "ui_ZegoBeautifyDemo.h"

#include "AppSupport/ZegoUtilHelper.h"
#include "EventHandler/ZegoEventHandlerWithLogger.h"

#include <QFileDialog>

ZegoBeautifyDemo::ZegoBeautifyDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoBeautifyDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "BeautifyRoom-1";
    userID = ZegoUtilHelper::getRandomString();
    ui->pushButton_roomID->setText(QString("RoomID: %1").arg(currentRoomID.c_str()));
    ui->pushButton_userID->setText(QString("UserID: %1").arg(userID.c_str()));

    ZegoUser user(userID, userID);
    engine->loginRoom(currentRoomID, user);

    ZegoCanvas canvas((void*)ui->frame_local_video->winId());
    engine->startPreview(&canvas);
    engine->startPublishingStream(userID);

#ifndef EnableBeautify
    ui->groupBox_beautify->hide();
#endif
}

ZegoBeautifyDemo::~ZegoBeautifyDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    delete ui;
}

void ZegoBeautifyDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoBeautifyDemo::printLogToView);
    engine->setEventHandler(eventHandler);
}

void ZegoBeautifyDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoBeautifyDemo::on_pushButton_setWatermark_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select watermark file", ".", "pic (*.png *jpg)");
    if(path.isEmpty()){
        return;
    }
    QString imageURL = QString("file:///%1").arg(path);
    ui->lineEdit_watermark_imageURL->setText(imageURL);

    ZegoWatermark watermark;
    watermark.imageURL = imageURL.toStdString();
    watermark.layout = ZegoRect(0, 0, 100, 200);
    engine->setPublishWatermark(&watermark, true);
}

#ifdef EnableBeautify
void ZegoBeautifyDemo::on_checkBox_polish_clicked()
{
    if(ui->checkBox_polish->isChecked()){
        beautifyFeature = beautifyFeature |  ZEGO_BEAUTIFY_FEATURE_POLISH;
    } else {
        beautifyFeature = beautifyFeature & (~ZEGO_BEAUTIFY_FEATURE_POLISH);
    }
    engine->enableBeautify(beautifyFeature);
}

void ZegoBeautifyDemo::on_checkBox_whiten_clicked()
{
    if(ui->checkBox_polish->isChecked()){
        beautifyFeature = beautifyFeature |  ZEGO_BEAUTIFY_FEATURE_WHITEN;
    } else {
        beautifyFeature = beautifyFeature & (~ZEGO_BEAUTIFY_FEATURE_WHITEN);
    }
    engine->enableBeautify(beautifyFeature);
}

void ZegoBeautifyDemo::on_checkBox_sharpen_clicked()
{
    if(ui->checkBox_polish->isChecked()){
        beautifyFeature = beautifyFeature |  ZEGO_BEAUTIFY_FEATURE_SHARPEN;
    } else {
        beautifyFeature = beautifyFeature & (~ZEGO_BEAUTIFY_FEATURE_SHARPEN);
    }
    engine->enableBeautify(beautifyFeature);
}


void ZegoBeautifyDemo::on_horizontalSlider_polishFactor_valueChanged(int value)
{
    beatifyOption.polishFactor = float(value / 100.0 * 16.0);
    engine->setBeautifyOption(beatifyOption);
}

void ZegoBeautifyDemo::on_horizontalSlider_whitenFactor_valueChanged(int value)
{
    beatifyOption.whitenFactor = float(value / 100.0 * 1.0);
    engine->setBeautifyOption(beatifyOption);
}

void ZegoBeautifyDemo::on_horizontalSlider_sharpenFactor_valueChanged(int value)
{
    beatifyOption.sharpenFactor = float(value / 100.0 * 2.0);
    engine->setBeautifyOption(beatifyOption);
}

#endif
