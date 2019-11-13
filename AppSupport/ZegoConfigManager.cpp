#include "ZegoConfigManager.h"
#include <QTime>
#include <string>

ZegoConfigManager *ZegoConfigManager::instance()
{
    static ZegoConfigManager oInstance;
    return &oInstance;
}

unsigned int ZegoConfigManager::getAppID()
{
    // 填写appID
    return ;
}

std::string ZegoConfigManager::getAppSign()
{
    // 填写appSign
    return "";
}

bool ZegoConfigManager::isTestEnviroment()
{
    return true;
}

std::string ZegoConfigManager::getRandomUserID()
{
    QTime time = QTime::currentTime();
    qsrand(uint(time.msec()+time.second()*1000));
    int n = qrand() % 1000;
    return std::to_string(n);
}

ZegoConfigManager::ZegoConfigManager()
{

}
