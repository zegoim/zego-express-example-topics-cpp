#ifndef ZEGOEXPRESSDEMO_H
#define ZEGOEXPRESSDEMO_H

#include <QWidget>
#include <QListWidget>

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

private slots:
    void on_pushButton_go_code_clicked();
    void on_pushButton_go_document_clicked();

private:
    Ui::ZegoExpressDemo *ui;

    QString currentItemText;
    QWidget* currentTopicWidget = nullptr;
};

#endif // ZEGOEXPRESSDEMO_H
