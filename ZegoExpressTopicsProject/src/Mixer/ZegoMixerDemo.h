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
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);

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
    std::string roomID;
    std::string userID;
};

#endif // ZEGOMIXERDEMO_H
