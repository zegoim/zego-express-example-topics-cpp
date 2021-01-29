#ifndef ZEGOEXTERNALVIDEOCAPTUREDEMO_H
#define ZEGOEXTERNALVIDEOCAPTUREDEMO_H

#include <QWidget>
#include <atomic>

#include "ZegoCustomVideoSourceContext.h"
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoCustomVideoCaptureDemo;
}


class CustomVideoCapturer: public IZegoCustomVideoCaptureHandler, public ZegoCustomVideoSourceContext{
public:
    CustomVideoCapturer(ZegoVideoBufferType bufferType, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN);
    ~CustomVideoCapturer() override;
    void onStart(ZegoPublishChannel channel) override;
    void onStop(ZegoPublishChannel channel) override;

private:
    void collectVideoFrameAndSendToEngine();

private:
    std::atomic<bool> mVideoCaptureRunning = {false};
    std::thread mVideoCaptureThread;
    ZegoVideoBufferType mVideoBufferType;
    ZegoPublishChannel mPublishChannel;
};

class ZegoCustomVideoCaptureDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoCustomVideoCaptureDemo(QWidget *parent = nullptr);
    ~ZegoCustomVideoCaptureDemo();

private slots:
    void on_pushButton_enableCustomVideoCapture_clicked();
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();
    void on_pushButton_imageSource_clicked();
    void on_pushButton_mediaSource_clicked();

private:
    void bindEventHandler();
    void printLogToView(const QString &log);

    Ui::ZegoCustomVideoCaptureDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::shared_ptr<CustomVideoCapturer> mCustomVideoCapture = nullptr;

    std::string currentRoomID;
    std::string userID;
};

#endif // ZEGOEXTERNALVIDEOCAPTUREDEMO_H
