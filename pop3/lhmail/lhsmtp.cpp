/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhsmtp.h"
#include "lhsmtpauth.h"
#include "lhmail.h"
#include "lhmailerr.h"
#include "lhmailsocket.h"

#include <qtextstream.h>
#include <qtcpsocket.h>
#include <qhostinfo.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qregexp.h>

/****************************************************************************/

enum State {
        Wait, //0
        Disconnected, //1
        Connected, //2
        Init, //3
        Mail, //4
        Auth, //5
        Rcpt,
        Data,
        Body,
        Quit,
        Close
        };

/****************************************************************************/

class LHSmtpPrivate {
public:

        LHSmtpPrivate(LHSmtp* _parent, const QString &smtp):
        parent(_parent),
        smtp_server(smtp),
        message(),
        from(),
        rcpt(),
        socket(0),
        t(0),
        state(Disconnected),
        response(),
        responseLine(),
        mxLookup(0),
        toSend(),
        rcpt_count(0),
        rcpt_curr(0),
        bytes_written(0),

        smtpAuth (NULL)
        {
                // socket = new QSocket(parent);
                socket = new LHMailSocket (parent);

                // socket->setExtension (new LHMailSocketExtensionSSL, true);
        };
        
        ~LHSmtpPrivate ()
        {
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtpPrivate () start");
                
                if (socket)
                {
                        delete socket;
                }
                if (t)
                {
                        delete t;
                }
                if (mxLookup)
                {
                        delete mxLookup;
                }
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtpPrivate () stop");                    
        }

        LHSmtp* parent;
        QString smtp_server;
        QString message;
        QString from;
        QString rcpt;
        // QSocket *socket;
        LHMailSocket *socket;
        QTextStream * t;
        State state;
        QString response;
        QString responseLine;
        QHostInfo * mxLookup;
        QList<LHMail *> toSend;
        uint rcpt_count;
        uint rcpt_curr;
        uint bytes_written;
        LHSmtpAuth *smtpAuth;
};

LHSmtp::LHSmtp( const QString &smtp_server)
{
        d = new LHSmtpPrivate(this, smtp_server);
        
        if (!connect ( d->socket, SIGNAL( readyReadMs() ),
                          this, SLOT( readyRead() ) ))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!connect");
        
        if (!connect ( d->socket, SIGNAL( bytesWritten(int) ),
                          this, SLOT( emitBytesWritten(int) ) ))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!connect");

        if (!connect ( d->socket, SIGNAL( connectedMs() ),
                          this, SLOT( connected() ) ))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!connect");

        if (!connect ( d->socket, SIGNAL( connectionClosed() ),
                          this, SLOT( connectionClosed() ) ))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!connect");

        if (!connect  (d->socket, SIGNAL (error (int)),
                          this, SLOT (onError (int))))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!connect");

        d->state = Disconnected;
}


LHSmtp::~LHSmtp()
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Destructor LHSMTP");
        while(d->toSend.count()>0){
        	LHMail *m=d->toSend.takeFirst();
        	delete m;
       	}
        delete d;
}


void LHSmtp::openConnection()
{
        //from here we go to dnsLookupHelper, when connect is done
        if (d->state != Disconnected)
        {
                qDebug("Wrong state in connect");
                emit status( tr( "Wrong state in connect") );
                emit (error (tr( "Wrong state in connect")));
                // emit (errorMail (NULL, LHMailError (1)));
                return;
        }
	QHostInfo::lookupHost(d->smtp_server,this,SLOT(dnsLookupHelper(QHostInfo)));
}

void LHSmtp::dnsLookupHelper(const QHostInfo &hostInfo)
{
	qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtp::dnsLookupHelper()");

	QString host = "";
	*d->mxLookup = hostInfo;
	QList<QHostAddress> s = d->mxLookup->addresses();
	if (hostInfo.error() != QHostInfo::NoError) {
		qDebug() << "Lookup failed:" << hostInfo.errorString();
		return;
	}
	if (s.isEmpty())
	{
		host = d->smtp_server;
	}
	else
	{
		host = s.first().toString();
	}

        emit status( tr( "Connecting to %1" ).arg( host ) );

        d->state = Init;
        d->socket->connectToHost( host, 25 );
        d->t = new QTextStream( d->socket );
}

void LHSmtp::connected()
{
        // QMessageBox::warning (NULL, tr ("Uwaga"), tr ("Connected to SMTP server"));
        emit status( tr( "Connected to %1" ).arg( d->socket->peerName() ) );
}

void LHSmtp::connectionClosed()
{
        emit status( tr( "Connection to %1 closed" ).arg( d->socket->peerName() ) );
        emit disconnected(this);
        d->state = Disconnected;
        //deleteLater();
}

void LHSmtp::send(LHMail* mail)
{
        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtp::send");
        
        d->toSend.append (mail); // will be removed by PtrList
        
        // qDebug ("*** %s,%d : LHSmtp::send count = %d", __FILE__, __LINE__, d->toSend.count ());

        if (d->state == Disconnected)
        {
                qDebug("Opening Connection");
                openConnection();
        }

        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "STOP LHSmtp::send");
}

void LHSmtp::emitSentMail(LHMail* mail)
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtp::emitSentMail()");
        emit sentMail(mail);
        //d->toSend.remove (mail);
}

void LHSmtp::emitBytesWritten(int bw)
{
        d->bytes_written += bw;
        emit bytesWritten(d->bytes_written);
        emit status( tr("Bytes written: %1").arg(d->bytes_written));
}

