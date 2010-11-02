/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmailbase.h"
#include "lhmailaddr.h"
#include "lhmime.h"
#include "lhsmtp.h"
#include "lhmailpart.h"
#include "lhmailheader.h"
#include "lhhtmltools.h"
#include "lhfiletools.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h> 
#include <qapplication.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qimage.h>

class LHMailBasePrivate {
public:

	LHMailBasePrivate(LHMailBase *_owner, LHMailBase *_parent) :
	owner(_owner),
	parent(_parent),
	text_body(),
	mail_parts(),
	header(),
	send_data(),
	raw_data(),
	is_parsed( FALSE ),
	index(0),
	is_send_data_valid( FALSE ),
	mime_source(),
	temp_file() {
		init ();
	};

/*--------------------------------------------------------------------------*/

        LHMailBasePrivate (const LHMailBasePrivate &copy);

/*--------------------------------------------------------------------------*/

	void init () {
		QString encoding = "ISO-8859-1";
		/*QTextCodec* cod = QTextCodec::codecForLocale();
		if (cod != NULL)
		{
			encoding = QString::fromLatin1(cod->name());
		}*/

		header.setParameter("Content-Type", "charset", encoding);

		//mail_parts.setAutoDelete(true);
	}

/*--------------------------------------------------------------------------*/

	LHMailBase* owner;
	/**
	 * Mail or MailPart that contains this part, NULL for mails
	 */ 
	LHMailBase* parent; 
	QString text_body;
	QList<LHMailBase *> mail_parts;
	LHMailHeader header;
	/**
	 * Text that is sent by SMTP, or received with header
	 */ 
	QString send_data;
	/**
	 * Content of attached filed
	 */ 
	QByteArray raw_data;

	bool is_parsed;
	
	uint index;
	
	bool is_send_data_valid;
	
	QString mime_source;
	QString temp_file;
};

/****************************************************************************/

LHMailBasePrivate::LHMailBasePrivate (const LHMailBasePrivate &copy)
{

        //??? Tak sie nie da.... No wiec jak?
        /*
        for (QPtrList <LHMailBase>::const_iterator i = copy.mail_parts.begin (); i != copy.mail_parts.end (); i++)
                mail_parts.append (new LHMailBase (*i));
        */
        
        // ?
        owner =               copy.owner;
        parent =              copy.parent;

        header =              LHMailHeader (copy.header);
        text_body =           copy.text_body;
        send_data =           copy.send_data;
        raw_data =            copy.raw_data;
        is_parsed =           copy.is_parsed;
        index =               copy.index;
        is_send_data_valid =  copy.is_send_data_valid;
        mime_source =         copy.mime_source;
        temp_file =           copy.temp_file;

        init ();
}

/****************************************************************************/

LHMailBase::LHMailBase (LHMailBase* parent)
{
	d = new LHMailBasePrivate (this, parent);
}

/****************************************************************************/

LHMailBase::LHMailBase(const LHMailBase &copyMe) : QObject()
{
	d = new LHMailBasePrivate(*copyMe.d);
}

/****************************************************************************/

LHMailBase::~LHMailBase()
{
	delete d;
}

/****************************************************************************/

QString LHMailBase::messageBody ()
{

	if (isSinglePart ())
                return d->text_body;

        else {

                QString ret;
                int pc = getPartCount ();
                
		for (uint i = 0; i < pc; i++)
		{
			LHMailBase* mp = getPart (i);

                        if (mp->header ().testData ("Content-Type", "text/plain"))
			        ret += mp->messageBody ();
		}
                
                return ret;
        }

}

/****************************************************************************/

void LHMailBase::setMessageBody(const QString& body)
{
	setSendDataValid( FALSE );
	
	d->text_body = body;
};


bool LHMailBase::isSinglePart()
{
	return header().isSinglePart();
}

LHMailHeader& LHMailBase::header()
{
	return d->header;
};

QString LHMailBase::encoding()
{
	return header().getParameter("Content-Type", "charset");
};

int LHMailBase::setEncoding(const QString& encod)
{
	setSendDataValid( FALSE );
	header().setParameter("Content-Type", "charset", encod);
	header().setEncoding(encod);
	return 0;
}

