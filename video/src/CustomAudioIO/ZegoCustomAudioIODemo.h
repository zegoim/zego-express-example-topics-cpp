#ifndef ZEGOCUSTOMAUDIOIODEMO_H
#define ZEGOCUSTOMAUDIOIODEMO_H

#include <QWidget>
#include <QAudioOutput>
#include <QAudioInput>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoCustomAudioIODemo;
}



class CustomAudioInputDevice;
class CustomAudioOutputDevice;
class ZegoCustomAudioIODemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoCustomAudioIODemo(QWidget *parent = nullptr);
    ~ZegoCustomAudioIODemo();

private slots:
    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();
    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_enableCustomAudioIO_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

    void startCustomAudioCapture();
    void stopCustomAudioCapture();
    void startCustomAudioRender();
    void stopCustomAudioRender();

private:
    Ui::ZegoCustomAudioIODemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
    std::string userID;
	
    QAudioInput  *mCustomAudioInput = nullptr;
    CustomAudioInputDevice *mCustomAudioInputDevice = nullptr;

    QAudioOutput *mCustomAudioOutput = nullptr;
    CustomAudioOutputDevice *mCustomAudioOutputDevice = nullptr;
};

#endif // ZEGOCUSTOMAUDIOIODEMO_H
