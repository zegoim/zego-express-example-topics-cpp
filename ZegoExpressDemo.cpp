#include "ZegoExpressDemo.h"
#include "ui_zegoexpressdemo.h"

#include "ZegoExpressEngine.h"
#include <string>
#include <iostream>
#include <QTimer>

#include "AppSupport/ZegoConfigManager.h"


ZegoExpressDemo::ZegoExpressDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoExpressDemo)
{
    ui->setupUi(this);

    // ================================================ Basic use case ==========================================
    QStringList basicUseCaseItems;
    basicUseCaseItems.append("推流");
    basicUseCaseItems.append("拉流");
    ui->listWidget_basic_menu->addItems(basicUseCaseItems);

    // ================================================Advance use case ==========================================
    QStringList advanceUseCaseItems;
    advanceUseCaseItems.append("视频通话");
    //advanceUseCaseItems.append("直播连麦");
    ui->listWidget_advance_menu->addItems(advanceUseCaseItems);


    connect(ui->listWidget_basic_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onBasicUseCaseItemChanged);
    connect(ui->listWidget_advance_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onAdvanceUseCaseItemChanged);

    ui->label_sdk_version->setText(QString::fromStdString(ZegoExpressEngine::getVersion()));
    ui->label_appID->setText(QString::number(ZegoConfigManager::instance()->getAppID()));
    ui->checkBox_testEnv->setChecked(ZegoConfigManager::instance()->isTestEnviroment());
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

    // 删掉老的窗口
    if(currentTopicWidget!= nullptr){
        ui->stackedWidget_panel->removeWidget(currentTopicWidget);
        delete currentTopicWidget;
        currentTopicWidget = nullptr;
    }


    // 重建新窗口
    currentItemText = itemText;
    if(currentItemText=="推流"){
        currentTopicWidget = new ZegoPublishDemo;
    }

    if(currentItemText=="拉流"){
        currentTopicWidget = new ZegoPlayDemo;
    }

    if(currentItemText=="视频通话"){
        currentTopicWidget = new ZegoVideoCommunicationDemo;
    }

    ui->stackedWidget_panel->addWidget(currentTopicWidget);
    ui->stackedWidget_panel->setCurrentWidget(currentTopicWidget);
}


