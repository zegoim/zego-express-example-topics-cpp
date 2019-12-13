#ifndef ZEGOBARCHART_H
#define ZEGOBARCHART_H

#include <QFrame>
#include <QWidget>
#include <list>
#include <QProgressBar>
#include <QLabel>

class ZegoBarChart : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoBarChart(QWidget *parent = nullptr);
    void updateBarData(std::vector<double> data);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QVector<double> barData;
};

class ZegoSoundFrame: public QFrame{

public:
    explicit ZegoSoundFrame(QWidget *parent = nullptr, std::string streamID="");

    void updateAudioSpectrum(std::vector<double> audioSpectrum);
    void updateSoundLevel(double level);

private:
    ZegoBarChart *audioSpectrumChart;
    QProgressBar *soundLevelChart;
};

#endif // ZEGOBARCHART_H
