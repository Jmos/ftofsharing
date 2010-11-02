/****************************************************************************
 *                                                                          *
 *  Author:                                                                 *
 *  -------                                                                 *
 *                                                                          *
 *      Lukasz Iwaszkiewicz   lukasz.iwaszkiewicz@lefthand.com.pl           *
 *                            lukasz.iwaszkiewicz@software.com.pl           *
 *                                                                          *
 *  Date :                                                                  *
 *  ------                                                                  *
 *                                                                          *
 *      28/07/2005                                                          *
 *                                                                          *
 *  Copyright:                                                              *
 *  ----------                                                              *
 *                                                                          *
 *      LeftHand sp. z o. o.    info@lefthand.com.pl                        *
 *                                                                          *
 ****************************************************************************/

#       ifndef LH_MAIL_SOCKET_H
#       define LH_MAIL_SOCKET_H

#       include <QtNetwork>
//#       include "main.h"

#       ifdef USE_QCA
#       include "qca.h"
#       endif

class LHMailSocket;

/**
 *  \see LHMailSocket
 */

class LHMailSocketExtension : public QObject {

        Q_OBJECT

public:

        virtual ~LHMailSocketExtension () {}

        virtual void setOn (bool b) { on = b; }
        virtual bool getOn () const { return on; }

        virtual void onConnected () = 0;
        virtual void onConnectionClosed () = 0;
        virtual void onReadyRead () = 0;
        virtual void onError (int) = 0;

        void setSocket (LHMailSocket *s) { socket = s; }
        LHMailSocket *getSocket () const { return socket; }

        virtual long readBlock (char *data, unsigned long maxlen) = 0;
        virtual long writeBlock (const char *data, unsigned long len) = 0;

private:

        bool on;
        LHMailSocket *socket;

};

/****************************************************************************/

class LHMailSocketExtensionSSL : public LHMailSocketExtension {

        Q_OBJECT

public:

        LHMailSocketExtensionSSL ();
        ~LHMailSocketExtensionSSL ();

        virtual void setOn (bool b);
        void sslHandshake ();

        long readBlock (char *data, unsigned long maxlen);
        long writeBlock (const char *data, unsigned long len);

private slots:

        virtual void onConnected ();
        virtual void onConnectionClosed ();
        virtual void onReadyRead ();
        virtual void onError (int);

        void onSslHandshaken ();
        void onSslReadyRead ();
        void onSslReadyReadOutgoing (int);
        void onSslClosed ();
        void onSslError (int);

private:

#       ifdef USE_QCA
	QCA::TLS *ssl;
	QPtrList <QCA::Cert> rootCerts;
	QCA::Cert cert;
#       endif

        QByteArray output;
};

/**
 *  QSocket wrapper, sort of extensible QSocket. The concept is to provide socket,
 *  that can be easily enriched with some functionality provided by
 *  LHMailSocketExtension. First step is to implement SSL by this
 *  mechanism but in the future maybe it will be suitable to implement
 *  something more.
 */

class LHMailSocket : public QTcpSocket {

        Q_OBJECT

public:

        LHMailSocket (QObject *parent = NULL, const char *name = NULL);
        virtual ~LHMailSocket  ();
        
        void setExtension (LHMailSocketExtension *e, bool b) { autoDeleteExtension = b; extension = e; e->setSocket (this); }
        LHMailSocketExtension *getExtension () const { return extension; }

        virtual void connectToHost (const QString &host, quint16 port);
        QString getHost () const { return host; }

        virtual long readBlock (char *data, unsigned long maxlen);
        virtual long writeBlock (const char *data, unsigned long len);

        void emitReadyRead ();
        void emitError (int);
        void emitConnected ();

signals:

        void connectedMs ();
        void readyReadMs ();
        void errorMs (int);

private slots:

        void onConnected ();
        void onConnectionClosed ();
        void onReadyRead ();
        void onError (int);

private:

        void init ();

        LHMailSocketExtension *extension;
        bool autoDeleteExtension;

        QString host;
};

#       endif

