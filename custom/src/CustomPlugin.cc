#include "CustomPlugin.h"

#include <QtCore/QFile>
#include <QtQml/QQmlApplicationEngine>

CustomPlugin::CustomPlugin(QGCApplication* app, QGCToolbox* toolbox)
    : QGCCorePlugin(app, toolbox)
{
}

CustomPlugin::~CustomPlugin()
{
}

QString CustomPlugin::brandImageIndoor(void) const
{
    return QStringLiteral("/Custom/res/QGCLogoFull");
}

QString CustomPlugin::brandImageOutdoor(void) const
{
    return QStringLiteral("/Custom/res/QGCLogoFull");
}

void CustomPlugin::paletteOverride(QString colorName, QGCPalette::PaletteColorInfo_t& colorInfo)
{
    static const QColor kAccent(0xE6, 0x3D, 0x14);
    static const QColor kDark(0x1D, 0x1D, 0x1B);
    static const QColor kWhite(0xFF, 0xFF, 0xFF);

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
}

QQmlApplicationEngine* CustomPlugin::createQmlApplicationEngine(QObject* parent)
{
    _qmlEngine = QGCCorePlugin::createQmlApplicationEngine(parent);
    _selector = new CustomOverrideInterceptor();
    _qmlEngine->setUrlInterceptor(_selector);
    return _qmlEngine;
}

const QVariantList& CustomPlugin::toolBarIndicators(void)
{
    if (_toolBarIndicatorList.size() == 0) {
        _toolBarIndicatorList = QGCCorePlugin::toolBarIndicators();
        _toolBarIndicatorList.prepend(QVariant::fromValue(QUrl::fromUserInput(QStringLiteral("qrc:/custom/qml/H12LogoIndicator.qml"))));
    }
    return _toolBarIndicatorList;
}

QUrl CustomOverrideInterceptor::intercept(const QUrl& url, QQmlAbstractUrlInterceptor::DataType type)
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