#include "ZegoExpressDemo.h"
#include "ui_ZegoExpressDemo.h"

#include "ZegoExpressSDK.h"
#include <string>
#include <iostream>

#include "QuickStart/ZegoQuickStartDemo.h"
#include "Publish/ZegoPublishDemo.h"
#include "Play/ZegoPlayDemo.h"
#include "VideoTalk/ZegoVideoTalkDemo.h"
#include "Mixer/ZegoMixerDemo.h"
#include "RoomMessage/ZegoRoomMessageDemo.h"
#include "SoundLevel/ZegoSoundLevelDemo.h"
#include "Beautify/ZegoBeautifyDemo.h"
#include "MediaPlayer/ZegoMediaPlayerDemo.h"
#include "ApiMonkey/ZegoApiMonkeyDemo.h"


static QString ItemTextQuickStart("QuickStart");
static QString ItemTextPublishStream("PublishStream");
static QString ItemTextPlayStream("PlayStream");
static QString ItemTextVideoTalk("VideoTalk");
static QString ItemTextMixerStream("MixerStream");
static QString ItemTextRoomMessage("RoomMessage");
static QString ItemTextSoundLevel("SoundLevel");
static QString ItemTextBeautify("Beautify");
static QString ItemTextMediaPlayer("MediaPlayer");
static QString ItemTextApiMonkey("ApiMonkey");


ZegoExpressDemo::ZegoExpressDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoExpressDemo)
{
    ui->setupUi(this);

    // show SDK version
    ui->label_sdk_version->setText(QString::fromStdString(ZegoExpressSDK::getVersion()));

    // ================================================ Basic use case ==========================================
    QStringList basicUseCaseItems;
    basicUseCaseItems.append(ItemTextQuickStart);
    basicUseCaseItems.append(ItemTextPublishStream);
    basicUseCaseItems.append(ItemTextPlayStream);
    ui->listWidget_basic_menu->addItems(basicUseCaseItems);

    // ================================================Advance use case ==========================================
    QStringList advanceUseCaseItems;
    advanceUseCaseItems.append(ItemTextVideoTalk);
    advanceUseCaseItems.append(ItemTextMixerStream);
    advanceUseCaseItems.append(ItemTextRoomMessage);
    advanceUseCaseItems.append(ItemTextSoundLevel);
    advanceUseCaseItems.append(ItemTextBeautify);
    advanceUseCaseItems.append(ItemTextMediaPlayer);
    advanceUseCaseItems.append(ItemTextApiMonkey);
    ui->listWidget_advance_menu->addItems(advanceUseCaseItems);

    connect(ui->listWidget_basic_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onBasicUseCaseItemChanged);
    connect(ui->listWidget_advance_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onAdvanceUseCaseItemChanged);
}

ZegoExpressDemo::~ZegoExpressDemo()
{
    delete ui;
}

void ZegoExpressDemo::onBasicUseCaseItemChanged(QListWidgetItem *item)
{
    doChangeTopic(item->text());
}

void ZegoExpressDemo::onAdvanceUseCaseItemChanged(QListWidgetItem *item)
{
    doChangeTopic(item->text());
}

void ZegoExpressDemo::doChangeTopic(QString itemText)
{
    if(currentItemText == itemText){
        return;
    }

    // Destroy Old Topic
    if(currentTopicWidget!= nullptr){
        ui->stackedWidget_panel->removeWidget(currentTopicWidget);
        delete currentTopicWidget;
        currentTopicWidget = nullptr;
    }


    // Create New Topic
    currentItemText = itemText;
    if(currentItemText==ItemTextQuickStart){
        currentTopicWidget = new ZegoQuickStartDemo;
    }

    if(currentItemText==ItemTextPublishStream){
        currentTopicWidget = new ZegoPublishDemo;
    }

    if(currentItemText==ItemTextPlayStream){
        currentTopicWidget = new ZegoPlayDemo;
    }

    if(currentItemText==ItemTextVideoTalk){
        currentTopicWidget = new ZegoVideoTalkDemo;
    }

    if(currentItemText==ItemTextMixerStream){
        currentTopicWidget = new ZegoMixerDemo;
    }

    if(currentItemText==ItemTextRoomMessage){
        currentTopicWidget = new ZegoRoomMessageDemo;
    }

    if(currentItemText==ItemTextApiMonkey){
        currentTopicWidget = new ZegoApiMonkeyDemo;
    }

    if(currentItemText == ItemTextSoundLevel){
        currentTopicWidget = new ZegoSoundLevelDemo;
    }

    if(currentItemText == ItemTextBeautify) {
        currentTopicWidget = new ZegoBeautifyDemo;
    }

    if(currentItemText == ItemTextMediaPlayer){
        currentTopicWidget = new ZegoMediaPlayerDemo;
    }

    if(currentTopicWidget != nullptr){
        ui->stackedWidget_panel->addWidget(currentTopicWidget);
        ui->stackedWidget_panel->setCurrentWidget(currentTopicWidget);
    }
}


