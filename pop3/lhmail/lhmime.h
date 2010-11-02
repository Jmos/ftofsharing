/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMIME_H
#define LHMIME_H
#define UNIT_API __declspec(dllexport)
#include <qobject.h>
#include <stdlib.h>
#include <time.h>

class QString;
class QDateTime;

//class LHMimePrivate;

/**
 * Helper class to do some encoding and decoding needed by mail system.
 */
#ifdef Q_OS_WIN32
	UNIT_API
#endif

class LHMime : public QObject
{
    Q_OBJECT

public:
	/**
	 * Encodes string using Base64 algorithm.
	 * @param str String to be encoded
	 * @param codec_name Name of codec used to convert @ref QString to <code>char*</code>. 
	 *                   <br> Example: ISO-8859-2.
	 */
	static QString encodeBase64(const QString& str, const char* codec_name = 0);
	
	/**
	 * Same as above but works on char* types.
	 */
	static char* encodeBase64(const char* str);
	
	/**
	 * Same as above but also for data that main contain 0s.
	 */ 
	static char* encodeBase64Data(const char* str, uint len);
	
	static QString encodeBase64Data(const QByteArray& barr);
	
	/**
	 * Encodes string for compatibility with mail header (RFC 2047).
	 * String is divided and encoded using Base64 or Q algorithm.
	 * You can explicitly say witch coding you want to use using <code>enc</code> parameter.
	 * Otherwise the decision is done automatically. For strings that need to encode more characters 
	 * Base64 is used. Consult code for details.
	 * @param str String to be encoded
	 * @param codec_name Name of codec used to convert @ref QString to <code>char*</code>. 
	 *                   <br> Example: ISO-8859-2.
	 * @param enc Char used to set encoding technique.
	 *            <br> 'c' -- choose automatically (default)
	 *            <br> 'b' -- Base64
	 *            <br> 'q' -- Q method
	 */
	static QString encodeMailHeader(const QString& str, const char* codec_name = 0, const char enc = 'c');
	/**
	 * Encodes string using Q algorithm (RFC 2047).
	 * @param str String to be encoded
	 * @param codec_name Name of codec used to convert @ref QString to <code>char*</code>. 
	 *                   <br> Example: ISO-8859-2.
	 */
	static QString encodeQ(const QString& str, const char* codec_name = 0, int in_body = 0);
	
	/**
	 * @param in_body Set to one if you are encoding body of message, 0 otherwise
	 */ 
	static char* encodeQ(const char* str, int in_body);
	
	/**
	 * Encodes string for using in mail header compatible with RFC 2047.
	 * Do not this method directly, use @ref encodeMailHeader instead.
	 * @param str String to be encoded
	 * @param enc Char used to set encoding technique.
	 *            <br> 'b' -- Base64
	 *            <br> 'q' -- Q method
	 * @param codec_name Name of codec used to convert @ref QString to <code>char*</code>. 
	 *                   <br> Example: ISO-8859-2.
	 */
	static QString encodeHeaderQB(const QString& str, const char enc, const char* codec_name = 0);
	
	/**
	 * Tests string for using in mail header compatible with RFC 2047.
	 * @return How many characters need to be encoded.
	 */
	static uint testQuoteHeader(const QString& str);
	
	/**
	 * Converts @ref QString to @ref QString using given codec.
	 * @param str String to be converted
	 * @param codec_name Name of codec used to convert @ref QString to <code>char*</code>. 
	 *                   <br> Example: ISO-8859-2.
	 */
	static QString getCString(const QString& str, const char* codec_name = 0);
	
	/**
	 * Converts @ref QString to @ref QString using given codec.
	 * @param str CString to be converted
	 * @param codec_name Name of codec used to convert @ref QString to QString.
	 *                   <br> Example: ISO-8859-2.
	 */
	static QString fromCString(const QString& str, const char* codec_name = 0);
	
	/**
	 * @return String with 'Boundary--'+random text
	 */
	static QString getBoundaryString();
	
	/**
	 * Method used to determine content type of the file (like 'application/pdf').
	 * For now it uses matching file extensions with content types 
	 * (defined in file mime-types.h).
	 * <br>
	 * In future will also test content of file.
	 * @return Mime content type (like 'application/pdf')
	 * @param fname Full name with path
	 */ 
	static const char* getFileContentType(const QString& fname);
	
	/**
	 * Decodes data of field in mail header. Used by @ref LHMailHeader::parseLine.
	 */ 
	static QString decodeMailHeader(const QString& str);
	
	/**
	 * Decodes quoted mail body
	 */ 
	static QString decodeMailBody(const QString& str);
	
	/**
	 * Decodes data encoded by base64 algorithm
	 */ 
	static QByteArray decodeBase64(const QString& str);
	
	/**
	 * Decodes data+time data from mail header
	 */ 
	static QDateTime decodeRfcTime(const QString& date);
	
	/**
	 * Encodes @ref QDateTime to string for mail header
	 */ 
	static QString encodeRfcTime(const QDateTime& date);
	
	/**
	 * Returnes name of application that is assigned to given 
	 * mime type (like image/gif)
	 */ 
	static QString getAppForMimeType(const QString& content_type);
	
	/**
	 * Method used to assign application to given mime type (like image/gif)
	 */ 
        static void setAppForMimeType(const QString& content_type, const QString& app);
	
private:
//	LHMimePrivate *d;
		
};


#       endif

