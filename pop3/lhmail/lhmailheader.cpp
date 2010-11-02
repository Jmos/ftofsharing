/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmailheader.h"
#include "lhmime.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h> 
#include <qapplication.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qfileinfo.h>

/****************************************************************************/

typedef QMap<QString, QString> LHMailHeaderFieldMap;

/****************************************************************************/

class LHMailHeaderField {
public:
	LHMailHeaderField(): props(), data()
	{
	};
	
	LHMailHeaderField(const LHMailHeaderField& mhf): props(mhf.props), data(mhf.data)
	{
	};

	~LHMailHeaderField() 
	{
	};
	
	LHMailHeaderFieldMap props;
	QString data;
	
};

/****************************************************************************/

typedef QMap<QString, LHMailHeaderField> LHMailHeaderMap;

/****************************************************************************/

class LHMailHeaderPrivate{
public:

	LHMailHeaderPrivate () :
	        map(),
	        encoding(),
	        is_send_data_valid( FALSE ),
	        is_parsed( FALSE ),
	        send_data()
	{
                init ();
	}

/*--------------------------------------------------------------------------*/

	LHMailHeaderPrivate (const LHMailHeaderPrivate &copyMe)
        {
                copy (copyMe);
                init ();
        }

/*--------------------------------------------------------------------------*/

	LHMailHeaderPrivate &operator= (const LHMailHeaderPrivate &copyMe)
        {
                copy (copyMe);
                return *this;
        }

/*--------------------------------------------------------------------------*/

        void init () {

		encoding = "ISO-8859-1";
		QTextCodec* cod = QTextCodec::codecForLocale();

		if (cod != NULL)
			encoding =  QString::fromLatin1(cod->name());
        }

/*--------------------------------------------------------------------------*/

	LHMailHeaderMap map;
	QString encoding;
	
	static QMap<QString, QStringList> field_param_map;
	
	bool is_send_data_valid;
	bool is_parsed;
	
	QString send_data;

private:

        void copy (const LHMailHeaderPrivate &copy)
        {
                // ?
	        map = copy.map;
	        is_send_data_valid = copy.is_send_data_valid;
	        is_parsed = copy.is_parsed;
	        send_data = copy.send_data;
        }

};

/****************************************************************************/

QMap<QString, QStringList> LHMailHeaderPrivate::field_param_map;

/****************************************************************************/

LHMailHeader::LHMailHeader()
{
	d = new LHMailHeaderPrivate();
}

/****************************************************************************/

LHMailHeader &LHMailHeader::operator= (const LHMailHeader &copy)
{
        d = copy.d;
        return *this;
}

/****************************************************************************/

LHMailHeader::LHMailHeader (const LHMailHeader &copy)
{
        d = new LHMailHeaderPrivate (*copy.d);
}

/****************************************************************************/

LHMailHeader::~LHMailHeader()
{
	delete d;
}

void LHMailHeader::setData(const QString& field, const QString& data)
{
	setSendDataValid( FALSE );
	d->map[field].data = data;
}

QString LHMailHeader::getData(const QString& field)
{
	return d->map[field].data;
}

bool LHMailHeader::testData(const QString& field, const QString& data, bool cs)
{
	QString g_data = getData(field);
	if (cs)
	{
		return g_data == data;
	}
	else
	{
		return g_data.toUpper() == data.toUpper();
	}
	
	//never gets here
	return FALSE;
}

void LHMailHeader::setParameter(const QString& field, const QString& parameter, const QString& data)
{
	setSendDataValid( FALSE );
	d->map[field].props[parameter] = data;
}
	

void LHMailHeader::removeParameter(const QString& field, const QString& parameter)
{
	setSendDataValid( FALSE );
	d->map[field].props.erase(d->map[field].props.find(parameter));
}

QString LHMailHeader::getParameter(const QString& field, const QString& parameter)
{
	return d->map[field].props[parameter];
}

void LHMailHeader::setDataDateTime(const QString& field, const QDateTime& data)
{
	setSendDataValid( FALSE );
	setData(field, LHMime::encodeRfcTime(data));
}

QDateTime LHMailHeader::getDataDateTime(const QString& field)
{
	return LHMime::decodeRfcTime(getData(field));
}

bool LHMailHeader::isSinglePart()
{
	bool ret= TRUE;
	
	QString ct = getData("Content-Type");
	
	if (ct.contains("multipart"))
	{
		ret=FALSE;
	}
	
	return ret;
};

/****************************************************************************/

QString LHMailHeader::getSendData (const QString& field)
{
	if (isSendDataValid() || d->is_parsed)
	{
		return d->send_data;
	}

        d->send_data = "";
	
	//we make copy of this data because later we do equivalent to:
	//LHMime::getCString(encoding(), codec_name);
	//and it clearc codec_name

	char* codec_name = qstrdup(encoding().toLatin1()); //"ISO-8859-1";        
	char enc = 'q';

	LHMailHeaderMap::ConstIterator it;

        LHMailHeaderMap::ConstIterator begin;
        LHMailHeaderMap::ConstIterator end;

        if (field == QString::null) {
                begin = d->map.begin ();
                end = d->map.end ();
        }
        else {
                begin = d->map.find (field);
                end = d->map.find (field);
                ++end;
        }
        
	for (it = begin; it != end; ++it)
	{
		LHMailHeaderField f = it.value();
		
		d->send_data += QString(it.key().toLatin1());
		d->send_data += QString(": ");
		d->send_data += LHMime::encodeMailHeader(f.data, codec_name, enc);
		
		if (f.props.size() > 0)
		{
			d->send_data += "; ";
			LHMailHeaderFieldMap::ConstIterator it2;

                        for (it2 = f.props.begin();;)
			{
				d->send_data += QString(it2.key().toLatin1());
				d->send_data += QString("=\"");
			        d->send_data += LHMime::encodeMailHeader(it2.value(), codec_name, enc);
				d->send_data += QString("\"");

                                if (++it2 != f.props.end ())
                		        d->send_data += QString("; ");
                                else
                                        break;
			}
		}
		d->send_data += "\r\n";
	}
	
	d->send_data += "\r\n";
	
	free(codec_name);
	
	setSendDataValid( TRUE );
	
	return d->send_data;
}

