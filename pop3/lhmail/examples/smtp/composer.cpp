/****************************************************************************f
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/


#include "composer.h"
#include "lhsmtp.h"
#include "lhmail.h"
#include "lhmailaddr.h"
#include "lhmime.h"
#include "lhmailsignature.h"

#include <iostream>

#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qlistview.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qcheckbox.h>

class LHAttachmentListViewItem : public QListViewItem
{
public:
	LHAttachmentListViewItem(QListView* _lview, const QString& _fname) : QListViewItem(_lview, _fname)
	{
		lview = _lview;
		fname = _fname;
		is_file = true;
		is_mail = false;
	};

	LHAttachmentListViewItem(QListView* _lview, LHMail* _mail) : QListViewItem(_lview, "FW:"+_mail->subject())
	{
		lview = _lview;
		fname = "";
		is_file = false;
		is_mail = true;
		mail = _mail;
	};

	QListView* lview;
	QString fname;
	LHMail* mail;
	bool is_file;
	bool is_mail;
};

Composer::Composer( QWidget *parent )
: QWidget( parent )
{
	QGridLayout * layout = new QGridLayout( this, 1, 1, 6 );

	uint line = 0;
	
	layout->addWidget( new QLabel( tr( "SMTP:" ), this ), line, 0 );
	smtp_server = new QLineEdit( this );
	//smtp_server->setText("10.10.11.7"); 
	smtp_server->setText("10.10.11.7");
	layout->addWidget( smtp_server, line, 1 );
	line++;

	layout->addWidget( new QLabel( tr( "From:" ), this ), line, 0 );
	from = new QLineEdit( this );
	from->setText("Lukasz Iwaszkiewicz <lukasz.iwaszkiewicz@software.com.pl>");
	layout->addWidget( from, line, 1 );
	line++;

	layout->addWidget( new QLabel( tr( "To:" ), this ), line, 0 );
	to = new QLineEdit( this );
	//to->setText("pawel@software.com.pl, pawel@lefthand.com.pl");
	to->setText("lukaszi@software.com.pl");
	layout->addWidget( to, line, 1 );
	line++;
	
	layout->addWidget( new QLabel( tr( "Cc:" ), this ), line, 0 );
	cc = new QLineEdit( this );
	//cc->setText("pawel@software.com.pl");
	cc->setText("");
	layout->addWidget( cc, line, 1 );
	line++;
	
	layout->addWidget( new QLabel( tr( "Bcc:" ), this ), line, 0 );
	bcc = new QLineEdit( this );
	//bcc->setText("pawel@software.com.pl");
	bcc->setText("");
	layout->addWidget( bcc, line, 1 );
	line++;

	layout->addWidget( new QLabel( tr( "Encoding" ), this ), line, 0 );
	enc = new QLineEdit( this );
	//bcc->setText("pawel@software.com.pl");
	enc->setText("ISO-8859-2");
	layout->addWidget( enc, line, 1 );
	line++;

	layout->addWidget( new QLabel( tr( "Subject:" ), this ), line, 0 );
	subject = new QLineEdit( this );
	//subject->setText("test");
	subject->setText(tr("test"));
	layout->addWidget( subject, line, 1 );
	line++;

	layout->addWidget( new QLabel( tr( "Send as HTML:" ), this ), line, 0 );
	html = new QCheckBox( this );
	layout->addWidget( html , line, 1 );
	line++;

	bold = new QPushButton( tr( "Bold" ), this );
	layout->addWidget( bold, line, 0 );
	connect( bold, SIGNAL( clicked() ), this, SLOT( setBold() ) );
	line++;
	
	message = new QTextEdit( this );
	message->setText("test");
	//message->setTextFormat(Qt::RichText);
	layout->addMultiCellWidget( message, line, line, 0, 1 );
	line++;

	attach = new QPushButton( tr( "&Attach" ), this );
	layout->addWidget( attach, line, 0 );
	connect( attach, SIGNAL( clicked() ), this, SLOT( attachFile() ) );

	forward = new QPushButton( tr( "Attach &Forward" ), this );
	layout->addWidget( forward, line, 1 );
	connect( forward, SIGNAL( clicked() ), this, SLOT( attachForward() ) );
	line++;

	files = new QListView( this );
	files->addColumn(tr("Attached Files"));
	layout->addMultiCellWidget( files, line, line, 0, 1 );
	line++;

	send = new QPushButton( tr( "S&end" ), this );
	layout->addWidget( send, line, 0 );
	connect( send, SIGNAL( clicked() ), this, SLOT( sendMessage() ) );

	save = new QPushButton( tr( "&Save" ), this );
	layout->addWidget( save, line, 1 );
	connect( save, SIGNAL( clicked() ), this, SLOT( saveMessage() ) );
	line++;

	sendStatus = new QLabel( this );
	layout->addMultiCellWidget( sendStatus, line, line, 0, 1 );

	LHMail::initMailSystem();

	smtp = 0;
}


void Composer::attachFile()
{
	QString new_file = QFileDialog::getOpenFileName(
												   ".",
												   "*",
												   this,
												   tr("Attach file dialog") +
												   tr("Choose a file to attach"));

	std::cout<< new_file <<"\n";

	new LHAttachmentListViewItem(files, new_file);// new QListViewItem(files, new_file);
}

void Composer::attachForward()
{
	LHMail* mail = createMessage();
	new LHAttachmentListViewItem(files, mail);
}

/****************************************************************************/

