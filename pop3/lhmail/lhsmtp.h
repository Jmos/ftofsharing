/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#       ifndef LHSMTP_H
#       define LHSMTP_H

#       include <QObject>
#       include <QString>

// class QSocket;
class LHMailSocket;
class QTextStream;
class QHostInfo;
class LHMail;
class LHSmtpPrivate;
class LHSmtpAuth;
class LHMailError;

/**
 * Class used to send mails using SMTP protocol (RFC 821).
 * <p>One object corresponds to one SMTP server. 
 * <p>You can send as many mails as you wish using one @ref LHSmtp object,
 * the connections will be opened and closed when needed.
 * <p>Sending mails is asynchronous, so you should be carefull deleting this object.
 * You should wait for server to disconnect and then delete.
 * You can have many objects of this class in your application,
 * each representing one SMTP server.
 * <p>Example:
 * <pre>
 * //Creating LHSmtp object
 * void MyClass::createSMTP()
 * {
 * ...
 * smtp = new LHSmtp("127.0.0.1"); //server name or address
 * //connect to receive current status of server
 * connect(smtp, SIGNAL(status(const QString &)), 
 *         sendStatus, SLOT(setText(const QString &)) );
 * //connect to be informed when mail is sent
 * connect( smtp, SIGNAL(sentMail(LHMail*)), 
 *          this, SLOT(sentMail(LHMail*)) );
 * }
 * 
 * //Sending mail
 * void MyClass::sendMail()
 * {
 * ...
 * LHMail *mail = new LHMail(from,to, subject,text, 
 *                           0, // do not delete mail
 *                           cc, bcc);
 * mail->setEncoding("ISO-8859-2");
 * //add mail to send queue
 * smtp->send(mail);
 * }
 * ...
 * //deleting sent mail
 * void MyClass::sentMail(LHMail* mail)
 * {
 *   delete mail;
 * }
 * </pre>
 * Complete examples are in <code>examples</code> catalog.
 * <p>
 * You can also distribute mail. In that case for each address on recipient list 
 * (combined <code>to, cc, bcc</code>) is created special object @ref LHMailDistribute and
 * added to send queue. This way in each mail is only one mail in <code>to</code> header.
 * <p>
 * <p>NOT IMPLEMENTED:
 * <p>- SMTP authentication
 * 
 * @short Sending mails using SMTP protocol (RFC 821). 
 * @see LHMail
 */ 

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHSmtp : public QObject {

        Q_OBJECT

public:
        /**
         * Creates one LHSmtp object for given server address or name
         */
         
        LHSmtp (const QString &smtp_server);        
        ~LHSmtp();

        /**
         *  Sets LHSmtpAuth object responsible for SMTP authentication.
         *  LHMail library user must create an instance od LHSmtp class by
         *  his own.
         */

        void setAuth (LHSmtpAuth *auth);

        /**
         *  Returns LHSmtpAuth object set by setAuth method call or NULL
         *  if no auth object was set.
         */

        LHSmtpAuth *getAuth () const;
        
        /**
         * Add mail to send queue and send it later in asynchronous manner.
         * After sending @ref LHMail::sent method is called, which should
         * emit @ref LHSmtp::sentMail signal
         */ 

        void send (LHMail* mail);
        
        /**
         * Used by @ref LHMail class. Do not use it directly.
         * <p>Simply emits @ref LHSmtp::sentMail signal.
         */ 
        void emitSentMail(LHMail* mail);

signals:
        /**
         * Emitted every time status of connection is changed
         */
        void status( const QString & );
        /**
         * Emitted on smtp error.
         */
        void error (const QString &);
        /**
         * Emitted when LHSmtp objects disconnects from server. 
         * <p>Only after this you should delete this object.
         */
        void disconnected(LHSmtp*);
        /**
         * Emitted after completion of sending mail.
         * <p>After this you should delete @ref LHMail object.
         */
        void sentMail(LHMail* mail);
        
        /**
         *  Emitted on SMTP error.
         */
        void errorMail (LHMail* mail, const LHMailError &);

        /**
         * Emits how many bytes are written for this mail.
         * <p>After each mail the counter is zeroed.
         */ 
        void bytesWritten(int nbytes);

private slots:

        void dnsLookupHelper(const QHostInfo &hostInfo);
        void readyRead();
        void connected();
        void connectionClosed();
        void emitBytesWritten(int nbytes);
        void onError (int);
        
protected:

        void openConnection();
        
private:

	LHSmtpPrivate* d;

};

#endif
