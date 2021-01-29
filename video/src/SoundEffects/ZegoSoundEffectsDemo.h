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

    void on_pushButton_setReverbPreset_clicked();
    void on_pushButton_setVoiceChangerPreset_clicked();
    void on_checkBox_enableVirtualStereo_clicked();
    void on_horizontalSlider_virtualStereoAngle_valueChanged(int value);

    void on_horizontalSlider_roomSize_valueChanged(int value);
    void on_horizontalSlider_reverberance_valueChanged(int value);
    void on_horizontalSlider_damping_valueChanged(int value);
    void on_horizontalSlider_wetGain_valueChanged(int value);
    void on_horizontalSlider_dryGain_valueChanged(int value);
    void on_horizontalSlider_toneLow_valueChanged(int value);
    void on_horizontalSlider_toneHigh_valueChanged(int value);
    void on_horizontalSlider_preDelay_valueChanged(int value);
    void on_horizontalSlider_stereoWidth_valueChanged(int value);

private:
    void printLogToView(const QString &log);
    void bindEventHandler();
    void updateReverbAdvanceParam();

private:
    Ui::ZegoSoundEffectsDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOSOUNDEFFECTSDEMO_H
