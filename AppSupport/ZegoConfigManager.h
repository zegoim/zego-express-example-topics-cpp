#ifndef ZEGOCONFIGMANAGER_H
#define ZEGOCONFIGMANAGER_H

#include <QString>

class ZegoConfigManager
{
public:

    static ZegoConfigManager* instance();
    unsigned int getAppID();
    std::string getAppSign();
    bool isTestEnviroment();

    std::string getRandomUserID();

private:
    ZegoConfigManager();
};

#endif // ZEGOCONFIGMANAGER_H
