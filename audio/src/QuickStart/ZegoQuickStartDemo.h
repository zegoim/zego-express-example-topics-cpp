#ifndef ZEGOQUICKSTARTDEMO_H
#define ZEGOQUICKSTARTDEMO_H

#include <QWidget>
#include <ZegoExpressInterface.h>
#include <ZegoExpressDefines.h>
#include <ZegoExpressSDK.h>
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

private slots:
    void on_pushButton_loginRoom_clicked();
    void on_pushButton_PublishStream_clicked();
    void on_pushButton_PlayStream_clicked();

private:
    void bindEventHandler();
    void printLogToView(QString log);

private:
    Ui::ZegoQuickStartDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
};

#endif // ZEGOQUICKSTARTDEMO_H
