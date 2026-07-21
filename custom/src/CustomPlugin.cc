#include "CustomPlugin.h"

#include <QtCore/QApplicationStatic>
#include <QtCore/QFile>
#include <QtQml/QQmlApplicationEngine>

Q_APPLICATION_STATIC(CustomPlugin, _customPluginInstance);

CustomPlugin::CustomPlugin(QObject *parent)
    : QGCCorePlugin(parent)
{
}

QGCCorePlugin *CustomPlugin::instance()
{
    return _customPluginInstance();
}

void CustomPlugin::cleanup()
{
    if (_qmlEngine) {
        _qmlEngine->removeUrlInterceptor(_selector);
    }
    delete _selector;
}

void CustomPlugin::paletteOverride(const QString &colorName, QGCPalette::PaletteColorInfo_t &colorInfo)
{
    static const QColor kAccent(0xE6, 0x3D, 0x14);   // #E63D14
    static const QColor kDark(0x1D, 0x1D, 0x1B);     // #1D1D1B
    static const QColor kWhite(0xFF, 0xFF, 0xFF);    // #FFFFFF

    if (colorName == QStringLiteral("window")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = kDark;
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = kDark;
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = kWhite;
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#f2f2f2");
    } else if (colorName == QStringLiteral("text")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = kWhite;
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#8a8a88");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = kDark;
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#9d9d9d");
    } else if (colorName == QStringLiteral("buttonHighlight") || colorName == QStringLiteral("primaryButton")
               || colorName == QStringLiteral("mapButtonHighlight") || colorName == QStringLiteral("hoverColor")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = kAccent;
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#495057");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = kAccent;
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#e4e4e4");
    } else if (colorName == QStringLiteral("buttonHighlightText") || colorName == QStringLiteral("primaryButtonText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = kWhite;
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#777c89");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = kWhite;
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#cad0d0");
    }
    // colorRed / colorGreen / colorOrange / warningText gibi "durum" renklerine
    // bilerek dokunmuyoruz - bunlar batarya/GPS uyarilarinin anlamini tasiyor,
    // marka rengiyle degistirmek ucusta yanlis anlasilmaya yol acabilir.
}

QQmlApplicationEngine *CustomPlugin::createQmlApplicationEngine(QObject *parent)
{
    _qmlEngine = QGCCorePlugin::createQmlApplicationEngine(parent);
    _selector = new CustomOverrideInterceptor();
    _qmlEngine->addUrlInterceptor(_selector);
    return _qmlEngine;
}

QUrl CustomOverrideInterceptor::intercept(const QUrl &url, QQmlAbstractUrlInterceptor::DataType type)
{
    switch (type) {
    case QQmlAbstractUrlInterceptor::QmlFile:
    case QQmlAbstractUrlInterceptor::UrlString:
        if (url.scheme() == QStringLiteral("qrc")) {
            const QString origPath = url.path();
            const QString overrideRes = QStringLiteral(":/Custom%1").arg(origPath);
            if (QFile::exists(overrideRes)) {
                const QString relPath = overrideRes.mid(2);
                QUrl result;
                result.setScheme(QStringLiteral("qrc"));
                result.setPath('/' + relPath);
                return result;
            }
        }
        break;
    default:
        break;
    }
    return url;
}
