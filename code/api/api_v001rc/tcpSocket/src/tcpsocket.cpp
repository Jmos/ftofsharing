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

 cSocketStream= new QTextStream(&cTcpSocket);

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

 //std::cout << "Bytes: " << cTcpSocket.bytesAvailable() << std::endl;

 QByteArray RxBuffer(cTcpSocket.bytesAvailable(), 0);

 RxBuffer= cTcpSocket.read(RxBuffer.size());

 QString RxData(RxBuffer);

 return RxData;
}
//-----------------------------------------------------------------

bool CTcpSocket::ReceiveLines(QList<QString> &oStringList)
{
 if (!cConnected)
    return false;

 if (cTcpSocket.bytesAvailable() == 0)
    return false;

 QString RxData;

 //std::cout << "Bytes: " << cTcpSocket.bytesAvailable() << std::endl;

 while (cTcpSocket.bytesAvailable() > 0)
 {
  oStringList.append(cTcpSocket.readLine());
 }

 return true;
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




