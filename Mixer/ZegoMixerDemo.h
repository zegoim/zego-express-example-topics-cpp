#ifndef ZEGOMIXERDEMO_H
#define ZEGOMIXERDEMO_H

#include <QWidget>

namespace Ui {
class ZegoMixerDemo;
}

class ZegoMixerDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoMixerDemo(QWidget *parent = nullptr);
    ~ZegoMixerDemo();

private:
    Ui::ZegoMixerDemo *ui;
};

#endif // ZEGOMIXERDEMO_H
