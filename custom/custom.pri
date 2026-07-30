
message("H12 QGC ozel derleme ekleniyor")

DEFINES += CUSTOMHEADER=\"\\\"CustomPlugin.h\\\"\"

DEFINES += CUSTOMCLASS=CustomPlugin

TARGET = H12QGC

DEFINES += QGC_APPLICATION_NAME=\"\\\"H12QGC\\\"\"

DEFINES += QGC_ORG_NAME=\"\\\"QGroundControl.org\\\"\"

DEFINES += QGC_ORG_DOMAIN=\"\\\"org.qgroundcontrol\\\"\"

QGC_APP_NAME = "H12QGC"

RESOURCES += $$PWD/custom.qrc

SOURCES += $$PWD/src/CustomPlugin.cc

HEADERS += $$PWD/src/CustomPlugin.h

INCLUDEPATH += $$PWD/src

