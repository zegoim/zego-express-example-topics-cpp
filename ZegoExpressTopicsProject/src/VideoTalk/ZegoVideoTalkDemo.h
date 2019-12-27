#ifndef ZEGOVIDEOCOMMUNICATIONDEMO_H
#define ZEGOVIDEOCOMMUNICATIONDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoVideoTalkDemo;
}

class ZegoVideoTalkDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoVideoTalkDemo(QWidget *parent = nullptr);
    ~ZegoVideoTalkDemo();

public slots:
     void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_logout_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoVideoTalkDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;

    std::vector<ZegoStream> zegoStreamList;
    QList<QWidget*> videoList;
    std::string roomID;
    std::string userID;
};

#endif // ZEGOVIDEOCOMMUNICATIONDEMO_H
