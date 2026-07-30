#pragma once

#include <QtCore/QVariant>
#include <QtQml/QQmlAbstractUrlInterceptor>

#include "QGCCorePlugin.h"

class QQmlApplicationEngine;

class CustomPlugin : public QGCCorePlugin
{
    Q_OBJECT

public:
    CustomPlugin(QGCApplication* app, QGCToolbox* toolbox);
    ~CustomPlugin();

    // QGCCorePlugin overrides
    QString                 brandImageIndoor         (void) const final;
    QString                 brandImageOutdoor        (void) const final;
    void                    paletteOverride          (QString colorName, QGCPalette::PaletteColorInfo_t& colorInfo) final;
    QQmlApplicationEngine*  createQmlApplicationEngine(QObject* parent) final;
    const QVariantList&     toolBarIndicators        (void) final;

private:
    QQmlApplicationEngine           *_qmlEngine             = nullptr;
    class CustomOverrideInterceptor *_selector              = nullptr;
    QVariantList                     _toolBarIndicatorList;
};

class CustomOverrideInterceptor : public QQmlAbstractUrlInterceptor
{
public:
    CustomOverrideInterceptor() = default;
    QUrl intercept(const QUrl& url, QQmlAbstractUrlInterceptor::DataType type) final;
};