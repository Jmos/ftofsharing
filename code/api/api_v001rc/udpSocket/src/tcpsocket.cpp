//=================================================================
// Name:	    	tcpsocket.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		27.10.2010
// Beschreibung:
//=================================================================

#include "tcpsocket.h"
//-----------------------------------------------------------------

CTcpSocket::CTcpSocket()
{
 cConnected= false;

 //connect(&cTcpSocket, SIGNAL(disconnected()), &cTcpSocket, SLOT(deleteLater()));
 connect(&cTcpSocket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
 connect(&cTcpSocket, SIGNAL(connected()), this, SLOT(Connected()));
}
//-----------------------------------------------------------------

void CTcpSocket::Connected()
{
 cConnected= true;
}
//-----------------------------------------------------------------

void CTcpSocket::Disconnected()
{
 cConnected= false;
}
//-----------------------------------------------------------------

bool CTcpSocket::ConnectTo(QHostAddress iHostAddress, int iPort, int iTimeoutMS)
{
 if (cConnected)
    return false;

 cTcpSocket.connectToHost(iHostAddress, (quint16)iPort);

 return cTcpSocket.waitForConnected();//iTimeoutMS);
}
//-----------------------------------------------------------------

bool CTcpSocket::ConnectTo(QString iHostName, int iPort, int iTimeoutMS)
{
 if (cConnected)
    return false;

 cTcpSocket.connectToHost(iHostName, (quint16)iPort);

 return cTcpSocket.waitForConnected();//iTimeoutMS);
}
//-----------------------------------------------------------------

void CTcpSocket::Disconnect()
{
 if (!cConnected)
    return;

 cTcpSocket.disconnectFromHost();
}
//-----------------------------------------------------------------

bool CTcpSocket::IsConnected()
{
 return cConnected;
}
//-----------------------------------------------------------------

bool CTcpSocket::SendText(QString iData)
{
 if (!cConnected)
    return false;

 if (cTcpSocket.write(qPrintable(iData), iData.size()) != -1)
    {
     cTcpSocket.flush();
     return true;
    }

 return false;
}
//-----------------------------------------------------------------
bool CTcpSocket::WaitForReceiveText()
{
 if (!cConnected)
    return false;

 if (cTcpSocket.bytesAvailable() > 0)
    return true;

 return cTcpSocket.waitForReadyRead();
}
//-----------------------------------------------------------------

bool CTcpSocket::WaitForReceiveText(int iTimeOutMs)
{
 if (!cConnected)
    return false;

 if (cTcpSocket.bytesAvailable() > 0)
    return true;

 return cTcpSocket.waitForReadyRead(iTimeOutMs);
}
//-----------------------------------------------------------------

QString CTcpSocket::ReceiveText()
{
 if (!cConnected)
    return "";

 if (cTcpSocket.bytesAvailable() == 0)
    return "";

 QByteArray RxBuffer(cTcpSocket.bytesAvailable(), 0);

 RxBuffer= cTcpSocket.read(RxBuffer.size());

 QString RxData(RxBuffer);

 return RxData;
}
//-----------------------------------------------------------------

QString CTcpSocket::WaitAndReceiveText()
{
 if (!WaitForReceiveText())
    return "";

 return ReceiveText();
}
//-----------------------------------------------------------------

QString CTcpSocket::WaitAndReceiveText(int iTimeOutMs)
{
 if (!WaitForReceiveText(iTimeOutMs))
    return "";

 return ReceiveText();
}
//-----------------------------------------------------------------




