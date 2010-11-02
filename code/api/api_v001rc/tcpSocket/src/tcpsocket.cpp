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

 cTcpSocket.setSocketOption(QAbstractSocket::KeepAliveOption, false);

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

 return cTcpSocket.waitForConnected();
}
//-----------------------------------------------------------------

bool CTcpSocket::ConnectTo(QString iHostName, int iPort, int iTimeoutMS)
{
 if (cConnected)
    return false;

 cTcpSocket.connectToHost(iHostName, (quint16)iPort);

 return cTcpSocket.waitForConnected(iTimeoutMS);
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

QString CTcpSocket::ReceiveText(int iTimeOutMs)
{
 if (!cConnected)
    return "";

 QString RxData;

 if (cTcpSocket.waitForReadyRead(iTimeOutMs))
    {
     int CharIndex;

     RxData= cTcpSocket.readAll();

     CharIndex= RxData.indexOf("\n");

     if (CharIndex != -1)
        RxData= RxData.left(CharIndex);
    }

 return RxData;
}
//-----------------------------------------------------------------

bool CTcpSocket::ReceiveLines(QList<QString> &oStringList, int iTimeOutMs)
{
 if (!cConnected)
    return false;

 QString RxData;
 int     CharIndex;
 bool    RetrunValue= false;

 while (cTcpSocket.waitForReadyRead(iTimeOutMs))
 {
  RxData= cTcpSocket.readAll();

  RetrunValue= true;

  CharIndex= RxData.indexOf("\n.\r\n");

  if (CharIndex != -1)
     RxData= RxData.left(CharIndex);

 oStringList.append(RxData);

 if (CharIndex != -1)
     break;
 }

 return RetrunValue;
}
//-----------------------------------------------------------------



