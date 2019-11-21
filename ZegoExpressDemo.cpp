#include "ZegoExpressDemo.h"
#include "ui_zegoexpressdemo.h"

#include "ZegoExpressEngine.h"
#include <string>
#include <iostream>

#include "QuickStart/ZegoQuickStartDemo.h"
#include "Publish/ZegoPublishDemo.h"
#include "Play/ZegoPlayDemo.h"
#include "VideoCommunication/ZegoVideoCommunicationDemo.h"
#include "Mixer/ZegoMixerDemo.h"
#include "ApiMoncky/ZegoApiMonckyDemo.h"


static QString ItemTextQuickStart("QuickStart");
static QString ItemTextPublishStream("PublishStream");
static QString ItemTextPlayStream("PlayStream");
static QString ItemTextVideoCommunication("VideoCommunication");
static QString ItemTextMixerStream("MixerStream");
static QString ItemTextApiMoncky("ApiMoncky");


ZegoExpressDemo::ZegoExpressDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoExpressDemo)
{
    ui->setupUi(this);

    // show SDK version
    ui->label_sdk_version->setText(QString::fromStdString(ZegoExpressEngine::getVersion()));

    // ================================================ Basic use case ==========================================
    QStringList basicUseCaseItems;
    basicUseCaseItems.append(ItemTextQuickStart);
    basicUseCaseItems.append(ItemTextPublishStream);
    basicUseCaseItems.append(ItemTextPlayStream);
    ui->listWidget_basic_menu->addItems(basicUseCaseItems);

    // ================================================Advance use case ==========================================
    QStringList advanceUseCaseItems;
    advanceUseCaseItems.append(ItemTextVideoCommunication);
    advanceUseCaseItems.append(ItemTextMixerStream);
    advanceUseCaseItems.append(ItemTextApiMoncky);
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

    if(currentItemText==ItemTextVideoCommunication){
        currentTopicWidget = new ZegoVideoCommunicationDemo;
    }

    if(currentItemText==ItemTextMixerStream){
        currentTopicWidget = new ZegoMixerDemo;
    }

    if(currentItemText==ItemTextApiMoncky){
        currentTopicWidget = new ZegoApiMonckyDemo;
    }

    if(currentTopicWidget != nullptr){
        ui->stackedWidget_panel->addWidget(currentTopicWidget);
        ui->stackedWidget_panel->setCurrentWidget(currentTopicWidget);
    }
}


