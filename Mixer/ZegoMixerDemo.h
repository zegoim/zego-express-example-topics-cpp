#ifndef ZEGOMIXERDEMO_H
#define ZEGOMIXERDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;


namespace Ui {
class ZegoMixerDemo;
}

class ZegoMixerDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoMixerDemo(QWidget *parent = nullptr);
    ~ZegoMixerDemo();

public slots:
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errCode);
    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList);


private slots:
    void on_pushButton_start_mixer_task_clicked();
    void on_pushButton_stop_mixer_task_clicked();
    void on_pushButton_start_play_clicked();
    void on_pushButton_stop_play_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoMixerDemo *ui;
    std::vector<ZegoStream> zegoStreamList;
    static std::string mixerTaskID;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOMIXERDEMO_H
