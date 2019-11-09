#ifndef ZEGOVIDEOCOMMUNICATIONDEMO_H
#define ZEGOVIDEOCOMMUNICATIONDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoVideoCommunicationDemo;
}

class ZegoVideoCommunicationDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoVideoCommunicationDemo(QWidget *parent = nullptr);
    ~ZegoVideoCommunicationDemo();

public slots:
     void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
     void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
     void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
     void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
     void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errCode);
     void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errCode);

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_logout_clicked();
    void on_pushButton_clear_log_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoVideoCommunicationDemo *ui;
    std::vector<ZegoStream> zegoStreamList;
    std::shared_ptr<IZegoEventHandler> eventHandler;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;
};

#endif // ZEGOVIDEOCOMMUNICATIONDEMO_H
