#ifndef ZEGOUTILHELPER_H
#define ZEGOUTILHELPER_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QImage>
#include <QPixmap>

class ZegoUtilHelper
{
public:
    static std::string getRandomString();
    static QJsonArray stringToJsonArray(QString json, bool &ok);
    static QJsonObject stringToJsonObject(QString json, bool &ok);
    static QString jsonObjectToString(QJsonObject object);

#if defined(WIN32)
    static bool convertUtf8ToANSI(const std::string& utf8, std::string& ansi);
#endif

    static QPixmap QPixmapFromZegoSnapshot(void* snapshot);

public:
    ZegoUtilHelper();
};

#endif // ZEGOUTILHELPER_H
