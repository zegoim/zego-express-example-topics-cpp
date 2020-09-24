#ifndef ZEGOSOUNDEFFECTSDEMO_H
#define ZEGOSOUNDEFFECTSDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoSoundEffectsDemo;
}

class ZegoSoundEffectsDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoSoundEffectsDemo(QWidget *parent = nullptr);
    ~ZegoSoundEffectsDemo();

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_setReverbParam_clicked();
    void on_pushButton_setVoiceChangerParam_clicked();
    void on_checkBox_enableVirtualStereo_clicked();

    void on_horizontalSlider_virtualStereoAngle_valueChanged(int value);

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoSoundEffectsDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOSOUNDEFFECTSDEMO_H
