#ifndef ZEGOEXPRESSDEMO_H
#define ZEGOEXPRESSDEMO_H

#include <QWidget>
#include <QListWidget>

#include "Publish/ZegoPublishDemo.h"
#include "Play/ZegoPlayDemo.h"
#include "VideoCommunication/ZegoVideoCommunicationDemo.h"

namespace Ui {
class ZegoExpressDemo;
}

class ZegoExpressDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ZegoExpressDemo(QWidget *parent = nullptr);
    ~ZegoExpressDemo();

public slots:
    void onBasicUseCaseItemChanged(QListWidgetItem *item);
    void onAdvanceUseCaseItemChanged(QListWidgetItem *item);
    void doChangeTopic(QString itemText);

private:
    Ui::ZegoExpressDemo *ui;

    QString currentItemText;
    QWidget* currentTopicWidget = nullptr;
};

#endif // ZEGOEXPRESSDEMO_H
