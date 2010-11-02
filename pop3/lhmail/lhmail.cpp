/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmail.h"
#include "lhmailaddr.h"
#include "lhmime.h"
#include "lhsmtp.h"
#include "lhmailpart.h"
#include "lhmailheader.h"
#include "lhhtmltools.h"
#include "lhmailerr.h"
#include "lhmailsignature.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h> 
#include <qapplication.h>
#include <qtextcodec.h>
#include <qtextstream.h>

class LHMailPrivate
{
public:

        LHMailPrivate(LHMail *_owner, int _del):
	        owner(_owner),
	        del(_del),
	        rcpt_mails(),
	        bcc()
	{}
	
	LHMail* owner;
	int del;
	QList<LHMailAddr *> rcpt_mails;
	QString bcc;
};


LHMail::LHMail( const QString &from, const QString &to,
			const QString &subject,
			const QString &body,
			int del,
			const QString &cc,
			const QString &bcc) : LHMailBase(0)

{
	d = new LHMailPrivate(this, del);
	init(from, to, subject, body, cc, bcc);
}

LHMail::LHMail(LHMailBase* parent) : LHMailBase(parent)
{
	d = new LHMailPrivate(this, 0);
	init("","","","","","");
}

/*
LHMail::LHMail (const LHMail &copyMe) //:
        // LHMailBase (copyMe)
{

	//d->owner      = copyMe.d->owner;
	//d->del        = copyMe.d->del;
	//d->rcpt_mails = copyMe.d->rcpt_mails;
	//d->bcc        = copyMe.d->bcc;


        // Nie wiem jak to inaczej zrobic.
        setData (copyMe.getSendData ())
        
}
*/

void LHMail::init(const QString &from, const QString &to,
			const QString &subject,
			const QString &body,
			const QString &cc,
			const QString &bcc)
{
	setMessageBody(body);
	
	header().setData("From", from);
	header().setData("To", to);
	header().setData("Cc", cc);
	//of course we do not add bcc to header
	d->bcc = bcc;
	header().setData("Subject", subject);
	header().setDataDateTime("Date", QDateTime::currentDateTime() );
	header().setData("Content-Type", "text/plain");
	header().setData("Content-Transfer-Encoding", "quoted-printable");
	header().setData("MIME-Version", "1.0");
	
	buildRcpt();
}


LHMail::~LHMail()
{
	delete d;
}

QString LHMail::from()
{
	return header().getData("From");
};

QString LHMail::to()
{
	return header().getData("To");
}

/****************************************************************************/

void LHMail::setTo (const QString &s)
{
	header().setData("To", s);
	buildRcpt();
}

/****************************************************************************/

QString LHMail::subject()
{
	return header().getData("Subject");
};

QString LHMail::cc()
{
	return header().getData("Cc");
};

QString LHMail::bcc()
{
	return d->bcc;
};

QDateTime LHMail::date()
{
	return header().getDataDateTime("Date");
};

//it has to be QCString because of different encoding standards
QString LHMail::getBodySendData ()
{
	return getBodySendDataForText();
};


int LHMail::deleteAfterSend()
{
	return d->del;
}

int LHMail::setDeleteAfterSend(int del)
{
	int old = d->del;
	d->del = del;
	return old;
}

void LHMail::buildRcpt()
{
	setSendDataValid( FALSE );

	d->rcpt_mails.clear();
	LHMailAddr::parseMulti(d->rcpt_mails, to() );
	LHMailAddr::parseMulti(d->rcpt_mails, cc() );
	LHMailAddr::parseMulti(d->rcpt_mails, bcc() );
}
	
QString LHMail::getRcpt(uint index)
{
	LHMailAddr* ma = getRcptMailAddr(index);
	if (ma == NULL)
	{
		qWarning("LHMail::getRcpt, index out of range: %u", index);
		return "";
	}
	return ma->simple();
	
}

LHMailAddr* LHMail::getRcptMailAddr(uint index)
{
	return d->rcpt_mails.at(index);
}

uint LHMail::countRcpt()
{
	return d->rcpt_mails.count();
}

void LHMail::sent(LHSmtp* smtp)
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMail::sent");
        
	smtp->emitSentMail(this);
		
	if (deleteAfterSend())
	{
		this->deleteLater();
	}

}

LHMailBase* LHMail::attachFile(const QString& fname, int flags)
{
	setSendDataValid( FALSE );

	LHMailPart* mp = new LHMailPart(this);
	
	if (mp->setFile(fname, flags)>0)
	{
		if (attachPart(mp))
		{
			return mp;
		}
	}
	
	//not here if success
	delete mp;
	return 0;
}

/****************************************************************************/

LHMailBase* LHMail::attachMail(LHMail* mail)
{
	LHMailPart* mp = new LHMailPart(this);
	
	if (mp->setMail(mail))
	{
		if (attachPart(mp))
		{
			return mp;
		}
	}
	
	//not here if success
	delete mp;
	return 0;
}

/****************************************************************************/

LHMailBase* LHMail::attachHtml(const QString& html_data)
{
	LHMailPart* mp = new LHMailPart(this);	
	mp->setHtml(html_data);
        
	if (attachPart(mp))
	{
		header().setData("Content-Type", "multipart/alternative");
		return mp;
	}
	
	//not here if success
	delete mp;
	return 0;
}

/****************************************************************************/

