

/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHHTMLTOOLS_H
#define LHHTMLTOOLS_H

#include <QObject>


class QString;

//class LHMimePrivate;

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

/**
 * Helper class to prepare strings for HTML usage.
 */

class LHHtmlTools : public QObject
{
    Q_OBJECT

public:
		
	/**
	 * Encodes all special HTML characters. Also changes \n to &lt;br&gt;
	 */ 
	static QString clean(const QString& str);
	
	/**
	 * Used to find links, mails etc. in text and change them into HTML links
	 */ 
	static QString enrich(const QString& str);
	
	/**
	 * clean + enrich
	 */ 
	static QString full(const QString& str);
	
};

#endif