LHMail* Composer::createMessage()
{
	LHMail *mail = 0;   

	if (!testAddresses(to->text(), tr("To"))) return 0;
	if (!testAddresses(cc->text(), tr("CC"))) return 0;
	if (!testAddresses(bcc->text(), tr("BCC")))	return 0;

	message->setTextFormat(Qt::PlainText);
	mail = new LHMail(from->text(), 
					  to->text(),
					  subject->text(), 
					  message->text(),
					  0, // do not delete
					  cc->text(),
					  bcc->text());
	
	if (html->isChecked())
	{
		message->setTextFormat(Qt::RichText);
		mail->attachHtml(message->text());
	}

	mail->setEncoding(enc->text());

	//Attaching files
	LHAttachmentListViewItem* element = (LHAttachmentListViewItem*) files->firstChild();

	while (element != NULL)
	{
		if (element->is_file)
		{
			qDebug("Attaching file %s", element->text(0).latin1());
			mail->attachFile(element->fname, LHMP_CONVERT_CRLF);
		}
		else if (element->is_mail)
		{
			qDebug("Attaching mail %s", element->text(0).latin1());
			mail->attachMail(element->mail);
		}
		element = (LHAttachmentListViewItem*) element->nextSibling();
	}

        LHMailSignature sig;
        sig.setCertFile ("/home/iwasz/mykey/iwasz/iwasz.pem");
        sig.setKeyFile ("/home/iwasz/mykey/iwasz/key.pem");
        mail->attachSignature (sig);

	return mail;
}

/****************************************************************************/

void Composer::sendMessage()
{
	sendStatus->setText( tr( "Looking up mail servers" ) );

	if (!smtp)
	{
		smtp = new LHSmtp( smtp_server->text());
		//connect( smtp, SIGNAL(destroyed()), this, SLOT(enableSend()) );
		connect( smtp, SIGNAL(status(const QString &)), 
				 sendStatus, SLOT(setText(const QString &)) );
		connect( smtp, SIGNAL(sentMail(LHMail*)), 
				 this, SLOT(sentMail(LHMail*)) );
	}

	for (int i=0; i<1; i++)
	{
		LHMail *mail = createMessage();
	
		if (!mail)
		{
			return;
		}
	
		smtp->send(mail);
	}

	return;
}

/****************************************************************************/

void Composer::saveMessage()
{
	LHMail *mail = createMessage();

	if (mail == NULL)
	{
		qWarning("Composer::saveMessage, mail is NULL");
		return;
	};

	QFile f("MAIL_TMP");
        
	if (!f.open(IO_WriteOnly))
	{
		qWarning("Could not open file: %s", (const char*) f.name());
		return;
	}

	QCString data = mail->getSendData();
	//qDebug("data: %s", (const char*) data);
	f.writeBlock((const char*)data, data.length());
	f.close();

	QMessageBox::information(this, tr("Mail saved"), tr("Mail data was saved in file: %1").arg(f.name()));

	qDebug("Mail size: %u", mail->getMemSize());
	mail->deleteLater();
}

/****************************************************************************/

int Composer::testAddresses(const QString& addr, const QString& section)
{
	QString wrong;
	int invalid = LHMailAddr::testMulti(wrong, addr);
	int send = 1;
	if (invalid != 0)
	{
		send = !QMessageBox::question(
									 this,
									 tr("Wrong mail address, send anyway?"),
									 tr("In %1 section is wrong %2 addresses: %3 "
										"Do you want to send mail anyway?")
									 .arg(section).arg(invalid).arg(wrong),
									 tr("&Yes"), tr("&No"),
									 QString::null, 0, 1 );
	}
	return send;
}

Composer::~Composer()
{
	if (smtp)
	{
		smtp->deleteLater();
	}
}

void Composer::enableSend()
{
	send->setEnabled( TRUE );
}

void Composer::sentMail(LHMail* mail)
{
        qDebug ("*** %s,%d : %d", __FILE__, __LINE__, (int)mail);
	mail->deleteLater();
}

void Composer::setBold()
{
	qDebug("setting Bold");
	message->setBold( true );
}

