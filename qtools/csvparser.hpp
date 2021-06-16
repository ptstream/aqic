#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

#include <QFile>
#include <QVector>
#include <QDebug>

/*! \brief Parse a csv file.
 *
 *  A typical algorithm is
 *  \code
 *  CCSVParser parser (fileName);
 *  if (parser.nextRecord ())
 *  {
 *    // Decode the header.
 *  }
 *
 *   while (parser.nextRecord ())
 *   {
 *     QVector<QByteArray> fields = parser.fields ();
 *     // Parse fields.
 *   }
 *  \endcode
 */
class CCSVParser
{
public:
  /*! Default constructor. */
  CCSVParser ();

  /*! Constructor with file name. */
  CCSVParser (QString const & fileName);

  /*! Destructor. */
  virtual ~CCSVParser ();

  /*! Returns true if the file is open corectly. */
  bool isDone () const { return m_done; }

  /*! Returns true is the file starts by ef, bb, bf characters. */
  bool isUtf8 () const { return m_utf8; }

  /*! Returns the field separator. Comma by default. */
  char fieldSeparator () const { return m_fieldSeparator; }

  /*! Returns the record separator. linefeed by default. */
  char recordSeparator () const { return m_recordSeparator; }

  /*! Sets the field separator. */
  void setFieldSeparator (char c) { m_fieldSeparator = c; }

  /*! Sets the record separator. */
  void setRecordSeparator (char c) { m_recordSeparator = c; }

  /*! Sets the file name. */
  void setFileName (QString const & fileName);

  /*! Closes the opened file. */
  void closeFile () { m_file.close (); }

  /*! Gets the nest record. */
  bool nextRecord ();

  /*! Returns the fields after call nextRecord. */
  QVector<QByteArray> fields ();

  /*! Returns the current record. */
  QByteArray const & record () const { return m_record; }

  /*! Sets the current record. */
  void setRecord (QByteArray const & record) { m_record = record; }

  /*! Decodes text depending on the value of utf8. If utf8= false, assums text is latin1. */
  QString decodeText (QByteArray const & text) const;

private :
  inline bool getChar (char* c); // Gets thenext character.
  void initSize (); // Initializes the size of different arrays.

protected:
  QFile m_file;
  QByteArray          m_record;
  QVector<QByteArray> m_fields;
  QByteArray          m_field;
  QByteArray          m_buffer;
  bool                m_done             = false;
  bool                m_utf8             = false;
  char                m_fieldSeparator   = ',';
  char                m_recordSeparator  = '\n';
  int                 m_bufferSize       = 8192;
  int                 m_fieldCount       = 30;
  int                 m_fieldSize        = 128;
  int                 m_recordSize       = 2048;
  int                 m_bufferCharPos    = 0;
  int                 m_bufferDataLength = 0;
};

bool CCSVParser::getChar (char* c)
{
  if (m_bufferCharPos == m_bufferDataLength)
  {
    m_bufferDataLength = m_file.read (m_buffer.data (), m_bufferSize);
    if (m_bufferDataLength == 0)
    {
      m_buffer.clear ();
      m_record.clear ();
      return false;
    }

    m_bufferCharPos = 0;
  }

  *c = m_buffer[m_bufferCharPos];
  ++m_bufferCharPos;
  return true;
}

#endif // CSVPARSER_HPP
