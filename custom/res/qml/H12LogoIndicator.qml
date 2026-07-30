import QtQuick 2.12

import QGroundControl.ScreenTools   1.0

Item {
    id:             _root
    anchors.top:    parent.top
    anchors.bottom: parent.bottom
    width:          height * 1.2

    property bool showIndicator: true

    Image {
        anchors.fill:    parent
        anchors.margins: ScreenTools.defaultFontPixelHeight * 0.15
        source:          "qrc:/Custom/res/H12LogoPng"
        fillMode:        Image.PreserveAspectFit
        smooth:          true
        mipmap:          true
    }
}