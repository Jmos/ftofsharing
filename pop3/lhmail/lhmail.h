/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAIL_H
#define LHMAIL_H
#define UNIT_API __declspec(dllexport)
#include "lhmailbase.h"
#include "lhmaildefs.h"

class QString;
class QDateTime;
class LHMailPrivate;
class LHMailAddr;
class LHMailHeader;
class LHMailPart;
class LHSmtp;
class QTextStream;
class LHMailSignature;

#ifndef LHMP_CONST 
#define LHMP_CONST 

#define LHMP_CONVERT_CRLF     0x0001

#endif

/**
 * This class represents single mail to be send by @ref LHSmtp.
 * Example:
 * <pre>
 * LHSmtp* smtp = new LHSmtp("127.0.0.1"); //server address
 * LHMail* mail = new LHMail("me@lefthand.pl", "you@lefthand.pl", 
 *                           "test", "test mail", 
 *                           1);  //delete after send
 * mail->send();
 * //mail is automatically deleted after send
 * //smtp should be deleted later when sending is finished
 * </pre>
 *
 * LHSmtp sends mails in asynchronous way, so we can not simply delete objects.
 * We can use signal @ref LHSmtp::sentMail to check if mail was sent.
 * More in @ref LHSmtp documentation.
 *  
 * @short Representation of single mail to be send.
 * @see LHSmtp
 * 
*/

#       ifdef Q_OS_WIN32
	UNIT_API
#       endif

class LHMail : public LHMailBase
{
    Q_OBJECT

public:
	/**
	 * Constructs a LHMail object. This is the only legal way to construct LHMail object.
	 * @param from Sender mail address
	 * @param to List of mail addresses separated by ',' or ';'. 
	 *           Can be in form <code>Long name &lt;name@domain&gt;</code>. 
	 *           <p>Example: 
	 *           <p><code>John He &lt;jhe@lefthand.pl&gt;; Katia She &lt;kshe@lefthand.pl&gt, other@lefthand.pl</code>
	 * @param subject Subject of mail
	 * @param body Body of mail in text format
	 * @param del If 1 then object is automatically deleted by @ref LHSmtp after sending mail
	 * @param cc List of mail addresses used as CC (same format as to)
	 * @param bcc List of mail addresses used as BCC (same format as to)
	*/

	LHMail( const QString &from, const QString &to,
			const QString &subject,
			const QString &body, int del = 0,
			const QString &cc =  QString::null,
			const QString &bcc =  QString::null);
	
        /**
         *  Copying constructor;
         */

	// LHMail (const LHMail &copyMe);
	                
	LHMail(LHMailBase* parent = 0);
	
	virtual void init( const QString &from, const QString &to,
			const QString &subject, const QString &body,
			const QString &cc, const QString &bcc);

        virtual ~LHMail();
	
	/**
	 * returns <code>from</code> text to be put in message header
	 */
	virtual QString from();
	
	/**
	 * returns <code>to</code> text to be put in message header
	 */

	virtual QString to();
	virtual void setTo (const QString &s);
	
	/**
	 * returns <code>subject</code> text to be put in message header
	 */
	virtual QString subject();
	
	/**
	 * returns <code>cc</code> text to be put in message header
	 */
	virtual QString cc();
	
	/**
	 * returns <code>bcc</code> data
	 */
	virtual QString bcc();

	/**
	 * returns <code>date</code> to be put in message header
	 */
	virtual QDateTime date();
	
	/**
	 * returns body of mail converted to specified encoding without headers.
	 */
	virtual QString getBodySendData();
	
	/**
	 * Returns 1 if message should be automatically deleted after sending by @ref LHSmtp.
	 */
	virtual int deleteAfterSend();
	
	/**
	 * Set 1 if message should be automatically deleted after sending by @ref LHSmtp.
	 */
	virtual int setDeleteAfterSend(int del);

	virtual void buildRcpt();
	
	/**
	 * Returns simple mail address from the list at <code>index</code> position.
	 * It is string in format <code>name@domain</code>.
	 * The list contains all mail addresses from <code>to, cc, bcc</code>.
	 */
	virtual QString getRcpt(uint index);
	
	/**
	 * Returns @ref LHMailAddr object from the list at <code>index</code> position.
	 * The list contains all mail addresses from <code>to, cc, bcc</code>.
	 */ 
	virtual LHMailAddr* getRcptMailAddr(uint index);
	
	/**
	 * Returns number of mail addresses on the list.
	 * The list contains all mail addresses from <code>to, cc, bcc</code>.
	 */ 
	virtual uint countRcpt();

	/**
	 * This method is called by @ref LHSmtp after the mail is sent.
	 * Generally it emits signal @ref LHSmtp::sentMail.
	 */
	void sent(LHSmtp* smtp);
	
	/**
	 * Attaches given file.
	 * @return Attached mail part or NULL if problem occured
	 * @param fname Name of file to be attached
	 * @param flags Flags, you can combine them using ||:
	 *              <p> LHMP_CONVERT_CRLF -- conver unix end of line LF to windows CRLF
	 */
	virtual LHMailBase* attachFile(const QString& fname, int flags = 0);
	
	/**
	 * Attaches given mail. This way mails are forwarded.
	 * @return Attached mail part or NULL if problem occured
	 * @param mail Mail to attached (forwarded)
	 */
	virtual LHMailBase* attachMail(LHMail* mail);
	
	/**
	 * Attaches html content to mail.
	 * @return Attached mail part or NULL if problem occured
	 * @param html_data Html code to be attached, probably html version of body
	 */
	virtual LHMailBase* attachHtml(const QString& html_data);

        /**
         *  Digitally sign this mail message. Use this method with caution.
         *  You must be aware, that this method will modify your LHMailSignature
         *  object passed as an argument. Signature mus be he last attached part in
         *  multipart message.
         */

        virtual LHMailBase* attachSignature (LHMailSignature &);
        
	/**
	 * Attaches given part.
	 * Do not use this method directly!
	 * Use @ref LHMail::attachFile, or LHMail::attachMail
	 * @return true if attached false otherwise
	 * @param mp Mail part to attached (forwarded)
	 */
	bool attachPart(LHMailBase* mp);
	
	/**
	 * NOT TESTED
	 */ 
	//virtual int removeAttachment(LHMailBase* att);
	
	/**
	 * Sets initial values for content-type->application map
	 * and some settings about mail headers.
	 * Must be called before using this library
	 */ 
	static void initMailSystem();
	
	/**
	 * Creates HTML that is showed to user. This method is used by LHMailBrowser.
	 * If you want to change the way mail is presented to user, you should change this
	 * method and @ref LHMailPart::getShowText.
	 * @param EMBEDDED True if this html is embedded in larger document.
         * @param advancedLook If set to true, this method will return message body with preceding description of
         * email message. Description contains information such as Sender, Rcpt and subject.
	 */ 
	virtual QString getShowText(bool EMBEDDED= FALSE, bool advancedLook = true);
	
private:

	LHMailPrivate* d;
};

#endif
