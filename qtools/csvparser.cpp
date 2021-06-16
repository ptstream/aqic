#include "csvparser.hpp"
#include <QFile>

char const doubleQuote     = '"';
char const replacementChar = static_cast<unsigned char>(0xff);
char const ef              = static_cast<unsigned char>(0xef);
char const bb              = static_cast<unsigned char>(0xbb);
char const bf              = static_cast<unsigned char>(0xbf);

CCSVParser::CCSVParser ()
{
  initSize ();
}

CCSVParser::CCSVParser (QString const & fileName)
{
  initSize ();
  setFileName (fileName);
}

CCSVParser::~CCSVParser ()
{
  if (m_file.isOpen ())
  {
    m_file.close ();
  }
}

void CCSVParser::initSize ()
{
  m_fields.reserve (m_fieldCount);
  m_field.reserve (m_fieldSize);
  m_record.reserve (m_recordSize);
  m_buffer.resize (m_bufferSize);
}

void CCSVParser::setFileName (QString const & fileName)
{
  m_file.setFileName (fileName);
  m_done = m_file.open (QIODevice::ReadOnly | QIODevice::Unbuffered);
}

bool CCSVParser::nextRecord ()
{
  Q_ASSERT (m_done);
  m_record.clear ();
  char  c;
  bool  quote    = false;
  bool  ok       = getChar (&c);
  qint8 utf8Char = 0;
  while (ok)
  {
    bool skipChar = false;
    if (c == m_recordSeparator)
    {
      if (!quote)
      {
        break;
      }
    }
    else if (c == doubleQuote)
    {
      if (!quote)
      {
        quote    = true;
        skipChar = true;
      }
      else
      {
        char nextChar;
        if (!getChar (&nextChar))
        {
          return false;
        }

        if (nextChar != doubleQuote)
        {
          quote    = false;
          ok       = false;
          c        = nextChar;
          skipChar = true;
        }
      }
    }
    else if (c == ef || c == bb || c == bf)
    {
      ++utf8Char;
      skipChar = true;
      m_utf8   = utf8Char == 3;
    }

    if (!skipChar)
    {
      if (c == m_fieldSeparator && quote)
      {
        c = replacementChar;
      }

      m_record.append (c);
    }

    if (ok)
    {
      ok = getChar (&c);
    }
    else
    {
      ok = true;
    }
  }

  if (m_record.endsWith ('\r'))
  {
    m_record.truncate (m_record.size () - 1);
  }

  return !m_record.isEmpty ();
}

QVector<QByteArray> CCSVParser::fields ()
{
  Q_ASSERT (m_done);
  m_fields.clear ();
  m_field.clear ();
  char       c;
  if (!m_record.isEmpty ())
  {
    QByteArray::const_iterator it  = m_record.cbegin ();
    QByteArray::const_iterator end = m_record.cend ();
    c                              = *it;
    while (it != end)
    {
      if (c == m_fieldSeparator)
      {
        m_fields.append (m_field);
        m_field.clear ();
      }
      else
      {
        if (c == replacementChar)
        {
          c = m_fieldSeparator;
        }

        m_field.append (c);
      }

      c = *++it;
    }

    if (!m_field.isEmpty ())
    {
      m_fields.append (m_field);
    }
  }

  return m_fields;
}

QString CCSVParser::decodeText (QByteArray const & text) const
{
  return !m_utf8 ? QString::fromLatin1 (text) : QString::fromUtf8 (text);
}

