import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import Qt.labs.platform 1.1
import aqic.town.tooltip 1.0

Item {
  signal mouseClicked(double lat, double lon)
  signal markerClicked(int index)
  signal markerEntered(int index)
  signal markerExited(int index)
  signal townClicked(string code, double lat, double lon)

  id : mapWidget
  visible: true

  Plugin {
    id: providerPlugin
    name: "osm"
//    PluginParameter { name: "osm.mapping.cache.disk.size"; value: "C:/Users/patrice/AppData/Local/aqic/cache/QtLocation" }
  }

  Map {
    id : map
    anchors.fill: parent
    plugin: providerPlugin
    focus: true
    gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.PinchGesture
    onCopyrightLinkActivated: Qt.openUrlExternally(link)

    // When the main window dimensions change, items, ... are not at the good position.
    // Make two pan to redraw items correctly
    function mapRefocuses () {
      var center = map.center;
      center.latitude += 0.001;
      map.center = center;
      center.latitude -= 0.001;
      map.center = center;
    }

    function delItems() {
      var cItems = items.length
      for (var i = 0; i < cItems; ++i) {
        items[i].destroy ()
      }
      items = []
      itemCircle = null
      clearMapItems ()
    }

    function addPolygon (path, code, color) {
      var item = polygonComponent.createObject (map, {  path: path, code: code, color:color, opacity: polygonOpacity })
      items.push (item)
      map.addMapItem (item)
    }

    function changeColors (colors) {
      var cItems = items.length
      for (var i = 0; i < cItems; ++i) {
        if (colors[items[i].code] !== undefined) {
          items[i].color = colors[items[i].code]
        }
        else {
          items[i].color = "#DDDDDD"
        }
      }
    }

    function removeCircle () {
      if (itemCircle != undefined)
      {
        itemCircle.destroy ()
        map.removeMapItem (itemCircle)
        itemCircle = null
      }
    }

    function mapCircle (lat, lon, radius, color) {
      var item = circleComponent.createObject (map)
      item.center.latitude = lat
      item.center.longitude = lon
      item.radius = radius
      item.border.color = color;
      itemCircle = item
      map.addMapItem (item)
    }

    function setPolygonOpacity (opacity) {
      polygonOpacity = opacity
      var cItems = items.length
      for (var i = 0; i < cItems; ++i) {
        items[i].opacity = opacity
      }
    }

    MouseArea {
      id: mapMouseArea
      hoverEnabled: true
      anchors.fill: parent
      acceptedButtons: Qt.LeftButton
      onClicked: {
        var gps = map.toCoordinate (Qt.point(mouseX, mouseY), false)
        mouseClicked (gps.latitude, gps.longitude)
      }
      onEntered: {
        if (map.toolTipActivated) {
          townTooltip.text ('', QtPositioning.coordinate (0, 0))
        }
      }
      onExited: {
        if (map.toolTipActivated) {
          townTooltip.text ('', QtPositioning.coordinate (0, 0))
        }
      }
      onPositionChanged: {
        if (map.toolTipActivated) {
          townTooltip.text ('', QtPositioning.coordinate (0, 0))
        }
      }
    }

    property var polygonComponent: Qt.createComponent("qrc:///polygon.qml")
    property var circleComponent: Qt.createComponent("qrc:///circle.qml")
    property var items:[]
    property var itemCircle: null
    property real polygonOpacity: 0.7
    property bool toolTipActivated: true
  }
}
