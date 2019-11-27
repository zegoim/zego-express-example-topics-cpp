#include "ZegoUtilHelper.h"
#include <QTime>

std::string ZegoUtilHelper::getRandomString()
{
    QTime time = QTime::currentTime();
    qsrand(uint(time.msec()+time.second()*1000));
    int n = qrand() % 1000;
    return std::to_string(n);
}

QJsonArray ZegoUtilHelper::stringToJsonArray(QString json, bool &ok)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {
        ok = true;
        return jsonDocument.array();
    } else {
        ok = false;
        return QJsonArray();
    }
}

QJsonObject ZegoUtilHelper::stringToJsonObject(QString json, bool &ok)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {
        ok = true;
        return jsonDocument.object();
    } else {
        ok = false;
        return QJsonObject();
    }
}

QString ZegoUtilHelper::jsonObjectToString(QJsonObject object)
{
    QJsonDocument doc = QJsonDocument(object);
    return doc.toJson(QJsonDocument::Indented);
}

ZegoUtilHelper::ZegoUtilHelper()
{

}
