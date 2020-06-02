#ifndef ZEGOEXPRESSDEMO_H
#define ZEGOEXPRESSDEMO_H

#include <QWidget>
#include <qlistwidget.h>

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
    void on_pushButton_ReadDocument_clicked();
    void onAdvanceUseCaseItemChanged(QListWidgetItem *item);
    void onBasicUseCaseItemChanged(QListWidgetItem *item);
    void doChangeTopic(QString itemText);

private:
    Ui::ZegoExpressDemo *ui;

    QString currentItemText;
    QWidget* currentTopicWidget = nullptr;
};

#endif // ZEGOEXPRESSDEMO_H