LHMailBase* LHMail::attachSignature (LHMailSignature &signature)
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "12346789");

	LHMailPart* mp = new LHMailPart (this);

        if (getPartCount () > 1) {
                QString partsText = getSendData (DataToSign);
                QString newBoundary = LHMime::getBoundaryString ();
                QString oldBoundary = header ().getParameter ("Content-Type", "boundary");
                partsText.replace (oldBoundary, newBoundary);

#               if 1
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "***********************************************");
                qDebug()<<partsText;
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "***********************************************");
#               endif

                removePart (NULL);
		LHMailPart* mpb = new LHMailPart (this);
		addPart(mpb);

		mpb->header ().setData ("Content-Type", "Multipart/Mixed");
		mpb->header ().setParameter ("Content-Type", "boundary", newBoundary);
                mpb->setBodySendData (partsText);
		mpb->setEncoding (encoding());
                
        }

        /**
         *  Aply this LHMail object to the signature object. The body of mail
         *  is essntial when signing.
         */
         
	if (attachPart (mp))
	{

                // if (getPartCount () == 2 && body.isEmpty ())
                if (getPartCount () != 2) {
                        // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!2");
                        return NULL;
                }

                QString body = getPart (0)->getSendData ();
        
                signature.setBody (body);
                signature.load ();

#               if 1
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "***********************************************");
                qDebug()<<body;
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "***********************************************");
#               endif

		header ().setData ("Content-Type", "multipart/signed");
                header ().setParameter ("Content-Type", "micalg", "sha1");
                header ().setParameter ("Content-Type", "protocol", "application/x-pkcs7-signature");
                
	        mp->setSignature (signature.getData ());
                
		return mp;
	}
	
	//not here if success
	delete mp;
	return 0;
}

/****************************************************************************/

bool LHMail::attachPart(LHMailBase* mp)
{
	setSendDataValid( FALSE );

	if (isSinglePart())
	{
		//first we need to change body into one of parts
		LHMailPart* mpb = new LHMailPart(this);
		mpb->setEncoding(encoding());
		mpb->setString(messageBody());
		addPart(mpb);
		
                header().setData("Content-Transfer-Encoding", "7bit");
		header().setData("Content-Type", "Multipart/Mixed");
		header().removeParameter("Content-Type", "charset");
		header().setParameter("Content-Type", "boundary", LHMime::getBoundaryString());
	}
	
	//check boundary
	while(0)
	{
		QString bound = header().getParameter("Content-Type", "boundary");
		if (testBoundary(bound))
		{
			break;
		}
		else
		{
			header().setParameter("Content-Type", "boundary", LHMime::getBoundaryString());
		}
	}
	
	//then add file part
	addPart(mp);
	
	return true;
}

/*
bool LHMail::removeAttachment(LHMailBase* att)
{
	if (!removePart(att))
	{
		return false;
	}
	
	uint pc = getPartCount();
	//if only one left then this is single part mail
	//and we have to convert it
	if (pc == 1)
	{
		LHMailBase* mp = getPart(0);
		removePart(mp);
		
		header().setData("Content-Type", "text/plain");
		header().setParameter("Content-Type", "charset", encoding());
		header().removeParameter("Content-Type", "boundary");
		header().setData("Content-Transfer-Encoding", "quoted-printable");
	}
	
	return true;
}
*/

QString LHMail::getShowText(bool EMBEDDED, bool advancedLook)
{
	QString ret;

	if (!EMBEDDED)
	{
		ret+= "<HTML>\n";
	}

        if (advancedLook) {
	        ret+= "<h3>" + LHHtmlTools::full( subject() ) + "</h3>\n";
	        ret+= "From: " + LHMailAddr::convToHtmlMulti(from()) + "<br>\n";
	        ret+= "To: " + LHMailAddr::convToHtmlMulti(to()) + "<br>\n<p>\n";
        }

	if (isSinglePart())
	{
		if (/* advancedLook && */header().testData("Content-Type", "text/html"))
		{
			//QMimeSourceFactory::defaultFactory()->setText( "HTML_PART", messageBody() );
			//ret += "\n<br><A href=\"HTML_PART\">" + tr("HTML message") + "</A><br>\n";
			ret += "\n<br><A href=\"html://"+getAddress()+"\">" + tr("HTML message") + "</A><br>\n";
		}
		ret += LHHtmlTools::full(messageBody());
	}
	else
	{
		//ret+="<meta http-equiv=\"content-type\" content=\"text/html; charset=" + encoding() +"\" /> \n";
		uint pc = getPartCount();
		for (uint i=0; i<pc; i++)
		{
			LHMailBase* mp = getPart(i);
			ret+= mp->getShowText (false, advancedLook);
		}
	}
	if (!EMBEDDED)
	{
		ret+= "</HTML>\n";
	}
	
	return ret;
}

void LHMail::initMailSystem()
{
	LHMailHeader::addFieldParameter("Content-Type", "charset");
	LHMailHeader::addFieldParameter("Content-Type", "boundary");
	LHMailHeader::addFieldParameter("Content-Type", "name");
	LHMailHeader::addFieldParameter("Content-Type", "filename");
	LHMailHeader::addFieldParameter("Content-Type", "format");
	LHMailHeader::addFieldParameter("Content-Disposition", "filename");
	
	LHMime::setAppForMimeType("image/gif", "gqview");
	LHMime::setAppForMimeType("image/jpeg", "gqview");
	LHMime::setAppForMimeType("image/png", "gqview");
	LHMime::setAppForMimeType("image/tiff", "gqview");
	LHMime::setAppForMimeType("image/bmp", "gqview");
	
	LHMime::setAppForMimeType("application/msword", "oowriter");
	LHMime::setAppForMimeType("application/vnd.sun.xml.writer", "oowriter");
	
	LHMime::setAppForMimeType("text/x-c++src", "kate");

        LHMailError::init ();
        LHMailSignature::init ();
}

