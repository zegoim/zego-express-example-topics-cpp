#ifndef ZEGOQUICKSTARTDEMO_H
#define ZEGOQUICKSTARTDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
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

private slots:
    void on_pushButton_createEngine_clicked();
    void on_pushButton_loginRoom_clicked();
    void on_pushButton_PublishStream_clicked();
    void on_pushButton_PlayStream_clicked();
    void on_pushButton_destroyEngine_clicked();

private:
    void printLogToView(QString log);

private:
    Ui::ZegoQuickStartDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
};

#endif // ZEGOQUICKSTARTDEMO_H
