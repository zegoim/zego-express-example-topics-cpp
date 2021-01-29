#include "ZegoExpressDemo.h"
#include "ui_ZegoExpressDemo.h"

#include <QDesktopServices>

#include "ZegoExpressSDK.h"
#include "AppSupport/ZegoConfigManager.h"

#include "QuickStart/ZegoQuickStartDemo.h"
#include "Publish/ZegoPublishDemo.h"
#include "Play/ZegoPlayDemo.h"
#include "VideoTalk/ZegoVideoTalkDemo.h"
#include "Mixer/ZegoMixerDemo.h"
#include "RoomMessage/ZegoRoomMessageDemo.h"
#include "SoundLevel/ZegoSoundLevelDemo.h"
#include "SoundEffects/ZegoSoundEffectsDemo.h"
#include "Beautify/ZegoBeautifyDemo.h"
#include "MediaPlayer/ZegoMediaPlayerDemo.h"
#include "AudioEffectPlayer/ZegoAudioEffectPlayerDemo.h"
#include "CustomVideoRender/ZegoCustomVideoRenderDemo.h"
#include "CustomVideoCapture/ZegoCustomVideoCaptureDemo.h"
#include "CustomAudioIO/ZegoCustomAudioIODemo.h"
#include "CDNAbout/ZegoCDNAboutDemo.h"
#include "AuxStream/ZegoAuxStreamDemo.h"
#include "AudioMixing/ZegoAudioMixingDemo.h"
#include "AudioProcess/ZegoAudioProcessDemo.h"
#include "AudioDataCallback/ZegoAudioDataCallbackDemo.h"
#include "DataRecord/ZegoDataRecordDemo.h"
#include "MultiLayer/ZegoMultiLayerDemo.h"
#include "MultiRoom/ZegoMultiRoomDemo.h"
#include "UtilityTools/ZegoUtilityToolsDemo.h"

#include "version.h"

static QString ItemTextQuickStart("QuickStart");
static QString ItemTextPublishStream("PublishStream");
static QString ItemTextPlayStream("PlayStream");

static QString ItemTextVideoTalk("VideoTalk");
static QString ItemTextRoomMessage("RoomMessage");
static QString ItemTextCDNAbout("CDNAbout");
static QString ItemTextMixerStream("MixerStream");
static QString ItemTextSoundLevel("SoundLevel");
static QString ItemTextSoundEffects("SoundEffects");
static QString ItemTextBeautify("Beautify");
static QString ItemTextMediaPlayer("MediaPlayer");
static QString ItemTextAudioEffectPlayer("AudioEffectPlayer");
static QString ItemTextCustomVideoRender("CustomVideoRender");
static QString ItemTextCustomVideoCapture("CustomVideoCapture");
static QString ItemTextCustomAudioIO("CustomAudioIO");
static QString ItemTextAuxStream("AuxStream");
static QString ItemTextAudioMixing("AudioMixing");
static QString ItemTextAudioProcess("AudioProcess");
static QString ItemTextAudioDataCallback("AudioDataCallback");
static QString ItemTextDataRecord("DataRecord");
static QString ItemTextMultiLayer("MultiLayer");
static QString ItemTextMultiRoom("MultiRoom");
static QString ItemTextUtilityTools("UtilityTools");

using namespace ZEGO::EXPRESS;

ZegoExpressDemo::ZegoExpressDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZegoExpressDemo)
{
    ui->setupUi(this);

    // show SDK version
    ui->label_demo_version->setText(QString::fromStdString(DEMO_VERSION));
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
    advanceUseCaseItems.append(ItemTextRoomMessage);
    advanceUseCaseItems.append(ItemTextCDNAbout);
    advanceUseCaseItems.append(ItemTextMixerStream);
    advanceUseCaseItems.append(ItemTextSoundLevel);
    advanceUseCaseItems.append(ItemTextSoundEffects);
    advanceUseCaseItems.append(ItemTextBeautify);
    advanceUseCaseItems.append(ItemTextMediaPlayer);
    advanceUseCaseItems.append(ItemTextAudioEffectPlayer);
    advanceUseCaseItems.append(ItemTextCustomVideoRender);
    advanceUseCaseItems.append(ItemTextCustomVideoCapture);
    advanceUseCaseItems.append(ItemTextCustomAudioIO);
    advanceUseCaseItems.append(ItemTextAuxStream);
    advanceUseCaseItems.append(ItemTextAudioMixing);
    advanceUseCaseItems.append(ItemTextAudioProcess);
    advanceUseCaseItems.append(ItemTextAudioDataCallback);
    advanceUseCaseItems.append(ItemTextDataRecord);
    advanceUseCaseItems.append(ItemTextMultiLayer);
    advanceUseCaseItems.append(ItemTextMultiRoom);
    advanceUseCaseItems.append(ItemTextUtilityTools);
    ui->listWidget_advance_menu->addItems(advanceUseCaseItems);

    connect(ui->listWidget_basic_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onBasicUseCaseItemChanged);
    connect(ui->listWidget_advance_menu, &QListWidget::itemClicked, this, &ZegoExpressDemo::onAdvanceUseCaseItemChanged);

    // Create Engine
    {
        auto appID = ZegoConfigManager::instance()->getAppID();
        auto appSign = ZegoConfigManager::instance()->getAppSign();
        auto isTest = ZegoConfigManager::instance()->isTestEnviroment();
        auto engine = ZegoExpressSDK::createEngine(appID, appSign, isTest, ZEGO_SCENARIO_GENERAL, nullptr);
        Q_ASSERT(engine != nullptr);
    }
}

