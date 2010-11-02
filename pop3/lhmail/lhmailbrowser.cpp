/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmailbrowser.h"
#include "lhmail.h"
#include "lhmailheader.h"
#include "lhmailpart.h"
#include "lhmime.h"
#include "lhfiletools.h"
#include "lhexternalapp.h"

#include <QtGui>

class LHMailBrowserPrivate
{
public:
	LHMailBrowserPrivate():
	mail(0),
	brush("white")
	{
	};

	LHMail* mail;
	QBrush brush;
};

LHMailBrowser::LHMailBrowser(QWidget* parent) : QTextBrowser(parent)
{
	d = new LHMailBrowserPrivate();
};

LHMailBrowser::~LHMailBrowser()
{
	delete d;
}


void LHMailBrowser::setMail(LHMail* mail)
{

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "1");

	if (d->mail)
	{
		d->mail->cleanTemp();
	}

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "2");

        d->mail = mail;

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "3");

        // Passing NULL enforces clearing of d->mail by invoking its cleanTemp() method.
        if (!mail)
                return;

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "4");

	QString text = mail->getShowText (false, false);
	// QString text = mail->getShowText ();
        
	//qDebug("\n---\n\n%s", (const char*) text);
	clear();
	//setPaper(d->brush);

        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "~~~~~~~~~~~`");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, text.ascii ());
        
	setText(text);
}

void LHMailBrowser::setSource( const QString &name )
{
	qDebug()<<"LHMailBrowser::setSource, name: "<<name;

	if (name.startsWith("html://"))
	{
		QString address = name.mid(7);
		address.replace("/","");

	        qDebug()<<"LHMailBrowser::setSource (html), address: "<<address;
                
		LHMailBase* mb = d->mail->getFromAddress(d->mail, address);

		//qDebug("\nHTLML\n---\n%s", (const char*) mb->messageBody());
		QTextBrowser::setText( mb->messageBody() );
		//emit customLinkClicked(name);
	}
        
	else if (name.startsWith("imag://") || name.startsWith("part://"))
	{
		QString address = name.mid(7);
		address.replace("/","");
		qDebug()<<"LHMailBrowser::setSource (img or part), address: "<<address;
		LHMailBase* mb = d->mail->getFromAddress(d->mail, address);
		LHExternalApp::showInAppUser(mb);
	}
/*
	else if (name.startsWith("http://"))
	{
		QProcess *proc = new QProcess();
		proc->addArgument( "mozilla" );
		proc->addArgument( name );

		if (!proc->start())
		{
			qWarning("Can not start application");
		}
	}
*/
	else
	{
		qDebug()<<"LHMailBrowser::setSource, address not handled emiting activateLink: "<<name;
		emit activateLink(name);
	}
}

QMenu *LHMailBrowser::createPopupMenu( const QPoint& pos )
{
	QMenu *m = new QMenu(0);
	QString link = anchorAt( pos );
	if (link.isEmpty())
	{
		delete m;
		//return QTextBrowser::createPopupMenu(pos);
		return 0;
	}
	qDebug()<<"link right click: "<<link<<"\n";

	emit popupForLink(link, m);

	return m;
}
