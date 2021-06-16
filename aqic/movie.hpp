#ifndef MOVIE_HPP
#define MOVIE_HPP

#include "../wtools/player.hpp"
#include "../town/airqualities.hpp"
#include <QTimer>
#include <QElapsedTimer>

class CMapView;

struct SAQs : public TTAQs
{
  SAQs (TTAQs const & taqs, float averageIndex) : m_averageIndex (averageIndex)
  {
    *static_cast<TTAQs*>(this) = taqs;
    updateIndexStats ();
  }

  void updateIndexStats ();

  float m_averageIndex;
  float m_max = 0.0F;
};

class CMovie  : public CPlayer
{
  Q_OBJECT
public:
  CMovie (CMapView* mapView, QWidget* parent = nullptr);

  void clear () { m_aqs.clear (); }
  void prepare ();

  int mostTownIndexFrame (quint8 index) const;
  int highestAverageFrame () const;
  int lowestAverageFrame () const;
  int highestMaxFrame () const;
  int lowestMaxFrame () const;

  void updateFrame ();
  void nextFrame ();


protected slots:
  void frameRateChanged (int);
  void playStateChanged (bool = false);
  void forwardClicked (bool);
  void backwardClicked (bool);
  void changeFrame (int position);
  void updateFrame (int action);
  void pausePlayer ();
  void startPlayer ();
  void highestAverage (bool);
  void highestMax (bool);
  void lowestAverage (bool);
  void lowestMax (bool);

signals:
  void frameUpdated (int dateIndex);

private:
  QTimer        m_timer;
  QElapsedTimer m_frameRate;
  qint64        m_frameCount    = 0LL;
  int           m_currentFrame  = 0;
  int           m_timerInterval = 41;
  QVector<SAQs> m_aqs;
  CMapView*     m_mapView;
};

#endif // MOVIE_HPP