/****************************************************************************/

int LHMailHeader::setEncoding(const QString& encod)
{
	setSendDataValid( FALSE );
	//qDebug()<<"Setting encoding for header: "<<encod;
	d->encoding = encod;
	return 0;
}

QString LHMailHeader::encoding()
{
	return d->encoding;
}

void LHMailHeader::addFieldParameter(const QString& field, const QString& parameter)
{
	LHMailHeaderPrivate::field_param_map[field].append(parameter);
}
	
bool LHMailHeader::testFieldHasParameter(const QString& field)
{
	return LHMailHeaderPrivate::field_param_map[field].size()>0;
}
	
uint LHMailHeader::parse(const QString& mail_data)
{

        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailHeader::parse ()");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, (const char *)mail_data);
        
	//QString s_mail_data(mail_data);
	QTextStream ts;
	ts<<mail_data;
	QString line;
	QStringList lines;
	
	bool more = TRUE;
	while(more)
	{
		line = ts.readLine();
		d->send_data += line;
		
		if (line.startsWith(" ") || line.startsWith("\t"))
		{
			lines.last() += line;
		}
		else
		{
			lines.append(line);
		}
		
		if (line == "")
		{
			more = FALSE;
		}
	}
	
	for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
	{
		line = *it;
		//qDebug("HEADER LINE: %s", line.latin1());
		parseLine(line);
    }
	
	d->is_parsed = TRUE;
	
	return ts.device()->pos();
	
}

void LHMailHeader::parseLine(const QString& line)
{
	int col_pos = line.indexOf(":");
	if (col_pos < 1)
	{
		return;
	}
	
	QString field = line.left(col_pos);
	if (testFieldHasParameter(field))
	{
		QStringList prop_list = LHMailHeaderPrivate::field_param_map[field];
		
		for ( QStringList::Iterator it = prop_list.begin(); it != prop_list.end(); ++it )
		{
			QString prop = *it;
			int pos = line.indexOf(prop+"=", FALSE);
			//qDebug("Searchnig for prop: %s, pos: %d", prop.latin1(), pos);
			if (pos>0)
			{
				int end;
				int start = pos+prop.length();
				
				if (line[start + 1] == '\"')
				{
					start+=2; 
					end = line.indexOf("\"", start);
					//qDebug("param starts with \", end: %d", end);
				}
				else
				{
					start++;
					end = line.indexOf(";", start);
					//qDebug("param find: ';' , end: %d", end);
					if (end < start)
					{
						end = line.indexOf(" ", start);
						//qDebug("param for: ' ' , end: %d", end);
					}
				}
				if (end < start)
				{
					end = line.length();
					//qDebug("param setting end at length, end: %d", end);
				}
				//qDebug("Searchnig for prop: %s, start: %d, end: %d", prop.latin1(), start, end);
				QString prop_data = LHMime::decodeMailHeader( line.mid(start, end-start) );
				
				//qDebug("SETTING header parameter, '%s' '%s' '%s'", field.latin1(), prop.latin1(), prop_data.latin1()); 
				setParameter(field,prop,prop_data);
			}
		}
		
		int end = line.indexOf(";");
		//qDebug("param find for first: ';' , end: %d", end);
		
		QString data = LHMime::decodeMailHeader( line.mid(col_pos+2, end - col_pos -2));
		//qDebug("SETTING header data, '%s' '%s'", field.latin1(), data.latin1()); 
			   
		setData(field, data);
	}
	else
	{
		QString data = LHMime::decodeMailHeader( line.mid(col_pos+2));
		//qDebug("SETTING header data, '%s' '%s'", field.latin1(), data.latin1()); 
			   
		setData(field, data);
	}
	
}

QString LHMailHeader::getFileName(bool NO_PATH)
{
	QString file_name = getParameter("Content-Type", "filename");
	if (file_name.isEmpty())
	{
		file_name = getParameter("Content-Type", "name");
	}
	if (file_name.isEmpty())
	{
		file_name = getParameter("Content-Disposition", "filename");
	}
	if (file_name.isEmpty())
	{
		file_name = "no_name";
	}
	
	if (NO_PATH)
	{
		QFileInfo fi(file_name);
		file_name = fi.fileName();
	}
	
	return file_name;
}

bool LHMailHeader::isSendDataValid()
{
	return d->is_send_data_valid;
}
	
bool LHMailHeader::setSendDataValid(bool VAL)
{
	if (d->is_parsed && !VAL)
	{
		qWarning("Changing parsed Header");
	}
	d->is_send_data_valid = VAL;
        return true;
}




