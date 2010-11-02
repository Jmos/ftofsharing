/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHPOP3_H
#define LHPOP3_H

#include <QObject>
#include <QString>
#include <stdlib.h>

// class QSocket;
class LHMailSocket;
class QTextStream;
class QHostInfo;
class LHMail;


class LHPop3Private;

/**
 * This class represents single account on Pop3 server. 
 * It is used to retreive mails from server.
 * 
 * Example:
 * <pre>
 * ...
 * LHPop3 pop3 = new LHPop3("10.0.0.1", login, pass );
 * connect( pop3, SIGNAL(newMail(LHMail*)), 
 *          this, SLOT(newMail(LHMail*) ) );
 * 
 * pop3->getAllMails( true ); //delete all mails on server
 * </pre>
 *
 * LHPop3 retreives mails in asynchronous way, so we have to respond
 * to signal that new mail arrived ( @ref newMail ). <br>
 * Before we delete this oblect we should wait for @ref disconnected signal.
 *  
 * @short Representation of single mail account on Pop3 server.
 * @see LHMail
 * 
*/

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHPop3 : public QObject {

        Q_OBJECT

public:
        /**
         * Constructor
         * @param pop3_server Name or address of Pop3 server
         * @param user User login name
         * @param passwd Password for this login
         */ 
        LHPop3 (const QString &pop3_server,
                const QString &user,
                const QString &passwd);

        ~LHPop3();

        /**
         * This is the only method that should be used. It retreives all mails
         * and for each new mail emits signal @ref newMail. <br>
         * Mails are retreived in asynchronous way.
         * @param DELETE True if you want to delete retreived mails on the server
         */ 

        void getAllMails(bool del=true);

        QString getPop3Server () const;
        QString getUser () const;
        QString getPassword () const;
        
signals:
        /**
         * Send status of retreiving mails
         */ 
        void status( const QString & );

/**
 *  Error messages during retreiving mail.
 */

        void error( const QString & );
        
        /**
         * Emitted when LHPop3 objects disconnects from server. 
         * <br>Only after this you should delete this object.
         */
        void disconnected(LHPop3*);
        
        void newMail(LHMail*);

private slots:
        void dnsLookupHelper(const QHostInfo &hostInfo);
        void readyRead();
        void connected();
        void connectionClosed();

private:
        void openConnection();
        void quit();
        void parseStatLine(const QString& line);
        void parseMail(const QString& mail);
        void nextLine(const QString& line);
        
        LHPop3Private* d;
};

#endif
