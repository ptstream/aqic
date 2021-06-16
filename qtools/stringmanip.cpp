#include "stringmanip.hpp"

QString CStringManip::removeDiacritics (QString s, EConversion conv)
{
  s = s.normalized (QString::NormalizationForm_D);
  for (int i = 0, j = 0, len = s.length (); i < len; ++i)
  {
    if (s[i].category () != QChar::Mark_NonSpacing)
    {
      if (i != j)
      {
        s[j] = s[i];
      }

      ++j;
    }
  }

  s = s.normalized (QString::NormalizationForm_C);
  switch (conv)
  {
    case Upper :
      s = s.toUpper ();
      break;

    case Lower :
      s = s.toLower ();
      break;

    default :
      break;
  }

  return s;
}