bool LHMailBase::testBoundary(const QString& bound)
{
	QString sd = getSendData();
	if (sd.contains(bound))
	{
		return false;
	}
	
	uint pc = getPartCount();
	
	for (uint i=0; i<pc; i++)
	{
		if (!getPart(i)->testBoundary(bound))
		{
			return false;
		}
	}
	
	return true;
}

bool LHMailBase::isParsed()
{
	return d->is_parsed;
}

bool LHMailBase::isText()
{
	bool r = FALSE;
	
	QString cnt_type = header().getData("Content-Type");
	
	if (cnt_type.contains("text"))
	{
		r= TRUE;
	}
		
	return r;
}

/**
 * helper function returning length of the longest line ended by \r\n
 */
 
uint longestLine(const QString cstr)
{
	int start_line = 0;
	int end_line = 0;
	uint ret = 0;
	while (end_line >= 0)
	{
		uint llen = 0;
		end_line = cstr.indexOf("\r\n", start_line);
		
		if (end_line <0 )
		{
			llen = cstr.length() - start_line;
		}
		else
		{
			llen = end_line - start_line;
			start_line = end_line+1;
		};
		
		if (llen>ret)
		{
			ret=llen;
		};
	}
	
	return ret;
}

#define MAX_LINE_LEN 75

QString breakLongLines(const QString& in, uint max_len)
{
	QString ret;
	/*
	
	uint in_size = in.length();
	uint out_size = in_size + (in_size/max_len)*3 +1;
	
	const QChar *line_sep("=\r\n");
	
	ret.resize(out_size);
	ret.fill(0, out_size);
	
	QChar* c_ret = ret.data();
	const QChar* c_in = &in;
		
	int start_line = 0;
	int end_line = 0;
	
	int out_off = 0;
	
	uint start = 0;
	uint step = 0;
	
	bool more = true;
	
	while (more)
	{
		uint llen = 0;
		end_line = in.indexOf("\r\n", start_line);
		if (end_line <0 )
		{
			more = false;
			end_line = in_size;
		}
		else
		{
			end_line += 2;
		};
		
		llen = end_line - start_line;
		
		if (llen>max_len)
		{
			//line is too long
			start = start_line;
			step = max_len;
			while (start+step < end_line)
			{
				qDebug("breakLongLines, piece of long line, out_off: %d, start: %d, step: %d", out_off, start, step);
				if (*(c_in + start + step -1) == '=')
				{
					step--;
				}
				if (*(c_in + start + step -2) == '=')
				{
					step-=2;
				}
				qmemmove(c_ret + out_off, c_in + start, step);
				out_off += step;
				start += step;
				qmemmove(c_ret + out_off, line_sep, 3);
				out_off += 3;
			};
			if (end_line >= 0)
			{
				llen += 2;
			};
			step = end_line - start;
			qDebug("breakLongLines, last piece of long line, out_off: %d, start: %d, llen: %d, step: %d", out_off, start, llen, step);
			qmemmove(c_ret + out_off, c_in + start, step);
			out_off += step;
		}
		else
		{
			//line is short so we copy it with \r\n on the end
			qmemmove(c_ret + out_off, c_in + start_line, llen);
			out_off += llen;
		};
		
		start_line = end_line;
	};
	*/
	return ret;
}

/****************************************************************************/

QString LHMailBase::getBodySendDataForText()
{
	QString send_body;
	
	const char* codec_name = encoding().toLatin1(); //"ISO-8859-2"; //?
	
	if (header().testData("Content-Transfer-Encoding", "quoted-printable"))
	{
		send_body = LHMime::encodeQ( messageBody(), codec_name, 1);// + "\n"; // Tego nie powinno tu byc. Trzeba przeniesc.
		send_body.replace("\n", "\r\n");
		send_body.replace("\n.", "\n=2E");
		
		//check for long lines
		uint long_line = longestLine(send_body);
		
		// qDebug("LHMailBase::getBodySendDataForText, longest line: %u", long_line);
		
		if (long_line > MAX_LINE_LEN)
		{
			send_body = breakLongLines(send_body, MAX_LINE_LEN);
		}

                // send_body += ".\r\n";
	}
	else
	{
		qDebug()<<"Unsupported Content-Transfer-Encoding: "<<header().getData("Content-Transfer-Encoding").toLatin1();
	}
	
	return send_body;
};

