/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/


#include "browser.h"
#include "lhmail.h"
#include "lhmailaddr.h"
#include "lhmailpart.h"
#include "lhmailheader.h"
#include "lhmime.h"
#include "lhfiletools.h"

#include <iostream>

#include <QtGui>

class LHMailTreeWidgetItem : public QTreeWidgetItem
{
public:
	LHMailTreeWidgetItem(LHMail* _mail, QTreeWidget* list, LHMailBrowser* br, QTreeWidget* _files, QLabel* _status):
	QTreeWidgetItem(list, QStringList()<<_mail->subject()<<_mail->from()<< _mail->date().toString())
	{
		m_br = br;
		mail = _mail;
		files = _files;
		status = _status;
	};

protected:

	virtual void activate();

private:
	LHMailBrowser* m_br;
	LHMail* mail;
	QTreeWidget *files;
	QLabel* status;
};

void LHMailTreeWidgetItem::activate()
{
    //m_br->setText(mail->messageBody());
	m_br->setMail(mail);

	files->clear();
	uint pc = mail->getPartCount();
	status->setText( QString( "Mail with %1 parts" ).arg(pc) );
	for (uint i=0; i<pc; i++)
	{
		LHMailBase* mp = mail->getPart(i);
		new QTreeWidgetItem(files, QStringList()<< mp->header().getData("Content-Type")
		<< mp->header().getParameter("Content-Type", "filename") );
	}
}


Browser::Browser( QWidget *parent )
: QWidget( parent )
{
	QGridLayout * layout = new QGridLayout(this);

	get = new QPushButton( tr( "&Get" ), this );
	layout->addWidget( get, 0, 0 );
	connect( get, SIGNAL( clicked() ), this, SLOT( getMails() ) );

	mails = new QTreeWidget(this);
	mails->setHeaderLabels(QStringList()<<tr("Subject")<<tr("From")<<tr("Date"));
	layout->addWidget( mails, 1, 1, 0, 1 );

	mb = new LHMailBrowser( this );
	mb->setText("");
	layout->addWidget( mb, 2, 2, 0, 1 );
	
	connect(mb, SIGNAL(activateLink(const QString&)), this, SLOT(activateLink(const QString&)));
	connect(mb, SIGNAL(popupForLink(const QString&, QPopupMenu*)), 
			this, SLOT(popupForLink(const QString&, QPopupMenu*)));
	
	files = new QTreeWidget( this );
	files->setHeaderLabels(QStringList()<<tr("Type")<<tr("Name"));
	layout->addWidget( files, 3, 3, 0, 1 );

	sendStatus = new QLabel( this );
	layout->addWidget( sendStatus, 4, 4, 0, 1 );

	LHMail::initMailSystem();
	
	actionWebBrowser = new QAction(tr("Open in browser"), this);
	connect( actionWebBrowser, SIGNAL( activated() ), this, SLOT( startWebBrowser() ) );
}

void Browser::getMails()
{
	sendStatus->setText( tr( "Reading from disk" ) );
	
	uint mail_num = 0;
	QDir d("Mails");
	d.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
	d.setSorting( QDir::Name );

	QFileInfoList list = d.entryInfoList();
	unsigned int i=0;
	QFileInfo fi;

	while (i<list.count()-1)
	{
		//qDebug( "%s", fi->fileName().latin1() );
		fi=list.at(i);
		
		QFile f( d.filePath(fi.fileName()) );
        if ( !f.open(QFile::ReadWrite) )
		{
            qWarning()<< "Cannot open the file "<<f.fileName().toLatin1();
		}
		else
		{
			LHMail *mail = new LHMail();
			uint size = fi.size()+1;
			char *buf = (char*) malloc(size);
			memset(buf, 0, size);
			f.read(buf, size);
			
			QString data(buf);
			mail->setData(data);
			newMail(mail);
			free(buf);
			mail_num++;
		}
		
		i++;
	}

	sendStatus->setText( tr( "done %1 mails" ).arg(mail_num) );
	
	return;
}

Browser::~Browser()
{
}

void Browser::newMail(LHMail* mail)
{
	mail_list.append(mail);

	//qDebug("newMail, body: %s", mail->messageBody().latin1());

	new LHMailTreeWidgetItem(mail, mails, mb, files, sendStatus);

	//(void) new QListViewItem(mails, mail->subject(), mail->from(), mail->date().toString());

}

void Browser::finish()
{
	//LHFileTools::removeAppTempDir();
}

void Browser::activateLink(const QString& name)
{
	if (name.startsWith("http://") || name.startsWith("ftp://"))
	{
		link = name;
		startWebBrowser();
	}
	else
	{
		//
	}
}

void Browser::startWebBrowser()
{
	QProcess *proc = new QProcess();
	proc->start( "mozilla", QStringList()<<link );

	if (!proc->waitForStarted())
	{
		qWarning("Can not start application");
	}
}

void Browser::popupForLink(const QString& link, QMenu* menu)
{
	if (link.startsWith("http://") || link.startsWith("ftp://"))
	{
		this->link = link;
		menu->addAction(actionWebBrowser);
	}
}

