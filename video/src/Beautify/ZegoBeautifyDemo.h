#ifndef ZEGOBEAUTIFYDEMO_H
#define ZEGOBEAUTIFYDEMO_H

#include <QWidget>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoBeautifyDemo;
}

class ZegoBeautifyDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoBeautifyDemo(QWidget *parent = nullptr);
    ~ZegoBeautifyDemo();

private slots:
    void on_pushButton_setWatermark_clicked();

#ifdef EnableBeautify
    void on_checkBox_polish_clicked();
    void on_checkBox_whiten_clicked();
    void on_checkBox_sharpen_clicked();
    void on_horizontalSlider_polishFactor_valueChanged(int value);
    void on_horizontalSlider_whitenFactor_valueChanged(int value);
    void on_horizontalSlider_sharpenFactor_valueChanged(int value);
#endif

private:
    void bindEventHandler();
    void printLogToView(const QString &log);

private:
    Ui::ZegoBeautifyDemo *ui;
    IZegoExpressEngine *engine = nullptr;

#ifdef EnableBeautify
    int beautifyFeature = ZEGO_BEAUTIFY_FEATURE_NONE;
    ZegoBeautifyOption beatifyOption;
#endif
    std::string currentRoomID;
    std::string userID;
};

#endif // ZEGOBEAUTIFYDEMO_H