/****************************************************************************/

void LHMailBase::buildSendData (Format f)
{
	if (isParsed() || isSendDataValid() )
	{
		//do nothing if mail or mailpart was received
		return;
	}
	else
	{
                QString sd;
                
                if (f != DataToSign)
                        sd = header().getSendData ();
                //else
                //       sd = header ().getSendData ("Content-Type");

		// if (isSinglePart())
                if (!getPartCount () || getPartCount() == 1)
		{
			sd += getBodySendData();
                        // sd += "\r\n";
		}
		else
		{
			
			const char* boundary = header().getParameter("Content-Type", "boundary").toAscii();
			uint b_len = qstrlen(boundary);

			uint pc = getPartCount();
                        bool init = false;
			
			for (uint i = 0; i< pc; i++)
			{
				LHMailBase *part = getPart(i);

                                // if (((f == DataToSign) && init) || (f != DataToSign)) {
           				sd += "--";
            			 	sd += boundary;
            				sd += "\r\n";
                                // }

                                init = true;
				part->buildSendData();
				sd += part->getSendData();

                                //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                                //qDebug (sd);
                                //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                                
				sd += "\r\n";
			}
			sd += "--";
			sd += boundary;
			sd += "--\r\n";

		}
		setSendData(sd);
		setSendDataValid( TRUE );
	}
}

void LHMailBase::send(QTextStream* ts)
{
	QString sd(getSendData());
	qDebug()<<sd;
	*ts<<sd;
}

void LHMailBase::setData(const QString& data)
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailBase::setData()");

	setSendData(data);
	parseData(data);
	setSendDataValid( TRUE );
	d->is_parsed = TRUE;
}

void LHMailBase::parseData(const QString& data)
{
	//const char *debugPtr = (const char *)data;

	uint offset = header().parse(data);

	QString body_data = data.mid(offset);

	//const char *debugPtr2 = (const char *)body_data;

	parseBody(body_data);
}

/****************************************************************************/

QString LHMailBase::getSendData (Format f)
{
	if (!isSendDataValid ())
		buildSendData (f);

	return d->send_data;
}

/****************************************************************************/

void LHMailBase::setSendData(const QString& data)
{
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailBase::setSendData()");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "~~~~~~~");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, (const char *)data);

	d->send_data = data;
}

void LHMailBase::parseBody(const QString& body_data)
{
	QByteArray cstr_body;

        // Debugging purpose only
        // const char *debugPtr = (const char *)body_data;

	if (isSinglePart())
	{
		if (header().testData("Content-Transfer-Encoding", "quoted-printable"))
		{
			cstr_body = LHMime::decodeMailBody(body_data).toAscii();
			setRawData(cstr_body);
		}
		else if (header().testData("Content-Transfer-Encoding", "base64"))
		{
			setRawData(LHMime::decodeBase64(body_data));
			if (isText())
			{
				cstr_body = qstrdup(d->raw_data.data());
			}
		}
		else
		{
			cstr_body = body_data.toAscii();
			setRawData(cstr_body);
		}

		if (isText())
		{
			d->text_body = LHMime::fromCString(cstr_body, encoding().toLatin1());
		}

                // BUG 762
                if (d->parent)
		        setSendData(body_data);
		
	}
	else
	{
		//qDebug("MULTIPART MAIL");
		QString bound = QString::fromLatin1("--") + header().getParameter("Content-Type", "boundary");
		uint bound_len = bound.length();

		//qDebug("BOUNDARY: '%s'", bound.latin1());
		int start = body_data.indexOf(bound);
		int end = 1;

		while (1)
		{
			start += bound_len;
			if (body_data[start] == '\r')
				start++;
			if (body_data[start] == '\n')
				start++;

			end = body_data.indexOf(bound, start);
			if (end < 0)
			{
				break;
			}
			LHMailPart* mp = new LHMailPart(this);
			QString part_data = body_data.mid(start, end - start);
			//qDebug("PART DATA: ---\n'%s'---\n", (const char*) part_data);
			mp->setData(part_data);
			addPart(mp);
			start = end;
		}
	}
}


