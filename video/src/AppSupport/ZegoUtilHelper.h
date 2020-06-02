#ifndef ZEGOUTILHELPER_H
#define ZEGOUTILHELPER_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class ZegoUtilHelper
{
public:
    static std::string getRandomString();
    static QJsonArray stringToJsonArray(QString json, bool &ok);
    static QJsonObject stringToJsonObject(QString json, bool &ok);
    static QString jsonObjectToString(QJsonObject object);

public:
    ZegoUtilHelper();
};

#endif // ZEGOUTILHELPER_H
