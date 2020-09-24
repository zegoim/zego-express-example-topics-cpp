#ifndef ZEGOAUDIOEFFECTPLAYERDEMO_H
#define ZEGOAUDIOEFFECTPLAYERDEMO_H

#include <QWidget>

#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoAudioEffectPlayerDemo;
}

class ZegoAudioEffectPlayerDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoAudioEffectPlayerDemo(QWidget *parent = nullptr);
    ~ZegoAudioEffectPlayerDemo();

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();

    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_startPlayImmediately_clicked();

    void on_pushButton_loadResource_1_clicked();
    void on_pushButton_unloadResource_1_clicked();
    void on_pushButton_start_1_clicked();
    void on_pushButton_stop_1_clicked();
    void on_pushButton_pause_1_clicked();
    void on_pushButton_resume_1_clicked();

    void on_pushButton_stopAll_clicked();
    void on_pushButton_pauseAll_clicked();
    void on_pushButton_ResumeAll_clicked();

    void on_pushButton_loadResource_2_clicked();
    void on_pushButton_unloadResource_2_clicked();
    void on_pushButton_start_2_clicked();
    void on_pushButton_stop_2_clicked();
    void on_pushButton_pause_2_clicked();
    void on_pushButton_resume_2_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();
    unsigned int getUniqueAudioEffectID();

private:
    Ui::ZegoAudioEffectPlayerDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;

    IZegoAudioEffectPlayer *audioEffectPlayer = nullptr;
    unsigned int audioEffectIDFirst = 0;
    unsigned int audioEffectIDSecond = 0;
};

#endif // ZEGOAUDIOEFFECTPLAYERDEMO_H
