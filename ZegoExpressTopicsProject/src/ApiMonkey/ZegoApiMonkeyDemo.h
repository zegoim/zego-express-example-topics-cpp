#ifndef ZEGOAPIMONCKYDEMO_H
#define ZEGOAPIMONCKYDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoApiMonkeyDemo;
}

class ZegoApiMonkeyDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoApiMonkeyDemo(QWidget *parent = nullptr);
    ~ZegoApiMonkeyDemo();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_logout_clicked();

    void on_pushButton_start_publish_clicked();
    void on_pushButton_stop_publish_clicked();
    void on_pushButton_setExtraInfo_clicked();

    void on_pushButton_start_play_clicked();
    void on_pushButton_stop_play_clicked();

    void on_pushButton_start_mixer_task_clicked();
    void on_pushButton_stop_mixer_task_clicked();

    void on_pushButton_sendBroadcastMessage_clicked();
    void on_pushButton_sendCustomCommand_clicked();

    void on_pushButton_sendSEI_clicked();

private slots:
    void onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength);


private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoApiMonkeyDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
};

#endif // ZEGOAPIMONCKYDEMO_H
