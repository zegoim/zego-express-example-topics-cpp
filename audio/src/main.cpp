#include "ExpressDemo/ZegoExpressDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZegoExpressDemo w;
    w.setMinimumSize(1024, 768);
    w.showMaximized();

    return a.exec();
}
