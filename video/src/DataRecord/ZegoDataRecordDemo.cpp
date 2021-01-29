#include "ZegoDataRecordDemo.h"
#include "ui_ZegoDataRecordDemo.h"

#include "EventHandler/ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

#include <QFileDialog>

ZegoDataRecordDemo::ZegoDataRecordDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoDataRecordDemo)
{
    ui->setupUi(this);

    engine = ZegoExpressSDK::getEngine();
    bindEventHandler();

    currentRoomID = "MediaPlayerRoom-1";
    std::string userID = ZegoUtilHelper::getRandomString();
    engine->loginRoom(currentRoomID, ZegoUser(userID, userID));

    ZegoCanvas canvas(ZegoView(ui->frame_local_preview->winId()));
    engine->startPreview(&canvas);


    QStringList dataRecordTypeList = {
        "ZEGO_DATA_RECORD_TYPE_DEFAULT",
        "ZEGO_DATA_RECORD_TYPE_ONLY_AUDIO",
        "ZEGO_DATA_RECORD_TYPE_ONLY_VIDEO",
        "ZEGO_DATA_RECORD_TYPE_AUDIO_AND_VIDEO"
    };
    ui->comboBox_dataRecordType->addItems(dataRecordTypeList);
}

ZegoDataRecordDemo::~ZegoDataRecordDemo()
{
    engine->logoutRoom(currentRoomID);
    engine->setEventHandler(nullptr);
    engine->setDataRecordEventHandler(nullptr);
    delete ui;
}

void ZegoDataRecordDemo::onCapturedDataRecordStateUpdate(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel)
{
    Q_UNUSED(channel);
    QStringList stateList = {
        "ZEGO_DATA_RECORD_STATE_NO_RECORD",
        "ZEGO_DATA_RECORD_STATE_RECORDING",
        "ZEGO_DATA_RECORD_STATE_SUCCESS"
    };
    QString log = QString("onCapturedDataRecordStateUpdate: state=%1, errorCode=%2, filePath=%3").arg(stateList.value(state)).arg(errorCode).arg(config.filePath.c_str());
    printLogToView(log);
}

void ZegoDataRecordDemo::onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel)
{
    Q_UNUSED(channel);
    Q_UNUSED(config);
    ui->label_duration->setText(QString::number(progress.duration/1000));
    ui->label_fileSize->setText(QString::number(progress.currentFileSize/1000));
}

void ZegoDataRecordDemo::on_pushButton_startRecord_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Select file to save", ".", "record file (*.mp4 *.flv *.aac)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEdit_recordFile->setText(path);

    ZegoDataRecordConfig recordConfig;
    recordConfig.filePath = path.toStdString();
    recordConfig.recordType = ZegoDataRecordType(ui->comboBox_dataRecordType->currentIndex());

    QString log = QString("start record: filePath=%1, recordType=%2").arg(path).arg(ui->comboBox_dataRecordType->currentText());
    printLogToView(log);
    engine->startRecordingCapturedData(recordConfig);
}

void ZegoDataRecordDemo::on_pushButton_stopRecord_clicked()
{
    engine->stopRecordingCapturedData();
}

void ZegoDataRecordDemo::printLogToView(const QString &log)
{
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->textEdit_log->append(QString("[ %1 ] %2").arg(time).arg(log));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void ZegoDataRecordDemo::bindEventHandler()
{
    auto eventHandler = std::make_shared<ZegoEventHandlerWithLogger>();
    engine->setEventHandler(eventHandler);
    connect(eventHandler.get(), &ZegoEventHandlerWithLogger::sigPrintLogToView, this, &ZegoDataRecordDemo::printLogToView);

    engine->setDataRecordEventHandler(std::make_shared<MyDataRecordEventHandler>(this));
}
