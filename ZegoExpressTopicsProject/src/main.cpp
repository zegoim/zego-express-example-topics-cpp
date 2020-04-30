#include <QApplication>
#include "ExpressDemo/ZegoExpressDemo.h"

#ifdef WIN32
#include "AppSupport/ZegoCrashHelper.h"
#endif

int main(int argc, char *argv[])
{
    setbuf(stderr, nullptr);
    setbuf(stdout, nullptr);

#ifdef WIN32
   SetUnhandledExceptionFilter(ExceptionFilter);
#endif
    QApplication a(argc, argv);
    ZegoExpressDemo w;
    w.setMinimumSize(1024, 768);
    w.showMaximized();
    return a.exec();
}
