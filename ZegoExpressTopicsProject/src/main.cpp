#include <QApplication>
#include "ExpressDemo/ZegoExpressDemo.h"
#include "AppSupport/ZegoCrashHelper.h"

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(ExceptionFilter);
    QApplication a(argc, argv);
    ZegoExpressDemo w;
    w.setMinimumSize(1024, 768);
    w.showMaximized();
    return a.exec();
}
