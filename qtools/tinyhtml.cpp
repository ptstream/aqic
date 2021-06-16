#include "tinyhtml.hpp"

QString const CTinyHtml::breakLine = QLatin1String ("<br/>");

QString CTinyHtml::value (QString const & label, QString const & value, QString const & unit, bool br, bool bold)
{
  QString text = label + QStringLiteral (": ");
  if (bold)
  {
    text += QStringLiteral ("<b>");
  }

  text += value + unit;
  if (bold)
  {
    text += QStringLiteral ("</b>");
  }

  if (br)
  {
    text += breakLine;
  }

  return text;
}
