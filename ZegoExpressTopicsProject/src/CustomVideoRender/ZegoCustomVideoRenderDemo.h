#ifndef ZEGOCUSTOMVIDEORENDERDEMO_H
#define ZEGOCUSTOMVIDEORENDERDEMO_H

#include <memory>
#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoCustomVideoRenderDemo;
}

class OnRenderDataInterface {
public:
    virtual ~OnRenderDataInterface(){}
    virtual void onCapturedVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode) = 0;
    virtual void onRemoteVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param, std::string streamID) = 0;
};

class CustomVideoRenderer: public IZegoCustomVideoRenderHandler{
public:

    CustomVideoRenderer(OnRenderDataInterface *dataInterface)
        :dataInterface(dataInterface){
    }

    void onCapturedVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param,  ZegoVideoFlipMode flipMode, ZegoPublishChannel channel) override {
        Q_UNUSED(channel)
        dataInterface->onCapturedVideoFrameRawData(data, dataLength, param, flipMode);
    }

    void onRemoteVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param, const std::string& streamID) override{
        dataInterface->onRemoteVideoFrameRawData(data, dataLength, param, streamID);
    }

private:
    OnRenderDataInterface *dataInterface;
};

class ZegoCustomVideoRenderDemo :
        public QWidget,
        public OnRenderDataInterface
{
    Q_OBJECT

public:
    explicit ZegoCustomVideoRenderDemo(QWidget *parent = nullptr);
    ~ZegoCustomVideoRenderDemo() override;

private slots:
    void on_pushButton_setEngineConfig_clicked();
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

public:
    void onCapturedVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param,  ZegoVideoFlipMode flipMode) override;
    void onRemoteVideoFrameRawData(unsigned char ** data, unsigned int* dataLength, ZegoVideoFrameParam param, std::string streamID) override;

private:
    void bindEventHandler();

    Ui::ZegoCustomVideoRenderDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
    std::string roomID;
    std::string userID;
};

#endif // ZEGOCUSTOMVIDEORENDERDEMO_H