ZegoExpressDemo::~ZegoExpressDemo()
{
    // Destroy current Topic
    if(currentTopicWidget!= nullptr){
        ui->stackedWidget_panel->removeWidget(currentTopicWidget);
        delete currentTopicWidget;
        currentTopicWidget = nullptr;
    }

    // Destroy Engine
    {
        auto engine = ZegoExpressSDK::getEngine();
        if(engine){
            ZegoExpressSDK::destroyEngine(engine);
        }
    }
    delete ui;
}

void ZegoExpressDemo::doChangeTopic(QString itemText)
{
    if(currentItemText == itemText){
        return;
    }
    currentItemText = itemText;

    // Destroy Old Topic
    if(currentTopicWidget!= nullptr){
        ui->stackedWidget_panel->removeWidget(currentTopicWidget);
        delete currentTopicWidget;
        currentTopicWidget = nullptr;
    }

    // Create New Topic
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

    if(currentItemText==ItemTextRoomMessage){
        currentTopicWidget = new ZegoRoomMessageDemo;
    }

    if(currentItemText == ItemTextCDNAbout){
        currentTopicWidget = new ZegoCDNAboutDemo;
    }

    if(currentItemText==ItemTextMixerStream){
        currentTopicWidget = new ZegoMixerDemo;
    }

    if(currentItemText == ItemTextSoundLevel){
        currentTopicWidget = new ZegoSoundLevelDemo;
    }

    if(currentItemText == ItemTextSoundEffects){
        currentTopicWidget = new ZegoSoundEffectsDemo;
    }

    if(currentItemText == ItemTextBeautify) {
        currentTopicWidget = new ZegoBeautifyDemo;
    }

    if(currentItemText == ItemTextMediaPlayer){
        currentTopicWidget = new ZegoMediaPlayerDemo;
    }

    if(currentItemText == ItemTextAudioEffectPlayer){
        currentTopicWidget = new ZegoAudioEffectPlayerDemo;
    }

    if(currentItemText == ItemTextCustomVideoRender){
        currentTopicWidget = new ZegoCustomVideoRenderDemo;
    }

    if(currentItemText == ItemTextCustomVideoCapture){
        currentTopicWidget = new ZegoCustomVideoCaptureDemo;
    }

    if(currentItemText == ItemTextCustomAudioIO){
        currentTopicWidget = new ZegoCustomAudioIODemo;
    }

    if(currentItemText == ItemTextAuxStream){
        currentTopicWidget = new ZegoAuxStreamDemo;
    }

    if(currentItemText == ItemTextAudioMixing){
        currentTopicWidget = new ZegoAudioMixingDemo;
    }

    if(currentItemText == ItemTextAudioProcess){
        currentTopicWidget = new ZegoAudioProcessDemo;
    }

    if(currentItemText == ItemTextAudioDataCallback){
        currentTopicWidget = new ZegoAudioDataCallbackDemo;
    }

    if(currentItemText == ItemTextDataRecord){
        currentTopicWidget = new ZegoDataRecordDemo;
    }

    if(currentItemText == ItemTextMultiLayer){
        currentTopicWidget = new ZegoMultiLayerDemo;
    }

    if(currentItemText == ItemTextMultiRoom) {
        currentTopicWidget = new ZegoMultiRoomDemo;
    }

    if(currentItemText == ItemTextUtilityTools){
        currentTopicWidget = new ZegoUtilityToolsDemo;
    }

    if(currentTopicWidget != nullptr){
        ui->stackedWidget_panel->addWidget(currentTopicWidget);
        ui->stackedWidget_panel->setCurrentWidget(currentTopicWidget);
    }
}

void ZegoExpressDemo::onBasicUseCaseItemChanged(QListWidgetItem *item)
{
    doChangeTopic(item->text());
}

void ZegoExpressDemo::onAdvanceUseCaseItemChanged(QListWidgetItem *item)
{
    doChangeTopic(item->text());
}

void ZegoExpressDemo::on_pushButton_go_code_clicked()
{
    QDesktopServices::openUrl(QUrl("https://doc-zh.zego.im/zh/3128.html"));
    QDesktopServices::openUrl(QUrl("https://storage.zego.im/express/example/cpp/ZegoExpressExample-CPP.zip"));
}

void ZegoExpressDemo::on_pushButton_go_document_clicked()
{
    QDesktopServices::openUrl(QUrl("https://doc-zh.zego.im/zh/693.html"));
}
