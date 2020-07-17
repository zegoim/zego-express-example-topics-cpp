#ifndef ZEGOCDNABOUTDEMO_H
#define ZEGOCDNABOUTDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoCDNAboutDemo;
}

class ZegoCDNAboutDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoCDNAboutDemo(QWidget *parent = nullptr);
    ~ZegoCDNAboutDemo();

private slots:
    void on_pushButton_startPublishDirectToCDN_clicked();
    void on_pushButton_stopPublishDirectToCDN_clicked();

    void on_pushButton_startPublishToZegoServer_clicked();
    void on_pushButton_addPublishToCDN_clicked();
    void on_pushButton_removePublishToCDN_clicked();
    void on_pushButton_stopPublishToZegoServer_clicked();

    void on_pushButton_startPlayFromCDN_clicked();
    void on_pushButton_stopPlayFromCDN_clicked();

private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoCDNAboutDemo *ui;
    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;
    std::string userID;

};

#endif // ZEGOCDNABOUTDEMO_H
