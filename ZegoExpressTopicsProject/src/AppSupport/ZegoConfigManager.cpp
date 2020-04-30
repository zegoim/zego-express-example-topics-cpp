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
    return ;    // input AppID here
}

std::string ZegoConfigManager::getAppSign()
{
    return "";  // input AppSign here
}

bool ZegoConfigManager::isTestEnviroment()
{
    return true;
}

ZegoConfigManager::ZegoConfigManager()
{

}
