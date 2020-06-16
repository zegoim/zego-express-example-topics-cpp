#ifndef ZEGOAUXSTREAMDEMO_H
#define ZEGOAUXSTREAMDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoAuxStreamDemo;
}

class ZegoAuxStreamDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoAuxStreamDemo(QWidget *parent = nullptr);
    ~ZegoAuxStreamDemo();

private slots:
    void on_pushButton_startPublish_1_clicked();
    void on_pushButton_stopPublish_1_clicked();
    void on_pushButton_startPublish_2_clicked();
    void on_pushButton_stopPublish_2_clicked();

    void on_comboBox_mirrormode_currentIndexChanged(int index);

    void on_comboBox_videoConfig_currentIndexChanged(int index);

    void on_checkBox_mutePublishStreamVideo_clicked(bool checked);

    void on_checkBox_mutePublishStreamAudio_clicked(bool checked);

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoAuxStreamDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
    std::string userID;
};

#endif // ZEGOAUXSTREAMDEMO_H
