#ifndef ZEGOBEAUTIFYDEMO_H
#define ZEGOBEAUTIFYDEMO_H

#include <QWidget>
#include "ZegoExpressEngine.h"
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

private:
    Ui::ZegoBeautifyDemo *ui;
    ZEGO::EXPRESS::IZegoExpressEngine *engine;

    int beautifyFeature = ZEGO_BEAUTIFY_FEATURE_NONE;
    ZegoBeautifyOption beatifyOption;
    std::string roomID;
    std::string userID;
};

#endif // ZEGOBEAUTIFYDEMO_H
