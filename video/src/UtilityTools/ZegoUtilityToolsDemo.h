#ifndef ZEGOPERFORMANCEMONITORDEMO_H
#define ZEGOPERFORMANCEMONITORDEMO_H

#include <QWidget>

#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoUtilityToolsDemo;
}

class ZegoUtilityToolsDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoUtilityToolsDemo(QWidget *parent = nullptr);
    ~ZegoUtilityToolsDemo();

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();

    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_startPerformaceMonitor_clicked();
    void on_pushButton_stopPerformaceMonitor_clicked();

    void onPerformanceStatusUpdate(const ZegoPerformanceStatus& status);

    void on_pushButton_startNetworkSpeedTest_clicked();
    void on_pushButton_stopNetworkSpeedTest_clicked();

    void onNetworkSpeedTestError(int errorCode, ZegoNetworkSpeedTestType type);
    void onNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality& quality, ZegoNetworkSpeedTestType type);

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoUtilityToolsDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOPERFORMANCEMONITORDEMO_H
