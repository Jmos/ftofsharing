/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/


#include "receiver.h"
#include "lhpop3.h"
#include "lhmail.h"
#include "lhmailaddr.h"
#include "lhmailpart.h"
#include "lhmailheader.h"
#include "lhmime.h"
#include "lhmailbrowser.h"

#include <iostream>

#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qlistview.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qptrlist.h>
#include <qcheckbox.h>

class LHMailListViewItem : public QListViewItem
{
public:
		LHMailListViewItem(LHMail* _mail, QListView* list, LHMailBrowser* edit, QListView* _files):
			QListViewItem(list, _mail->subject(), _mail->from(), _mail->date().toString())
		{
			m_edit = edit;
			mail = _mail;
			files = _files;
		};

protected:
	
	virtual void activate();
		
private:
	LHMailBrowser* m_edit;
	LHMail* mail;
	QListView *files;
};

void LHMailListViewItem::activate()
{
	qDebug("Activate: %p", mail);
	qDebug("Activate: %s", mail->messageBody().latin1());
	m_edit->setMail(mail);
	
	files->clear();
	uint pc = mail->getPartCount();
	for (uint i=0; i<pc; i++)
	{
		LHMailBase* mp = mail->getPart(i);
		new QListViewItem(files, 
						  mp->header().getData("Content-Type"), 
						  mp->header().getParameter("Content-Type", "filename")
						  );
	}
}

Receiver::Receiver( QWidget *parent )
: QWidget( parent )
{
	QGridLayout * layout = new QGridLayout( this, 1, 1, 6 );

	layout->addWidget( new QLabel( tr( "POP3:" ), this ), 0, 0 );
	pop3_server = new QLineEdit( this );
	//pop3_server->setText("10.10.11.7"); 
	pop3_server->setText("127.0.0.1");
	layout->addWidget( pop3_server, 0, 1 );

	layout->addWidget( new QLabel( tr( "User:" ), this ), 1, 0 );
	user = new QLineEdit( this );
	user->setText("test");
	layout->addWidget( user, 1, 1 );

	layout->addWidget( new QLabel( tr( "Pass:" ), this ), 2, 0 );
	pass = new QLineEdit( this );
	pass->setText("test");
	layout->addWidget( pass, 2, 1 );
	
	layout->addWidget( new QLabel( tr( "Delete:" ), this ), 3, 0 );
	dele = new QCheckBox( this );
	layout->addWidget( dele, 3, 1 );

	get = new QPushButton( tr( "&Get" ), this );
	layout->addWidget( get, 4, 0 );
	connect( get, SIGNAL( clicked() ), this, SLOT( getMails() ) );
	
	mails = new QListView( this );
	mails->addColumn(tr("Subject"));
	mails->addColumn(tr("From"));
	mails->addColumn(tr("Date"));
	layout->addMultiCellWidget( mails, 5, 5, 0, 1 );

	message = new LHMailBrowser( this );
	message->setText("");
	layout->addMultiCellWidget( message, 6, 6, 0, 1 );
	
	files = new QListView( this );
	files->addColumn(tr("Type"));
	files->addColumn(tr("Name"));
	layout->addMultiCellWidget( files, 7, 7, 0, 1 );
	
	sendStatus = new QLabel( this );
	layout->addMultiCellWidget( sendStatus, 8, 8, 0, 1 );

	pop3 = 0;
	
	LHMail::initMailSystem();
}


void Receiver::getMails()
{
	sendStatus->setText( tr( "Looking up mail servers" ) );

	if (!pop3)
	{
		pop3 = new LHPop3( pop3_server->text(), user->text(), pass->text());
		
		connect( pop3, SIGNAL(status(const QString &)), 
				 sendStatus, SLOT(setText(const QString &)) );
		connect( pop3, SIGNAL(newMail(LHMail*)), 
				 this, SLOT(newMail(LHMail*) ) );
	}

	pop3->getAllMails(dele->isChecked());
	
	return;
}
	
Receiver::~Receiver()
{
	if (pop3)
	{
		pop3->deleteLater();
	}
}

void Receiver::newMail(LHMail* mail)
{
	mail_list.append(mail);
	
	qDebug("newMail, body: %s", mail->messageBody().latin1());
	
	new LHMailListViewItem(mail, mails, message, files);
		
	//(void) new QListViewItem(mails, mail->subject(), mail->from(), mail->date().toString());
	
}

