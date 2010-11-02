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

#       include "lhmailsocket.h"

/****************************************************************************/

LHMailSocketExtensionSSL::LHMailSocketExtensionSSL ()
{
#       ifdef USE_QCA

	if (!QCA::isSupported (QCA::CAP_TLS))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "TLS not supported!");

        ssl = new QCA::TLS;
#       endif
}

/****************************************************************************/

LHMailSocketExtensionSSL::~LHMailSocketExtensionSSL ()
{
#       ifdef USE_QCA
        delete ssl;
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::setOn (bool b)
{
        LHMailSocketExtension::setOn (b);

#       ifdef USE_QCA

        if (!connect (ssl, SIGNAL (handshaken ()), SLOT (onSslHandshaken ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
        if (!connect (ssl, SIGNAL (readyRead ()), SLOT (onSslReadyRead ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
        if (!connect (ssl, SIGNAL (readyReadOutgoing (int)), SLOT (onSslReadyReadOutgoing (int))))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
        if (!connect (ssl, SIGNAL (closed ()), SLOT (onSslClosed ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
        if (!connect (ssl, SIGNAL (error (int)), SLOT (onSslError (int))))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                        
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::sslHandshake ()
{
#       ifdef USE_QCA

        ssl->setCertificateStore (rootCerts);
        ssl->startClient (getSocket ()->getHost ());
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onSslHandshaken ()
{
#       ifdef USE_QCA
        cert = ssl->peerCertificate();
        int vr = ssl->certificateValidityResult();

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Successful SSL handshake");
      
        if(!cert.isNull())
                ;// showCertInfo(cert);
         
        if(vr == QCA::TLS::Valid)
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Valid certificate.");
        else
                // qDebug ("*** %s,%d : %s%s", __FILE__, __LINE__, "Invalid certificate : ", resultToString (vr).latin1 ());
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Invalid certificate : ");

        getSocket ()->emitConnected ();
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onSslReadyRead ()
{
#       ifdef USE_QCA
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocketExtensionSSL::onSslReadyRead ()");
        output = ssl->read();
        getSocket ()->emitReadyRead ();
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onSslReadyReadOutgoing (int)
{
#       ifdef USE_QCA

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocketExtensionSSL::onSslReadyReadOutgoing (int)");
        QByteArray a = ssl->readOutgoing ();
        getSocket ()->writeBlock (a.data (), a.size ());
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onSslClosed ()
{
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onSslError (int i)
{
        getSocket ()->emitError (i);
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onConnected ()
{
        // emitConnected ();
        sslHandshake ();
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onConnectionClosed ()
{
        // emitConnectionClosed ();
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onReadyRead ()
{
#       ifdef USE_QCA

        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocketExtensionSSL::onReadyRead ()");
        QByteArray buf (getSocket ()->bytesAvailable ());
                
        int num = getSocket ()->QSocket::readBlock (buf.data (), buf.size ());
                
        if (num < (int)buf.size ())
                buf.resize (num);
                        
        ssl->writeIncoming (buf);
        qDebug ("*** %s,%d : %d", __FILE__, __LINE__, buf.size ());
        
#       endif
}

/****************************************************************************/

void LHMailSocketExtensionSSL::onError (int i)
{
        getSocket ()->emitError (i);
}

/****************************************************************************/

long LHMailSocketExtensionSSL::readBlock (char *data, unsigned long maxlen)
{
        memcpy (data, output.data (), maxlen);
        return output.size ();
}

/****************************************************************************/

long LHMailSocketExtensionSSL::writeBlock (const char *data, unsigned long len)
{
#       ifdef USE_QCA
        QByteArray b (len);
        memcpy (b.data (), data, len);
        ssl->write (b);
#       endif
}

/*##########################################################################*/

LHMailSocket::LHMailSocket (QObject *parent, const char *name) : QTcpSocket (parent)
{
        // socket = new QSocket (parent, name);
        init ();
}

/****************************************************************************/

LHMailSocket::~LHMailSocket  ()
{
        if (autoDeleteExtension)
                delete extension;
}

/****************************************************************************/

void LHMailSocket::init ()
{
        extension = NULL;
        autoDeleteExtension = false;

        if (!connect (this, SIGNAL (connected ()), SLOT (onConnected ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
	if (!connect (this, SIGNAL (readyRead ()), SLOT (onReadyRead ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
        
	if (!connect (this, SIGNAL (connectionClosed ()), SLOT (onConnectionClosed ())))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
        
	if (!connect (this, SIGNAL (error (int)),  SLOT (onError (int))))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");

}

/****************************************************************************/

void LHMailSocket::connectToHost (const QString &h, quint16 p)
{
        QTcpSocket::connectToHost(h, p);
        host = h;
}

/****************************************************************************/

long LHMailSocket::readBlock (char *data, unsigned long maxlen)
{
        if (getExtension ())
                return getExtension ()->readBlock (data, maxlen);

        return QTcpSocket::read(data, maxlen);
}

/****************************************************************************/

long LHMailSocket::writeBlock (const char *data, unsigned long len)
{
        if (getExtension ())
                return getExtension ()->writeBlock (data, len);

        return QTcpSocket::write(data, len);
}

/****************************************************************************/

void LHMailSocket::onConnected ()
{
        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocket::onConnected");

        if (getExtension ())
                getExtension ()->onConnected ();
        else
                emitConnected ();
}

/****************************************************************************/

void LHMailSocket::onConnectionClosed ()
{
        if (getExtension ())
                getExtension ()->onConnectionClosed ();
}

/****************************************************************************/

void LHMailSocket::onReadyRead ()
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocket::onReadyRead ()");
        
        if (getExtension ())
                getExtension ()->onReadyRead ();

        else
                emitReadyRead ();
}

/****************************************************************************/

void LHMailSocket::onError (int i)
{
        if (getExtension ())
                getExtension ()->onError (i);

        else
                emitError (i);
}

/****************************************************************************/

void LHMailSocket::emitReadyRead ()
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "LHMailSocket::emitReadyRead ()");
        emit (readyReadMs ());
}

/****************************************************************************/

void LHMailSocket::emitConnected ()
{
        emit (connectedMs ());
}

/****************************************************************************/
/*
void LHMailSocket::emitConnectionClosed ()
{
        emit (connectionClosed ());
}
*/
/****************************************************************************/

void LHMailSocket::emitError (int i)
{
        emit (errorMs (i));
}

