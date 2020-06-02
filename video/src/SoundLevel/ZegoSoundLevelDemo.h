#ifndef ZEGOSOUNDLEVELDEMO_H
#define ZEGOSOUNDLEVELDEMO_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>

#include "ZegoExpressSDK.h"
#include "ZegoSoundFrame.h"

using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoSoundLevelDemo;
}

class ZegoSoundLevelDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoSoundLevelDemo(QWidget *parent = nullptr);
    ~ZegoSoundLevelDemo();

public slots:
     void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
     void onCapturedSoundLevelUpdate(float soundLevel);
     void onPlayerSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels);
     void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& audioSpectrum);
     void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum>& audioSpectrums);

private slots:
     void on_checkBox_soundLevelMonitor_clicked();
     void on_checkBox_audioSpectrumMonitor_clicked();

private:
    void bindEventHandler();

private:
    void addSoundFrame(std::string streamID);
    void removeSoundFrame(std::string streamID);

private:
    Ui::ZegoSoundLevelDemo *ui;
    std::vector<ZegoStream> zegoStreamList;
    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;

    std::string roomID;
    std::string userID;

    QMap<std::string, ZegoSoundFrame*> soundFrameMap;
    QVBoxLayout *soundFrameLayout;
};

#endif // ZEGOSOUNDLEVELDEMO_H