uint LHMailBase::getPartCount()
{
	return d->mail_parts.count();
}

LHMailBase* LHMailBase::getPart(uint index)
{
	return d->mail_parts.at(index);
}

/****************************************************************************/

uint LHMailBase::addPart(LHMailBase* mp)
{
	setSendDataValid( FALSE );
	d->mail_parts.append(mp);
	
	return d->mail_parts.count();
}

/****************************************************************************/

int LHMailBase::findPart(LHMailBase* mp)
{
	return d->mail_parts.indexOf(mp);
}

/****************************************************************************/

bool LHMailBase::removePart(LHMailBase* mp)
{
// Remove All parts
	if (!mp) {
		d->mail_parts.clear();
		setSendDataValid (false);
		return true;
	} else {
		int pos = d->mail_parts.indexOf(mp);
		if (pos >= 0)
		{
			d->mail_parts.removeAt(pos);
			setSendDataValid( false );
			return true;
		}
	}

	return false;
}

/****************************************************************************/

QByteArray LHMailBase::getRawData()
{
	return d->raw_data;
}

void LHMailBase::setRawData(const QByteArray& data)
{
	setSendDataValid( FALSE );
	
	d->raw_data = data;
}

QString LHMailBase::getAddress()
{
	QString out;
	
	if (d->parent)
	{
		int index = d->parent->findPart(this);
		if (index >= 0)
		{
			out = d->parent->getAddress()+"_"+QString("%1").arg(index);
		}
		else
		{
			qFatal("LHMailBase::getAddress, part not found in parent");
		}
	}
	else
	{
		out = QString::fromLatin1("0");
	}
	return out;
}

LHMailBase* LHMailBase::getFromAddress(LHMailBase* parent, const QString& address)
{
	QStringList al = address.split("_");
	uint count = al.size();
	LHMailBase* out = parent;
	//ommit first element it is parent
	for (uint i = 1; i<count; i++)
	{
		if (out == NULL)
		{
			qDebug()<<"LHMailBase::getFromAddress, out is NULL, address: "<<address.toAscii();
		}
		out = out->getPart(al[i].toUInt());
	}
	if (out == NULL)
	{
		qDebug()<<"LHMailBase::getFromAddress, out is NULL, address: "<<address.toAscii();
	}
	return out;
}

QString LHMailBase::contentType()
{
	return header().getData("Content-Type");
}

bool LHMailBase::isSendDataValid ()
{
	return d->is_send_data_valid && header().isSendDataValid();
}
	
void LHMailBase::setSendDataValid(bool VAL)
{
	if (isParsed() && !VAL)
	{
		qWarning("Changing data in parsed LHMailBase");
	}
	d->is_send_data_valid = VAL;
}

uint LHMailBase::getMemSize()
{
	uint ret = d->send_data.size() + d->raw_data.size();
	uint pc = getPartCount();
	for (uint i=0; i<pc; i++)
	{
		ret+= getPart(i)->getMemSize();
	}
	
	return ret;
}
	
void LHMailBase::cleanTemp()
{
	if (!d->temp_file.isEmpty())
	{
		LHFileTools::removeTemp(d->temp_file);
	}
	
	/*
	if (!d->mime_source.isEmpty())
	{
		QMimeSourceFactory::defaultFactory()->setData(d->mime_source, 0);
	}
	*/
	
	uint pc = getPartCount();
	
	for (uint i = 0; i<pc; i++)
	{
		LHMailBase* mp = getPart(i);
		mp->cleanTemp();
	}
}
	
QString LHMailBase::saveInTemp()
{
	QString short_fname = header().getFileName();
	d->temp_file = LHFileTools::writeTempFile( short_fname, getRawData());
	return d->temp_file;
}
	
QString LHMailBase::makeMimeSourceImg(QImage img)
{
	d->mime_source = getAddress();
	//QMimeSourceFactory::defaultFactory()->setImage( d->mime_source, img);
	return d->mime_source;
}
