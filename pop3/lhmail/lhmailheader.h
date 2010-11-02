
/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAILHEADER_H
#define LHMAILHEADER_H

#include <QObject>
#include <QMap>
#include <QStringList>

class QString;
class QDateTime;
class QTextStream;


class LHMailHeaderPrivate;

/**
 * This class represents header used in @ref LHMail or @ref LHMailPart.
 * You can use this class to add or change header od mail o mail part. <br>
 * Example, setting 'reply to':
 * <pre>
 * LHMail* mail = new LHMail("me@lefthand.pl", "you@lefthand.pl", 
 *                           "test", "test mail", 
 *                           1);  //delete after send
 * mail->header().setData("Reply-To", "reply@lefthand.pl");
 * mail->send();
 * </pre>
 * 
 * You should be carefull when changing Content-Type, or Content-Transfer-Encoding 
 * since they are set automaticaly.
 * @short Representation of header used in @ref LHMail or @ref LHMailPart.
 * @see LHMail
 * @see LHMailPart
 * 
*/

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHMailHeader : public QObject
{
    Q_OBJECT

public:
	/**
	 * Constructs a LHMailHeader object.
	*/

	LHMailHeader();
	LHMailHeader (const LHMailHeader &copy);
	LHMailHeader &operator= (const LHMailHeader &copy);
    
         virtual ~LHMailHeader();
	
	/**
	 * Sets one header line of form: <br>
	 * field: data
	 * @param field Name of mail header field (without ':')
	 * @param data Content of header line, after 'field: ', but without 
	 *             parameters like charset, see @ref setParameter.
	 */ 
	virtual void setData(const QString& field, const QString& data);
	
	/**
	 * Used to test if given field contains given data.
	 * Mail headers are not case sensitive, so should be checked with cs == FALSE.
	 * @param field Name of mail header field (without ':')
	 * @param data Content of header line, after 'field: ', but without 
	 *             parameters like charset, see @ref setParameter.
	 * @param cs True if you want case sensitive check
	 */  
	virtual bool testData(const QString& field, const QString& data, bool cs = FALSE);
	
	/**
	 * Returnes content of header field. It is exactly what was set by @ref setData.
	 * If header was parsed from received data, then this method will return 
	 * decoded data after 'field: '. <br>
	 * If this field is configured as containing parameters (see @ref addFieldProp), 
	 * then this method will return data without this patameters.
	 * @param field Name of mail header field (without ':')
	 * @return  Content of header line, after 'field: ', but without 
	 *             parameters like charset, see @ref setParameter.
	 */ 
	virtual QString getData(const QString& field);
	
	/**
	 * Sets parameter for given field. Like charset for Content-Type.
	 * @param field Name of mail header field (without ':')
	 * @param parameter Name of parameter
	 * @param data Data of parameter (will be encoded and put into "")
	 */ 
	virtual void setParameter(const QString& field, const QString& parameter, const QString& data);
	
	/**
	 * Returnes content of header field parameter. 
	 * It is exactly what was set by @ref setParameter. <br>
	 * If header was parsed from received data, then this method will return 
	 * decoded data using such pattern: <br>
	 * field:<field data>; parameter=<parameter data><br>
	 * For parsing this field must be configured as containing parameters (see @ref addFieldProp).
	 * @param field Name of mail header field (without ':')
	 * @param parameter Name of parameter
	 * @return  Content of parameter data (decoded and without "")
	 */ 
	virtual QString getParameter(const QString& field, const QString& parameter);
	
	/**
	 * Removes parameter for this field.
	 * @param field Name of mail header field (without ':')
	 * @param parameter Name of parameter
	 */  
	virtual void removeParameter(const QString& field, const QString& parameter);
	
	/**
	 * Same like @ref setData, but for @ref QDateTime values.
	 * Calls @ref LHMime::encodeRfcTime to convert to string
	 * @param field Name of mail header field (without ':')
	 * @param data Data for field in @ref QDateTime format.
	 */ 
	virtual void setDataDateTime(const QString& field, const QDateTime& data);
	
	/**
	 * Same like @ref getData, but for @ref QDateTime values.
	 * Calls @ref LHMime::decodeRfcTime to convert from string
	 * @param field Name of mail header field (without ':')
	 * @return Field data decoded to @ref QDateTime
	 */ 
	virtual QDateTime getDataDateTime(const QString& field);
	
         /**
	 * @return Header converted to 7bit data (RFC 2047) ready for sending.
	 */ 
	virtual QString getSendData (const QString& field = QString::null);
	
	/**
	 * Set encoding used in strings (especialy Subject, From, To, Cc and Bcc)
	 * LHMail sets encoding to mail of header to mail encoding. 
	 * See @ref LHMailBase::setEncoding.
	 * @param encod String representing encoding like ISO-8859-2
	 */ 
	virtual int setEncoding(const QString& encod);

	/**
	 * @return encoding of mail or mail part header (like ISO-8859-2)
	 */
	virtual QString encoding();
	
	/**
	 * Used to check if mail or mail part is single or multipart.
	 * You should use: @ref LHMailBase::isSinglePart.
	 * @return True if mail is not multipart
	 */
	virtual bool isSinglePart();
	
	/**
	 * Adds information that this field may contain given parameter. This information
	 * is used when parsing header data. Only such set parameters are looked for.
	 * @param field Name of mail header field (without ':')
	 * @param parameter Name of parameter
	 */ 
	static void addFieldParameter(const QString& field, const QString& parameter);
	
	/**
	 * Tests if given field may have parameters. Used by @ref parse.
	 * @param field Name of mail header field (without ':')
	 */ 
	static bool testFieldHasParameter(const QString& field);

	/**
	 * Parses mail header data
	 * @return Position in string when header ends
	 */ 
	virtual uint parse(const QString& data);
	
	/**
	 * Parse one line of header
	 */ 
	virtual void parseLine(const QString& line);
	
	/**
	 * Return file name of attachment
	 * @param NO_PATH If True then name is truncated to not include path
	 */ 
	virtual QString getFileName(bool NO_PATH = TRUE);
	
	/**
	 * Used by @ref LHMail, do not use directly.
	 */ 
	virtual bool isSendDataValid();
	
protected:
	
	virtual bool setSendDataValid(bool VAL = TRUE);
	
private:

	LHMailHeaderPrivate* d;
};

#endif
