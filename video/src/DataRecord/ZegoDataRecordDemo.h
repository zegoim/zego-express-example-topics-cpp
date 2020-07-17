#ifndef ZEGODATARECORDDEMO_H
#define ZEGODATARECORDDEMO_H

#include <QWidget>

#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoDataRecordDemo;
}

class IDataRecordCallback {
public:
    virtual ~IDataRecordCallback(){}
    virtual void onCapturedDataRecordStateUpdate(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel) = 0;
    virtual void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel) =0;
};

class MyDataRecordEventHandler: public IZegoDataRecordEventHandler{
public:
    MyDataRecordEventHandler(IDataRecordCallback* callback):callback(callback){}

    void onCapturedDataRecordStateUpdate(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel) override{
        callback->onCapturedDataRecordStateUpdate(state, errorCode, config, channel);
    }

    void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel) override
    {
        callback->onCapturedDataRecordProgressUpdate(progress, config, channel);
    }

private:
    IDataRecordCallback* callback = nullptr;
};

class ZegoDataRecordDemo : public QWidget, public IDataRecordCallback
{
    Q_OBJECT

public:
    explicit ZegoDataRecordDemo(QWidget *parent = nullptr);
    ~ZegoDataRecordDemo() override;

    void onCapturedDataRecordStateUpdate(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel) override;
    void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel) override;

private slots:
    void on_pushButton_startRecord_clicked();
    void on_pushButton_stopRecord_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoDataRecordDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGODATARECORDDEMO_H