void LHSmtp::readyRead()
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHSmtp::readyRead ()");
        //emit status( tr( "readyRead %1" ).arg( socket->peerName() ) );
        
        // SMTP is line-oriented
        if (!d->socket->canReadLine())
        {
                emit (status (tr ( "Socket %1 can not read lines" ).arg (d->socket->peerName ())));
                emit (error (tr ( "Socket %1 can not read lines" ).arg (d->socket->peerName ())));
                // emit (errorMail (d->toSend.current (), LHMailError (2)));
                d->state = Close;
                return;
        }

        do
        {
                d->responseLine = d->socket->readLine();
                d->response += d->responseLine;
                // qDebug("%s", d->responseLine.ascii());
        } while (d->socket->canReadLine() && d->responseLine[3] != ' ');
        
        d->responseLine.truncate( 3 );

                
        if (d->state == Init && d->responseLine[0] == '2')
        {
                // banner was okay, let's go on
                *(d->t) << "HELO there\r\n";

                if (d->smtpAuth)
                        d->state = Auth;
                else
                        d->state = Mail;
        }

        else if (d->state == Auth) {

                bool end;
                QString out = d->smtpAuth->getOutputLine (&end);

                *(d->t) << out;

                if (end)
                        d->state = Mail;

                qDebug ("*** %s,%d : %s, %d, %s", __FILE__, __LINE__, "state = auth", d->state, d->responseLine.toAscii());
        }
        
        else if (d->state == Mail && d->responseLine[0] == '2')
        {
                // HELO response was okay (well, it has to be)
                *(d->t) << "MAIL FROM: <" << d->toSend[0]->from() << ">\r\n";
                d->rcpt_count = d->toSend[0]->countRcpt();
                d->rcpt_curr = 0;
                //qDebug("LHSmtp::readyRead, FROM sent, rcpt_count: %d, rcpt_curr: %d", d->rcpt_count, d->rcpt_curr);
                d->state = Rcpt;
        }
        else if (d->state == Rcpt && d->responseLine[0] == '2')
        {
                //qDebug("LHSmtp::readyRead, Will send RCPT, rcpt_count: %d, rcpt_curr: %d", d->rcpt_count, d->rcpt_curr);
                
                *(d->t) << "RCPT TO: <" << d->toSend[0]->getRcpt(d->rcpt_curr) << ">\r\n";
                ++d->rcpt_curr;
                if (d->rcpt_curr < d->rcpt_count)
                {
                        d->state = Rcpt;
                }
                else
                {
                        d->state = Data;
                }
        }
        else if (d->state == Data && d->responseLine[0] == '2')
        {
                *(d->t) << "DATA\r\n";
                d->state = Body;
        }
        else if (d->state == Body && d->responseLine[0] == '3')
        {
                d->toSend[0]->send(d->t);
                *(d->t) << "\r\n.\r\n";
                d->state = Quit;
        }
        else if (d->state == Quit && d->responseLine[0] == '2')
        {
                LHMail* sent = d->toSend.takeFirst();
                Q_ASSERT (sent != NULL);
                
                sent->sent(this); 
                
                d->bytes_written = 0;
                
                unsigned int mails_to_send = d->toSend.count();
                
                if (mails_to_send == 0)
                {
                        *(d->t) << "QUIT\r\n";
                        // here, we just close.
                        d->state = Close;
                }
                else
                {
                        //start send next mail
                        *(d->t) << "MAIL FROM: <" << d->toSend[0]->from() << ">\r\n";
                        d->rcpt_count = d->toSend[0]->countRcpt();
                        d->rcpt_curr = 0;
                        //qDebug("LHSmtp::readyRead, FROM sent, rcpt_count: %d, rcpt_curr: %d", d->rcpt_count, d->rcpt_curr);
                        d->state = Rcpt;
                }
                
                emit status( tr( "Message sent, to send: %1" ).arg(mails_to_send));
        }
        else if (d->state == Close)
        {
                emit disconnected(this);
                //deleteLater();
                return;
        }
        else
        {
                // something broke.
                emit status( tr( "Unexpected reply from SMTP server:\n\n%1" ).arg(d->response) );
                emit error( tr( "Unexpected reply from SMTP server:\n\n%1" ).arg(d->response) );
                // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                // emit (errorMail (d->toSend.current (), LHMailError (3, d->response)));
                // emit (errorMail (d->toSend.current (), LHMailError (5)));
                d->state = Close;
        }

        d->response = "";
}

/****************************************************************************/

void LHSmtp::setAuth (LHSmtpAuth *auth)
{
        d->smtpAuth = auth;
}

/****************************************************************************/

LHSmtpAuth *LHSmtp::getAuth () const
{
        return d->smtpAuth;
}

/**
 *  Slot invoked on d->socket error.
 */

void LHSmtp::onError (int er)
{
        qDebug ("*** %s,%d : %s, %d", __FILE__, __LINE__, "LHSmtp::onError", er);

        /*
        QSocket::ErrConnectionRefused - if the connection was refused
        QSocket::ErrHostNotFound - if the host was not found 
        QSocket::ErrSocketRead - if a read from the socket failed
        */

        switch (er) {
        case QAbstractSocket::ConnectionRefusedError:
                // emit (errorMail (NULL, LHMailError (3)));
                break;
        
        case QAbstractSocket::HostNotFoundError:
                // emit (errorMail (NULL, LHMailError (4)));
                break;
        default:
                break;
        }
        
}

