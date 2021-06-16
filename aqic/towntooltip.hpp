#ifndef TOWNTOOLTIP_HPP
#define TOWNTOOLTIP_HPP

#include <QObject>
#include <qqml.h>

class CTowns;
class CTown;
class CMapView;
class CAQLegend;
class QGeoCoordinate;

class CTownTooltip : public QObject
{
  Q_OBJECT
public:
  CTownTooltip (QObject *parent = nullptr);

  Q_INVOKABLE QString text (QString const & code, QGeoCoordinate const & loc);

  void setMapView (CMapView const * mapView) { m_mapView = mapView; }
  static void resetTooltip ();

protected:
  QString tooltipText (CTown const & town, float lat, float lon);

protected:
  CMapView const * m_mapView = nullptr;

private:
  QString m_styleSheet;
};

#endif // TOWNTOOLTIP_HPP
