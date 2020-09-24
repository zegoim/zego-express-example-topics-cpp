#ifndef ZEGOMULTIROOMDEMO_H
#define ZEGOMULTIROOMDEMO_H

#include <QWidget>

#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoMultiRoomDemo;
}

class ZegoMultiRoomDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoMultiRoomDemo(QWidget *parent = nullptr);
    ~ZegoMultiRoomDemo();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_logout_clicked();

    void on_pushButton_setRoomExtraInfo_clicked();
    void on_pushButton_sendBroadcastMessage_clicked();

    void on_pushButton_startPublish_clicked();
    void on_pushButton_stopPublish_clicked();

    void on_pushButton_startPlay_clicked();
    void on_pushButton_stopPlay_clicked();

    void on_pushButton_switchRoom_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoMultiRoomDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string mainRoomID;
    std::string auxRoomID;
    std::string userID;
};

#endif // ZEGOMULTIROOMDEMO_H
