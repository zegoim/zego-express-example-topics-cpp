#ifndef ZEGOMULTILAYERDEMO_H
#define ZEGOMULTILAYERDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoMultiLayerDemo;
}

class ZegoMultiLayerDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoMultiLayerDemo(QWidget *parent = nullptr);
    ~ZegoMultiLayerDemo();

private slots:
    void on_pushButton_start_publish_clicked();
    void on_pushButton_stop_publish_clicked();

    void on_pushButton_start_play_clicked();
    void on_pushButton_stop_play_clicked();

    void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality &quality);
    void onPlayerVideoSizeChanged(const std::string &streamID, int width, int height);

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoMultiLayerDemo *ui;

    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOMULTILAYERDEMO_H
