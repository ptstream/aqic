#ifndef TOWNCODES_HPP
#define TOWNCODES_HPP

#include <QString>
#include <QMap>

/*! \brief This class is used to correct the insee code in air qualities.
 *
 * For example, for the Alpes Cote d'Azur region, the air quality index is defined for Marseille by neighborhoods
 * not for the whole city. The contours of cities are the outer contours of neighborhoods.
 * When the air qualities are decoded, the Insee district code is replaced by the Insee code for the municipality.
 */
class CTownCodes : public QMap<QString, QString>
{
public:
  /*! Single contructor. */
  CTownCodes (QString const & fileName);
};

#endif // TOWNCODES_HPP
