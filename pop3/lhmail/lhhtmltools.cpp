/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/
#include "lhhtmltools.h"

QString LHHtmlTools::clean(const QString& str)
{
	QString out = str;
	out.replace("&", "&amp;");
	out.replace("<", "&lt;");
	out.replace(">", "&gt;");
	out.replace("\"", "&quot;");
	out.replace("'", "&#39;");
	
	out.replace("\n", "\n<br>");
	
	return out;
}

QString LHHtmlTools::enrich(const QString& str)
{
	QString out = str;
	
	return out;
}

QString LHHtmlTools::full(const QString& str)
{
	return enrich(clean(str));
}

	
