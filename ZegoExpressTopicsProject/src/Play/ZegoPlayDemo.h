#ifndef ZEGOPLAYDEMO_H
#define ZEGOPLAYDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoPlayDemo;
}

class ZegoPlayDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoPlayDemo(QWidget *parent = nullptr);
    ~ZegoPlayDemo();

public slots:
       void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality &quality);
	   void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height);

private slots:
    void on_comboBox_audioOutputDevice_currentIndexChanged(const QString &arg1);
    void on_comboBox_viewmode_currentIndexChanged(int index);
    void on_checkBox_mutePlayStreamAudio_clicked(bool checked);
    void on_checkBox_mutePlayStreamVideo_clicked(bool checked);
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

private:
    void bindEventHandler();

private:
    Ui::ZegoPlayDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOPLAYDEMO_H