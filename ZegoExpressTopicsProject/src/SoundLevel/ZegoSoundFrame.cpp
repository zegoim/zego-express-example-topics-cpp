#include "ZegoSoundFrame.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <math.h>
#include <QtDebug>

ZegoBarChart::ZegoBarChart(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(100, 150);
}

void ZegoBarChart::updateBarData(std::vector<float> data)
{
    this->barData.clear();
    this->barData.reserve(int(data.size()));
    for(const float&d: data){
        this->barData.append(d);
    }
    this->repaint();
}

void ZegoBarChart::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    if(barData.isEmpty())
    {
        return;
    }

    double unitWidth = this->width()/1.0/barData.count();
    double unitHeight = this->height()/100.0;

    for(int i = 0; i < barData.count(); ++i)
    {
        double barWidth = unitWidth/4*3;
        double barHight = barData.at(i) * unitHeight;
        double left = 0 + i*unitWidth + unitWidth/2 - barWidth/2;
        double top = this->height() - barHight;

        QRectF r = QRectF(left, top, barWidth, barHight);
        QPainterPath path;
        path.addRect(r);
        painter.fillPath(path, QColor(185, 185, 185));
    }

    painter.setPen(Qt::black);
    painter.drawLine(0, this->height(), this->width(), this->height());
}

ZegoSoundFrame::ZegoSoundFrame(QWidget *parent, std::string streamID): QFrame(parent)
{
    this->setFrameShape(QFrame::Box);

    QVBoxLayout *vLayout = new QVBoxLayout(this);

    QHBoxLayout *hboxSpectrum = new QHBoxLayout;
    hboxSpectrum->addWidget(new QLabel("spectrum:"));
    hboxSpectrum->addStretch(1);
    QLabel *streamIDLable = new QLabel(QString::fromStdString(streamID));
    hboxSpectrum->addWidget(streamIDLable);
    vLayout->addLayout(hboxSpectrum);

    audioSpectrumChart = new ZegoBarChart;
    vLayout->addWidget(audioSpectrumChart);

    QHBoxLayout *hboxLevel = new QHBoxLayout;
    hboxLevel->addWidget(new QLabel("SoundLevel:"));
    soundLevelChart = new QProgressBar;
    soundLevelChart->setTextVisible(false);
    hboxLevel->addWidget(soundLevelChart);

    vLayout->addLayout(hboxLevel);
}

void ZegoSoundFrame::updateAudioSpectrum(std::vector<float> audioSpectrum)
{
    std::vector<float> adjustedData;
    for(double valueTemp: audioSpectrum){
        // to dB
        float dBValue = 0.0;
        if(valueTemp > 10){
            dBValue = 10.0 * log10(valueTemp);
        } else {
            dBValue = valueTemp;
        }
        adjustedData.push_back(dBValue);
    }

    audioSpectrumChart->updateBarData(adjustedData);
}

void ZegoSoundFrame::updateSoundLevel(float level)
{
    soundLevelChart->setValue(int(level));
}
