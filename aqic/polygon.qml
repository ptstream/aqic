import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import aqic.town.tooltip 1.0

MapPolygon {
  id: polygon
  objectName: "polygon"
  border.color: "gray"
  MouseArea {
    id: mouseArea
    anchors.fill: parent
    acceptedButtons: Qt.LeftButton
    cursorShape: Qt.PointingHandCursor
    hoverEnabled: true
    onClicked: {
      var gps = map.toCoordinate (Qt.point(mouseX, mouseY), false)
      townClicked(code, gps.latitude, gps.longitude);
    }
    onEntered: {
      if (map.toolTipActivated) {
        townTooltip.text (code, map.toCoordinate (mapToItem (map, mouseArea.mouseX, mouseArea.mouseY)))
      }
    }
    onExited: {
      if (map.toolTipActivated) {
        townTooltip.text ('', QtPositioning.coordinate (0, 0))
      }
    }
    onPositionChanged: {
      if (map.toolTipActivated) {
        townTooltip.text (code, map.toCoordinate (mapToItem (map, mouseArea.mouseX, mouseArea.mouseY)))
      }
    }
  }

  property string code
}

