#ifndef TOWNS_HPP
#define TOWNS_HPP

#include "../town/airqualities.hpp"
#include "town.hpp"
#include <QHash>

/*! This is the towns container. */
class CTowns : public QHash<QString, CTown>, public CGlobalInstance<CTowns>
{
public:
  /*! Constructor. */
  CTowns (QStringList const & fileNames = QStringList ());

  /*! Destructor. */
  ~CTowns () override = default;

  /*! Loads the container from a json file. */
  void load (QStringList fileNames);

  /*! Returns the epci hash table. */
  QMultiHash<QString, QString> const & epcis () const { return m_epcis; }

  /*! Returns the epci hash table. */
  QMultiHash<QString, QString>& epcis () { return m_epcis; }

  /*! Returns the region code from town code. */
  QString regionFromTownCode (QString const & code) const;

  /*! Returns the region code from town name. */
  QString regionFromTownName (QString townName) const;

  /*! Returns the town code list from the region code. */
  QList<CTown> towns (QString const & region) const;

  /*! Updates the current air qualities. */
  QMap<QString, CTown> updateAirQualities (TTAQs const & airQualities);

  /*! Returns the global instance. */
  static CTowns* gblInst () { return static_cast<CTowns*>(CGlobalInstance<CTowns>::gblInst ()); }

private:
  void loadPath (QStringList const & fileNames);

private :
  QMultiHash<QString, QString> m_epcis;
};

QDataStream& operator << (QDataStream& out, CTowns const & towns);
QDataStream& operator >> (QDataStream& in, CTowns& towns);

#endif // TOWNS_HPP
