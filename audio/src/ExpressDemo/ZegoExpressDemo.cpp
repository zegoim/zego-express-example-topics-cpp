#include "ZegoExpressDemo.h"
#include "ui_ZegoExpressDemo.h"
#include "ZegoExpressSDK.h"
#include <string>
#include <iostream>
#include <QuickStart/ZegoQuickStartDemo.h>
#include <SoundLevel/ZegoSoundLevelDemo.h>

static QString ItemTextQuickStart("QuickStart");
static QString ItemTextSoundLevel("SoundLevel");

ZegoExpressDemo::ZegoExpressDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoExpressDemo)
{
    ui->setupUi(this);

    // show SDK version
    ui->label_sdk_version->setText(QString::fromStdString(ZEGO::EXPRESS::ZegoExpressSDK::getVersion()));

    // ================================================ Basic use case ==========================================
    QStringList basicUseCaseItems;
    basicUseCaseItems.append(ItemTextQuickStart);

    ui->listWidget_basicMenu->addItems(basicUseCaseItems);

    // ================================================Advance use case ==========================================
    QStringList advanceUseCaseItems;
    advanceUseCaseItems.append(ItemTextSoundLevel);
    ui->listWidget_advance_menu->addItems(advanceUseCaseItems);

    connect(ui->listWidget_advance_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onAdvanceUseCaseItemChanged);
    connect(ui->listWidget_basicMenu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onBasicUseCaseItemChanged);
}

ZegoExpressDemo::~ZegoExpressDemo()
{
    delete ui;
}

void ZegoExpressDemo::onAdvanceUseCaseItemChanged(QListWidgetItem *item)
{
    doChangeTopic(item->text());
}

void ZegoExpressDemo::onBasicUseCaseItemChanged(QListWidgetItem *item)
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

    if(currentItemText == ItemTextSoundLevel){
        currentTopicWidget = new ZegoSoundLevelDemo;
    }

    if(currentTopicWidget != nullptr){
        ui->stackedWidget_panel->addWidget(currentTopicWidget);
        ui->stackedWidget_panel->setCurrentWidget(currentTopicWidget);
    }
}


void ZegoExpressDemo::on_pushButton_ReadDocument_clicked()
{


}
