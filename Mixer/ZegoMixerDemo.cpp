#include "ZegoMixerDemo.h"
#include "ui_ZegoMixerDemo.h"

ZegoMixerDemo::ZegoMixerDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoMixerDemo)
{
    ui->setupUi(this);
}

ZegoMixerDemo::~ZegoMixerDemo()
{
    delete ui;
}
