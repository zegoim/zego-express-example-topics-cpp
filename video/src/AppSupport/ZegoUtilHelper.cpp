#include "ZegoUtilHelper.h"
#include <QTime>
#include <vector>

#if defined (WIN32)
#include <Windows.h>
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat);
#elif TARGET_OS_OSX
#import <Foundation/Foundation.h>
#import <CoreGraphics/CGImage.h>
#endif

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

#if defined(WIN32)
bool ZegoUtilHelper::convertUtf8ToANSI(const std::string& utf8, std::string& ansi)
{
    if (utf8.length() == 0)
        return false;

    //utf8 to unicode
    int len = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    if (len == 0)
        return false;

    std::vector<wchar_t> unicode(len);
    ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &unicode[0], len);

    //unicode to ansi
    len = ::WideCharToMultiByte(CP_ACP, 0, &unicode[0], -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return false;

    std::vector<char> dest(len);
    ::WideCharToMultiByte(CP_ACP, 0, &unicode[0], -1, &dest[0], len, NULL, NULL);

    ansi = &dest[0];
    return true;
}
#endif

QPixmap ZegoUtilHelper::QPixmapFromZegoSnapshot(void *snapshot)
{
    QPixmap pixmap;
#if defined(WIN32)

    pixmap = qt_pixmapFromWinHBITMAP(HBITMAP(snapshot), 0);

#elif TARGET_OS_OSX

    CGImageRef cgImage =  (CGImageRef)snapshot;

    CFDataRef pixelData= CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    unsigned char * ucBuffer = (unsigned char *)CFDataGetBytePtr(pixelData);

    size_t width = CGImageGetWidth(cgImage);
    size_t height = CGImageGetHeight(cgImage);
    QImage qImage(ucBuffer ,width, height, QImage::Format_ARGB32);

    pixmap = QPixmap::fromImage(qImage);

#else

   pixmap =QPixmap::fromImage(*(QImage*)snapshot);

#endif
    return  pixmap;
}

ZegoUtilHelper::ZegoUtilHelper()
{

}
