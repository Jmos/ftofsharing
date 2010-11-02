/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmailpart.h"
#include "lhmail.h"
#include "lhmailheader.h"
#include "lhmime.h"
#include "lhhtmltools.h"

#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qstringlist.h>
#include <qimage.h>
#include <qdatetime.h>

class LHMailPartPrivate
{
public:
	LHMailPartPrivate(LHMailPart* _owner):
	owner(_owner),
	fname(),
	send_body(),
	data(),
	is_valid(0),
	short_fname(),
	flags(0)
	{
	  
	};
	
	LHMailPart* owner;
	QString fname;
	QString send_body;
	QByteArray data;
	int is_valid;
	QString short_fname;
	int flags;
	
	static int counter;
	
	//Todo: locking
	static int getCounter() { return counter++; };
};

int LHMailPartPrivate::counter = 0;

/****************************************************************************/

LHMailPart::LHMailPart(LHMailBase* parent) : LHMailBase(parent)
{
	d = new LHMailPartPrivate(this);
}

/****************************************************************************/

QString LHMailPart::getBodySendData()
{
        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailPart::getBodySendData ()");
	return d->send_body;
}

/****************************************************************************/

void LHMailPart::setBodySendData (const QString &content)
{
        d->send_body = content.toAscii ();
        /*
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "==================================================");
        qDebug (d->send_body);
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "==================================================");
        */
}

/****************************************************************************/

void LHMailPart::encodeContent()
{
	setSendDataValid( FALSE );

	QTime t;
	t.start();
	
	qDebug("Start encoding");
	
	if (isText() || header().testData("Content-Type", "message/rfc822"))
	{
		header().setData("Content-Transfer-Encoding", "quoted-printable");
		d->send_body = getBodySendDataForText();
		
		//if (d->flags && LHMP_CONVERT_CRLF) ??
	}
	else
	{
	/*	
		QByteArray barr = getRawData();
		
		//QTime t2;
		//t2.start();
	
		// qDebug("Start Base64");
		QString big_line = LHMime::encodeBase64Data(barr);
		// qDebug("Finished Base64, time: %d", t2.elapsed());
		
		uint size = big_line.length();
		uint start = 0;
		uint step = 72;
		uint bsize = size + (size/72)*2 +1;
		
		d->send_body.resize(bsize);
		d->send_body.fill(0, bsize);
		
		QChar *c_send_body = d->send_body.data();
		const char* c_big_line = big_line;

		uint asize = 0;
		while (start+step < size)
		{
			qmemmove(c_send_body + asize, c_big_line + start, step);
			asize += step;
			
			qmemmove(c_send_body + asize, "\r\n", 2);
			asize += 2;
			
			start += step;
		}
		qmemmove(c_send_body + asize, c_big_line + start, size - start);
		asize += size - start;
		asize++;
		c_send_body[asize] = 0;
		
		qDebug("asize: %u, bsize: %u", asize, bsize);
		
		header().setData("Content-Transfer-Encoding", "base64");
		*/
	}
	
	qDebug("Finished encoding, time: %d", t.elapsed());
}

void LHMailPart::setContentType()
{
	setSendDataValid( FALSE );

	const char* ctf = LHMime::getFileContentType(d->fname);
	QString cnt_type;
	if (ctf)
	{
		cnt_type = ctf;
	}
	else
	{
		cnt_type = "application/octet-stream";
	}
	
    header().setData("Content-Type", cnt_type);
}

QString getShortFileName(const QString& name)
{
	QFileInfo fi(name);
	QString shname = fi.fileName();
	return shname;
}

/****************************************************************************/

int LHMailPart::setFile(const QString &file_name, int flags)
{
	setSendDataValid( FALSE );

	int size = isValid(file_name);
	if (size >= 0)
	{
		QByteArray raw_data;
		
		d->flags = flags;
		d->fname = file_name;
		
		header().setData("Content-Disposition", "attachment");
		header().setParameter("Content-Disposition", "filename", getShortFileName(d->fname));
		
		QTime t;
		t.start();
		qDebug("Reading file");
		QFile f(d->fname);
		f.open(QFile::ReadOnly);
		raw_data = f.readAll(); //f.readBlock(d->content, d->size);
		f.close();
		qDebug("File read, time: %d", t.elapsed());
		
		setRawData(raw_data);
		
		setContentType();
		
		if (isText())
		{
			//char* cstr = (char*) malloc(raw_data.size()+1);
			//memmove(
			uint size = raw_data.size();
			raw_data.resize(size+1);
			raw_data[size] = 0;
			QString cnt = LHMime::fromCString( (const char *) raw_data, encoding().toLatin1());
			setMessageBody(cnt);
		}
		
		encodeContent();
	}
	return size;
}

