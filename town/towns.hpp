#ifndef TOWNS_HPP
#define TOWNS_HPP

#include "../town/airqualities.hpp"
#include "town.hpp"
#include <QHash>

class CTowns : public QHash<QString, CTown>, public CGlobalInstance<CTowns>
{
public:
  CTowns (QStringList const & fileNames = QStringList ());
  ~CTowns () override = default;

  void load (QStringList fileNames);
  QMultiHash<QString, QString> const & epcis () const { return m_epcis; }
  QMultiHash<QString, QString>& epcis () { return m_epcis; }

  QString regionFromTownCode (QString const & code) const;
  QString regionFromTownName (QString townName) const;
  QList<CTown> towns (QString const & region) const;
  QMap<QString, CTown> updateAirQualities (TTAQs const & airQualities);

  static CTowns* gblInst () { return static_cast<CTowns*>(CGlobalInstance<CTowns>::gblInst ()); }

private:
  void loadPath (QStringList const & fileNames);

private :
  QMultiHash<QString, QString> m_epcis;
};

QDataStream& operator << (QDataStream& out, CTowns const & towns);
QDataStream& operator >> (QDataStream& in, CTowns& towns);

#endif // TOWNS_HPP
