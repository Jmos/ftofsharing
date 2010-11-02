/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmailaddr.h"
#include "lhmail.h"
#include "lhhtmltools.h"

#include <string.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>

class LHMailAddrPrivate
{
public:
	LHMailAddrPrivate(const QString& addr):
	full(addr),
	valid(0)
	{
		simple = parseAddr(full);
		valid = checkAddr(simple);
	};
	
	QString full;
	QString simple;
	int valid;

	QString parseAddr(const QString& addr)
	{
		int op = addr.indexOf('<');
		if (op > -1)
		{
			int cl = addr.indexOf('>');
			if (cl > op+1)
			{
				return addr.mid(op+1, cl-op-1).trimmed();
			}
			else
			{
				//error but we do not care now
			}
		}
		return addr.trimmed();
	};

	//return 0 if not valid 1 if valid
	
	int checkAddr(const QString& addr) 
	{
		int        count = 0;
		const char *c, *domain;
		static char *rfc822_specials = "()<>@,;:\\\"[]";
		const char *address = addr.toAscii();

		/* first we validate the name portion (name@domain) */
		for (c = address;  *c;  c++)
		{
			if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == 
									   '\"'))
			{
				while (*++c)
				{
					if (*c == '\"')	break;
					if (*c == '\\' && (*++c == ' ')) continue;
					if (*c < ' ' || *c >= 127) return 0;
				}
				if (!*c++) return 0;
				if (*c == '@') break;
				if (*c != '.') return 0;
				continue;
			}
			if (*c == '@') break;
			if (*c <= ' ' || *c >= 127)	return 0;
			if (strchr(rfc822_specials, *c)) return 0;
		}
		if (c == address || *(c - 1) == '.') return 0;

		/* next we validate the domain portion (name@domain) */
		if (!*(domain = ++c)) return 0;
		do
		{
			if (*c == '.')
			{
				if (c == domain || *(c - 1) == '.')	return 0;
				count++;
			}
			if (*c <= ' ' || *c >= 127) return 0;
			if (strchr(rfc822_specials, *c)) return 0;
		} while (*++c);

		return(count >= 1);
	};
};


LHMailAddr::LHMailAddr(const QString &addr)
{
	d = new LHMailAddrPrivate(addr);
}

LHMailAddr::~LHMailAddr()
{
	delete d;
}

QString LHMailAddr::simple()
{
	return d->simple;
}

QString LHMailAddr::full()
{
	return d->full;
}


int LHMailAddr::isValid()
{
	return d->valid;
}

int LHMailAddr::testAddr(const QString& addr)
{
	LHMailAddr* a1 = new LHMailAddr(addr);
	//qDebug("Address full: %s simple: %s valid: %d", a1->full().ascii(), a1->simple().ascii(), a1->isValid());
	delete a1;
	return 0;
}

int LHMailAddr::parseMulti(QList<LHMailAddr *>& addr_vect, const QString &multi_addr)
{
	int all_valid = 1;
	QStringList addr_list;
	addr_list = multi_addr.split(QRegExp("[\\,\\;]"));
	
	for ( QStringList::Iterator it = addr_list.begin(); it != addr_list.end(); ++it )
	{
        //qDebug("Address: %s", (*it).ascii());
		LHMailAddr* addr = new LHMailAddr(*it);
		if (addr->isValid())
		{
			addr_vect.append(addr);
		}
		else
		{
			all_valid = 0;
		}
    }
	
	return all_valid;
	
}

int LHMailAddr::testMulti(QString& wrong, const QString &multi_addr)
{
        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, multi_addr.ascii ());

	int invalid = 0;
	QStringList addr_list;
	addr_list = multi_addr.split(QRegExp("[\\,\\;]"));
	
	for ( QStringList::Iterator it = addr_list.begin(); it != addr_list.end(); ++it )
	{
        //qDebug("Address: %s", (*it).ascii());
		LHMailAddr* addr = new LHMailAddr(*it);
		if (!addr->isValid())
		{
			invalid++;
			wrong += addr->full();
		}
		delete addr;
    }
	return invalid;
	
}

QString LHMailAddr::convToHtmlMulti(const QString &multi_addr)
{
	QString out;
	
	QList<LHMailAddr *> addr_vect;
	parseMulti(addr_vect, multi_addr);
	
	LHMailAddr *addr;
	int first = 1;
	while (addr_vect.count()>0)
	{
		addr = addr_vect.first();
		if (!first)
		{
			out += ", ";
		}
		else
		{
			first = 0;
		}
		out += "<A href=\"mailto:"+addr->simple()+"\">" + LHHtmlTools::clean(addr->full()) + "</A>";
	}
	
	return out;
}

