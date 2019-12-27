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
	
    virtual void onMediaPlayerStateUpdate(ZegoMediaPlayerState /*state*/, int /*errorCode*/) {
	}

    virtual void onMediaPlayerNetWorkEvent(ZegoMediaPlayerNetworkEvent /*networkEvent*/) {
	}

    virtual void onMediaPlayerPlayingProcess(unsigned long long /*process in millisecond*/) {
	}

    virtual void onVideoFrame(const char** /* buffer */, ZegoVideoFrameParam /*video frame param*/) {
    }

    virtual void onAudioFrame(const unsigned char * /* buffer */, ZegoAudioFrameParam /*audio frame*/) {
    }

};


class ZegoExpressMediaPlayerCallbackCenter :
	public IZegoExpressMediaPlayerEventHandler,
	public IZegoExpressMediaPlayerVideoHandler,
	public IZegoExpressMediaPlayerAudioHandler{

public:
    ~ZegoExpressMediaPlayerCallbackCenter() override{

    }
    void setCallback(IMediaPlayerCallback* cb){
        this->cb = cb;
    }

	void onMediaPlayerStateUpdate(IZegoExpressMediaPlayer* mediaPlayer, ZegoMediaPlayerState state, int errorCode) override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerStateUpdate(state, errorCode);
        }
	}
	
    void onMediaPlayerNetWorkEvent(IZegoExpressMediaPlayer* mediaPlayer, ZegoMediaPlayerNetworkEvent networkEvent)  override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerNetWorkEvent(networkEvent);
        }
	}

    void onMediaPlayerPlayingProcess(IZegoExpressMediaPlayer* mediaPlayer, unsigned long long process) override {
        Q_UNUSED(mediaPlayer)
        if(cb){
            cb->onMediaPlayerPlayingProcess(process);
        }
	}

    void onAudioFrame(IZegoExpressMediaPlayer* mediaPlayer, const unsigned char* buffer, ZegoAudioFrameParam param) override {
        Q_UNUSED(mediaPlayer)
        if (cb) {
            cb->onAudioFrame(buffer, param);
		}
	}

    void onVideoFrame(IZegoExpressMediaPlayer* mediaPlayer, const char**buffer, ZegoVideoFrameParam param)  override {
        Q_UNUSED(mediaPlayer)
        if (cb) {
            cb->onVideoFrame(buffer, param);
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
    void on_pushButton_loadResource_clicked();
    void on_pushButton_stopPlay_clicked();
    void on_pushButton_resumePlay_clicked();
    void on_pushButton_pausePlay_clicked();
    void on_pushButton_startPlay_clicked();

    void onMediaPlayerStateUpdate(ZegoMediaPlayerState state, int errorCode) override;
    void onMediaPlayerNetWorkEvent(ZegoMediaPlayerNetworkEvent event) override;
    void onMediaPlayerPlayingProcess(unsigned long long process) override;
    void onVideoFrame(const char** buffer, ZegoVideoFrameParam param) override;
    void onAudioFrame(const unsigned char * buffer, ZegoAudioFrameParam param) override;

    void on_horizontalSlider_progress_valueChanged(int value);
    void on_horizontalSlider_volume_valueChanged(int value);

private:
    void printLogToView(QString log);
    void bindEventHandler();

private:
    Ui::ZegoMediaPlayerDemo *ui;

    ZEGO::EXPRESS::IZegoExpressEngine *engine = nullptr;
    ZEGO::EXPRESS::IZegoExpressMediaPlayer *mediaPlayer = nullptr;
};

#endif // ZEGOMEDIAPLAYERDEMO_H
