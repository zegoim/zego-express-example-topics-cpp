#include "ZegoUtilHelper.h"
#include <QTime>

std::string ZegoUtilHelper::getRandomString()
{
    QTime time = QTime::currentTime();
    qsrand(uint(time.msec()+time.second()*1000));
    int n = qrand() % 1000;
    return std::to_string(n);
}

ZegoUtilHelper::ZegoUtilHelper()
{

}
