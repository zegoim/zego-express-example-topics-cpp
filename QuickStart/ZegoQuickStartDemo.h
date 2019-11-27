#ifndef ZEGOQUICKSTARTDEMO_H
#define ZEGOQUICKSTARTDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoQuickStartDemo;
}

class ZegoQuickStartDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoQuickStartDemo(QWidget *parent = nullptr);
    ~ZegoQuickStartDemo();

public slots:
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState roomState, int errorCode);
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errCode);
    void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errCode);

private slots:
    void on_pushButton_createEngine_clicked();
    void on_pushButton_loginRoom_clicked();
    void on_pushButton_PublishStream_clicked();
    void on_pushButton_PlayStream_clicked();
    void on_pushButton_destroyEngine_clicked();

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoQuickStartDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
};

#endif // ZEGOQUICKSTARTDEMO_H
