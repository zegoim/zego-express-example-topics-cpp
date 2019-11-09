#include "ZegoExpressDemo.h"
#include <QApplication>
#include "AppSupport/ZegoCrashHelper.h"

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(ExceptionFilter);
    QApplication a(argc, argv);
    ZegoExpressDemo w;
    w.showMaximized();
    return a.exec();
}
