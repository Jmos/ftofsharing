/****************************************************************************
* http://sourceforge.net/projects/nlcreator/
*
* QNewsletterCreator - Business Email Client for Mass Mails
* Nuntius Leo - Personal Qt Email Client
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License Version 2 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA. http://www.gnu.org/copyleft/library.html
*
* Please report all bugs and problems to the project admins:
* http://sourceforge.net/projects/nlcreator/
* 
*
* Copyright by dontinelli@users.sourceforge.net if no additional copyright information is given
* otherwise copyright by given authors/organizations 
* and modfified, e.g. ported, by dontinelli@users.sourceforge.net.
* Some code has been taken from 
* http://sourceforge.net/projects/lhmail - License : GNU Library General Public License    
* Authors: lukasz.iwaszkiewicz@gmail.com
* lukasz.iwaszkiewicz@lefthand.com.pl, lukasz.iwaszkiewicz@software.com.pl 
* Copyright (C) 2004/2005 LeftHand sp.z.o.o. info@lefthand.com.pl 
*
*****************************************************************************/

#ifndef LHPOP3_H
#define LHPOP3_H

#include <QObject>
#include <QString>

#include "mail.h"

class QTextStream;
class QHostInfo;

#ifdef Q_OS_WIN32
 #ifdef BUILD_DLL
  #define DLL_MAKRO __declspec(dllexport)
 #else
  #define DLL_MAKRO __declspec(dllimport)
 #endif
#else
 #define DLL_MAKRO
#endif

class Pop3Private;

class DLL_MAKRO Pop3 : public QObject {

        Q_OBJECT

public:
        /**
         * Constructor
         * @param pop3_server Name or address of Pop3 server
         * @param user User login name
         * @param passwd Password for this login
         */ 
        Pop3 (const QString &user,const QString &passwd, const QString &pop3_server,QObject *parent=0);

        ~Pop3();

        /**
         * This is the only method that should be used. It retreives all mails
         * and for each new mail emits signal @ref newMail. <br>
         * Mails are retreived in asynchronous way.
         * @param DELETE True if you want to delete retreived mails on the server
         */ 

        void getAllMails(bool del=false);

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
        void disconnected(Pop3*);
        
        void newMail(Mail*);

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
        
        Pop3Private* d;
};

#endif
