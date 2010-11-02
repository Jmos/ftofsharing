/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAILBASE_H
#define LHMAILBASE_H

#include <QObject>
#include <QDebug>

class LHMailBasePrivate;
class LHMailAddr;
class LHMailHeader;
class LHMailPart;
class LHSmtp;

class QString;
class QDateTime;
class QTextStream;
class QImage;


#ifndef LHMP_CONST 
#define LHMP_CONST 

#define LHMP_CONVERT_CRLF     0x0001

#endif

/**
 * This is base class for @ref LHMail and @ref LHMailPart
 * This is pure virtual class and you can not se it directly.
 * 
 * @short Base class for @ref LHMail and @ref LHMailPart
 * @see LHMail
 * @see LHMailPart
 * 
*/

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHMailBase : public QObject
{
    Q_OBJECT

public:
		
	/**
	 * Constructor
	 * @param parent For LHMailPart LHMail conatining this part
	 */ 
	LHMailBase(LHMailBase* parent = 0);
        LHMailBase (const LHMailBase &copyMe);

        virtual ~LHMailBase();
	
	/**
	 * @return Text representing message body
	 */
	virtual QString messageBody();
	
	/**
	 * Sets message body
	 */
	virtual void setMessageBody(const QString& body);
	
	/**
	 * @return encoding of mail or mail part (like ISO-8859-2)
	 */
	virtual QString encoding();
	
    /**
	 * Used to set encoding of mail (like ISO-8859-2). 
	 * On Unix you generally do not have to set encoding explicitly --
	 * encoding for Locale is used. 
	 * <br>
	 * On Windows machines you should set this.
	 * @param encod String representing encoding like ISO-8859-2
	 */
	virtual int setEncoding(const QString& encod);
 
	/**
	 * @return @ref LHMailHeader of this mail
	 */
	virtual LHMailHeader& header();

	/**
	 * This method is called by @ref LHSmtp to send this message.
	 * You should not use this method directly.
	 */
	virtual void send(QTextStream* ts);

	/**
	 * Used to check if mail or mail part is single or multipart.
	 * @return True if mail is not multipart
	 */
	virtual bool isSinglePart();
	
	/**
	 * @return True if mail or mail part was received and parsed
	 */ 
	bool isParsed();

	/**
	 * @return Encoded @ref messageBody ready for sending
	 */ 
	QString getBodySendDataForText();
	
	/**
	 * Used to set data for received mails
	 * Next data is parsed devided to header and body
	 * This data is converted according to setting in included headers
	 */ 
	virtual void setData(const QString& mail_data);
	
	/**
	 * Parses mail data set by @ref setData
	 * First calls parse for @ref LHMailHeader
	 * then parses body @ref parseBody
	 */ 
	virtual void parseData(const QString& ts);
	
	/**
	 * Parses body of mail or mail part
	 */ 
	virtual void parseBody(const QString& ts);

        /**
         *  Format of data to be built by buildSendData, and returned by getSendData.
         */

        enum Format { FullMessage, DataToSign };
	
	/**
	 * Returnes data to be send or received data,
	 * If send data is not valid @ref buildSendData
	 * is called first
	 */ 
	virtual QString getSendData (Format f = FullMessage);

	/**
	 * Used to set send data, you should not change this method, rather
	 * @ref buildSendData, or @ref getBodySendData
	 */ 
	virtual void setSendData(const QString& send_data);
        
	/**
	 * Method called just before sending data
	 * In implementation send data is prepared
	 */ 
	virtual void buildSendData (Format f = FullMessage);
	
	/**
	 * Pure virtual method that must be implemented by specialized classes
	 * @return data to be send without header
	 */ 
	virtual QString getBodySendData() = 0;
	
	/**
	 * @return Number of parts
	 */ 
	virtual uint getPartCount();
	
	/**
	 * @return Part of given index, or NULL id index is out of range
	 */ 
	virtual LHMailBase* getPart(uint index);
	
	/**
	 * Adds mail part to mail or mail part. Do not use this method
	 * directly. Use @ref LHMail::attachFile instead.
	 * @return Part count
	 */ 
	virtual uint addPart(LHMailBase* mp);
	
	/**
	 * Looks for position of part in part collection
	 * @return Index in collection or -1 if not found
	 */ 
	virtual int findPart(LHMailBase* mp);
	
	/**
	 * Removes mail part to mail or mail part. Do not use this method
	 * directly. Use @ref LHMail::removeAttachment instead.
         * If mp is set to NULL, all parts will be removed.
	 * @return true if removed false if not found
	 */ 
	virtual bool removePart (LHMailBase* mp);
	
	/**
	 * @return Raw data for attachment, set or received
	 */ 
	virtual QByteArray getRawData();
	
	/**
	 * Used to set raw data for attachment. Do not use directly.
	 */ 
	virtual void setRawData(const QByteArray& data);

	/**
	 * @return True if mail contains text, ie. if Content-Type is text
	 */ 
	virtual bool isText();
	
	/**
	 * @return True if this boundary can be used for this mail or mail part
	 */ 
	virtual bool testBoundary(const QString& bound);
	
	/**
	 * @return Address of mail or mailpart in part tree. This address is unique in mail
	 * And can be used to find this part or mail using @ref getFromAddress
	 */ 
	virtual QString getAddress();

	/**
	 * Method used to find mail or part in mail using address rceived from @ref getAddress.
	 * @param parent The top most mail
	 * @param address String obtainde by @ref getAddress
	 * @return Mail o part if success NULL otherwise
	 */ 
	virtual LHMailBase* getFromAddress(LHMailBase* parent, const QString& address);
	
	/**
	 * Pure virtual method returning HTML that is showed to user.
	 * Reimplemented in @ref LHMail::getShowText and @ref LHMailPart::getShowText
	 */ 
	virtual QString getShowText(bool EMBEDDED= FALSE, bool advancedLook = true) = 0;
	
	/**
	 * @return Value of Content-Type header
	 */ 
	virtual QString contentType();
	
	/**
	 * @return Approximate of used memory by this mail or part
	 */ 
	virtual uint getMemSize();
	
	/**
	 * Cleans temp files and memory used by this mail or part
	 */ 
	virtual void cleanTemp();
	
	/**
	 * Save content of part in temp file. You can clean this using @ref cleanTemp.
	 * Original file name is used and is put into unique directory.
	 * @return Full path to temp file.
	 */ 
	virtual QString saveInTemp();
	
	/**
	 * Used by parts containing graphics to create temporary mime source
	 * @return Name of mime source (unique in mail)
	 */ 
	virtual QString makeMimeSourceImg(QImage imp);
	
protected:
	
	virtual bool isSendDataValid();
	
	virtual void setSendDataValid(bool VAL = TRUE);
		
private:

	LHMailBasePrivate* d;
};

#endif
