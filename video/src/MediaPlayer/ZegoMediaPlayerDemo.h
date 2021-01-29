#ifndef ZEGOMEDIAPLAYERDEMO_H
#define ZEGOMEDIAPLAYERDEMO_H

#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

namespace Ui {
class ZegoMediaPlayerDemo;
}


class IMediaPlayerCallback {
public:
    virtual ~IMediaPlayerCallback() = default;
	
    virtual void onMediaPlayerStateUpdate(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerState /* state */, int /* errorCode */) {
	}

    virtual void onMediaPlayerNetworkEvent(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerNetworkEvent /* networkEvent */) {
	}

    virtual void onMediaPlayerPlayingProgress(IZegoMediaPlayer* /*mediaPlayer*/, unsigned long long /* progress */) {
	}

    virtual void onVideoFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char** /* buffer */, unsigned int* /*dataLenght*/, ZegoVideoFrameParam /* param */) {
    }

    virtual void onAudioFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char * /* buffer */, unsigned int /*dataLenght*/, ZegoAudioFrameParam /* param */) {
    }
};


class ZegoExpressMediaPlayerCallbackCenter :
    public IZegoMediaPlayerEventHandler,
    public IZegoMediaPlayerVideoHandler,
    public IZegoMediaPlayerAudioHandler{

public:
    ~ZegoExpressMediaPlayerCallbackCenter() override{

    }
    void setCallback(IMediaPlayerCallback* cb){
        this->cb = cb;
    }

    void onMediaPlayerStateUpdate(IZegoMediaPlayer* mediaPlayer, ZegoMediaPlayerState state, int errorCode) override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerStateUpdate(mediaPlayer, state, errorCode);
        }
	}
	
    void onMediaPlayerNetworkEvent(IZegoMediaPlayer* mediaPlayer, ZegoMediaPlayerNetworkEvent networkEvent)  override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerNetworkEvent(mediaPlayer, networkEvent);
        }
	}

    void onMediaPlayerPlayingProgress(IZegoMediaPlayer* mediaPlayer, unsigned long long progress) override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerPlayingProgress(mediaPlayer, progress);
        }
	}

    void onAudioFrame(IZegoMediaPlayer* mediaPlayer, const unsigned char* data, unsigned int dataLenght, ZegoAudioFrameParam param) override {
        Q_UNUSED(mediaPlayer)
        if (cb) {
            cb->onAudioFrame(mediaPlayer, data, dataLenght, param);
		}
	}

    void onVideoFrame(IZegoMediaPlayer* mediaPlayer, const unsigned char**data, unsigned int* dataLenght, ZegoVideoFrameParam param)  override {
        Q_UNUSED(mediaPlayer)
        if (cb) {
            cb->onVideoFrame(mediaPlayer, data, dataLenght, param);
		}
	}

private:
    IMediaPlayerCallback* cb = nullptr;
};


class ZegoMediaPlayerDemo : public QWidget, public IMediaPlayerCallback
{
    Q_OBJECT

public:
    explicit ZegoMediaPlayerDemo(QWidget *parent = nullptr);
    ~ZegoMediaPlayerDemo() override;

private slots:
    void on_pushButton_loadResource_1_clicked();
    void on_pushButton_startPlay_1_clicked();
    void on_pushButton_pausePlay_1_clicked();
    void on_pushButton_resumePlay_1_clicked();
    void on_pushButton_stopPlay_1_clicked();
    void on_horizontalSlider_progress_1_valueChanged(int value);
    void on_horizontalSlider_playVolume_1_valueChanged(int value);
    void on_horizontalSlider_publishVolume_1_valueChanged(int value);
    void on_comboBox_voiceChanger_1_currentIndexChanged(int index);
    void on_pushButton_setAudioTrackIndex_1_clicked();

    void on_pushButton_loadResource_2_clicked();
    void on_pushButton_startPlay_2_clicked();
    void on_pushButton_pausePlay_2_clicked();
    void on_pushButton_resumePlay_2_clicked();
    void on_pushButton_stopPlay_2_clicked();
    void on_horizontalSlider_progress_2_valueChanged(int value);
    void on_horizontalSlider_playVolume_2_valueChanged(int value);
    void on_horizontalSlider_publishVolume_2_valueChanged(int value);

    void onMediaPlayerStateUpdate(IZegoMediaPlayer* mediaPlayer, ZegoMediaPlayerState state, int errorCode) override;
    void onMediaPlayerNetworkEvent(IZegoMediaPlayer* mediaPlayer, ZegoMediaPlayerNetworkEvent event) override;
    void onMediaPlayerPlayingProgress(IZegoMediaPlayer* mediaPlayer, unsigned long long process) override;
    void onVideoFrame(IZegoMediaPlayer* mediaPlayer, const unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param) override;
    void onAudioFrame(IZegoMediaPlayer* mediaPlayer, const unsigned char * data, unsigned int dataLength, ZegoAudioFrameParam param) override;


private:
    void printLogToView(const QString &log);
    void bindEventHandler();

private:
    Ui::ZegoMediaPlayerDemo *ui;

    IZegoExpressEngine *engine = nullptr;
    std::string currentRoomID;

    IZegoMediaPlayer *mediaPlayer1 = nullptr;
    IZegoMediaPlayer *mediaPlayer2 = nullptr;
};

#endif // ZEGOMEDIAPLAYERDEMO_H
