#ifndef TINYHTML_HPP
#define TINYHTML_HPP

#include <QString>

/*! Not use actually. */
class CTinyHtml
{
public:
  static QString value (QString const & label, QString const & value, QString const & unit = QString (),
                        bool br = true, bool bold = false);

  static QString const breakLine;
};

#endif // TINYHTML_HPP
