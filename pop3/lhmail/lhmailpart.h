
/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAILPART_H
#define LHMAILPART_H

#include "lhmailbase.h"

#ifndef LHMP_CONST 
#define LHMP_CONST 

#define LHMP_CONVERT_CRLF     0x0001

#endif


class QString;
class LHMailPartPrivate;
class LHMailHeader;
class LHMail;


/**
 * This class represents one part of multipart mail.
 * Usually you should use this class through @ref LHMail::attachFile
 * Example:
 * <pre>
 * LHSmtp* smtp = new LHSmtp("127.0.0.1"); //server address
 * LHMail* mail = new LHMail("me@lefthand.pl", "you@lefthand.pl", 
 *                           "test", "test mail", 
 *                           1);  //delete after send
 * 
 * LHMailBase part = mail->attachFile(file_name);
 * if (part == NULL)
 * {
 *    //some problems with the file
 * }
 * mail->send();

 * LHMail
 * </pre>
 *
 * @short Representation of one part of multipart mail.
 * @see LHMail
 * @see LHMail::attachFile
 * 
*/

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHMailPart : public LHMailBase
{
    Q_OBJECT

public:
	
	/**
	 * Constructor.
	 * @param parent Mail or mail part conatining this part.
	 */ 
	LHMailPart(LHMailBase* parent);
	
	virtual ~LHMailPart();
	
	/**
	 * @return Size of file if file is valid, -1 otherwise
	 * @param flags Flags, you can combine them using ||:
	 *              <p> LHMP_CONVERT_CRLF -- conver unix end of line LF to windows CRLF
	 */ 
	virtual int setFile(const QString &file_name, int flags = 0);
	
	/**
	 * @return True is success, False otherwise
	 * @param mail Mail to be changed into attachment
	 */ 
	virtual bool setMail(LHMail* mail);
	
	/**
	 * Used to set string as content of message part.
	 * In this case disposition is set to inline.
	 */
	virtual void setString(const QString &content);
	
	/**
	 * Used to set Html as content of message part.
	 * In this case disposition is set to inline.
	 */
	virtual void setHtml(const QString &content);

        /**
         *  Sets S/MIME signature as body of this mail part.
         *  \see LHMail::addSignature
         */

        virtual void setSignature (const QString &content);

        /**
         *  Sets passed data as it is, without any additional data or modification.
         */

        virtual void setBodySendData (const QString &content);
	
	/**
	 * Generaly you do not have to use this function.
	 * @ref LHMail::attachFile doeas this test automatically.
	 * @return Size of file if file is valid, -1 otherwise
	 */ 
	virtual int isValid(const QString &file_name);
	
	/**
	 * Overloaded method used for retreiving data to be sent.
	 * Do not use thi smethod directly
	 */ 
	//virtual QString getBodySendData();
	
	/**
	 * Creates HTML that is showed to user. This method is used by LHMailBrowser.
	 * If you want to change the way mail is presented to user, you should change this
	 * method and @ref LHMail::getShowText.
	 * @param EMBEDDED True if this html is embedded in larger document.
	 */ 
	virtual QString getShowText(bool EMBEDDED= FALSE, bool advancedLook = true);
	
	QString getBodySendData();
	
protected:
	void encodeContent();
	void setContentType();
	
private:

	LHMailPartPrivate* d;
};

#endif