/****************************************************************************/

void LHMailPart::setString(const QString &content)
{
	setSendDataValid( FALSE );

	setMessageBody(content);
	
	header().setData("Content-Disposition", "inline");
	header().setData("Content-Type", "text/plain");	
	encodeContent();
}

void LHMailPart::setHtml(const QString &content)
{
	setSendDataValid( FALSE );

	setMessageBody(content);
	
	//header().setData("Content-Disposition", "inline");
	header().setData("Content-Type", "text/html");
	
	encodeContent();
}

/****************************************************************************/

void LHMailPart::setSignature (const QString &content)
{
	setSendDataValid (FALSE);

        d->send_body = content.toAscii ();


	header().setData ("Content-Type", "application/x-pkcs7-signature");
        header ().setParameter ("Content-Type", "filename", "epodpis.p7s");

	header().setData ("Content-Transfer-Encoding", "base64");
        
        header().setData ("Content-Disposition", "attachment");
        header ().setParameter ("Content-Disposition", "filename", "epodpis.p7s");
}

/****************************************************************************/

bool LHMailPart::setMail(LHMail* mail)
{
	setSendDataValid( FALSE );
	
	d->send_body = mail->getSendData();
	
	header().setData("Content-Transfer-Encoding", "7bit");
	header().setData("Content-Disposition", "inline");
	header().setData("Content-Type", "message/rfc822");
	header().setData("Content-Description", mail->subject());
	
	//addPart(mail);
	
	return true;
}

int LHMailPart::isValid(const QString &file_name)
{
	QFileInfo finf(file_name);
	if (finf.exists() && finf.isReadable() && finf.isFile())
	{
		return finf.size();
	}
	return -1;
}

LHMailPart::~LHMailPart()
{
	delete d;
}
	
QString LHMailPart::getShowText (bool, bool)
{
	QString ret;
	QString cnt_type = contentType();
	
	if (isSinglePart())
	{
		if (isText())
		{
			if (cnt_type.contains("text/html"))
			{
				ret +="\n<br><A href=\"html://"+getAddress()+"\">" + tr("HTML message") + "</A><br>\n";
			}
			
			else
			{
				if (header().testData("Content-Disposition", "attachment"))
				{
					QString file_name = header().getFileName();
					ret += "\n<br><A href=\"part://"+getAddress()+"\">" 
						   + LHHtmlTools::clean(file_name) + "</A><br>\n";
				}
				ret += LHHtmlTools::full(messageBody());
			}
		}
		else if (cnt_type.contains("message/rfc822"))
		{
			LHMail *mm = new LHMail(this);
			addPart(mm);
			mm->setData( getSendData() );
			ret+= "\n<table border=\"1\"><tr><td>\n";
			ret+= "<small>"+tr("Embedded message") + "</small><BR>";
			//ret=tr("\n<br>---ORIGINAL MESSAGE---<br>\n");
			ret+=mm->getShowText( TRUE ); //embedded
			ret+= "\n</td></tr></table>\n";
		}
		else if (cnt_type.contains("image"))
		{
			QString file_name = header().getFileName();
			ret += "\n<br><A href=\"imag://"+getAddress()+"\">" + LHHtmlTools::clean(file_name) + "</A><br>\n";
			
			QString str(getRawData());
			QImage img(str);
			if (!img.isNull())
			{
				QString name = makeMimeSourceImg(img);
				ret += "\n<br><img source=\""+ name + "\"><br>\n";
			}
		}
		else
		{
			QString file_name = header().getFileName();
			ret += "\n<br><A href=\"part://"+getAddress()+"\">" + LHHtmlTools::clean(file_name) + "</A><br>\n";
		}
	}
	else
	{
		//ret+="<meta http-equiv=\"content-type\" content=\"text/html; charset=" + encoding() +"\" /> \n";
		uint pc = getPartCount();
		for (uint i=0; i<pc; i++)
		{
			LHMailBase* mp = getPart(i);
			ret+= mp->getShowText();
		}
	}

	return ret;
}





