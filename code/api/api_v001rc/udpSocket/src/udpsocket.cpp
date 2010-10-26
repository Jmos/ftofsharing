//=================================================================
// Name:	    	udpsocket.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		14.10.2010
// Beschreibung:
//=================================================================

#include "udpsocket.h"
//-----------------------------------------------------------------

void CUdpSocket::SetSenderPort(int iPort)
{
 setLocalPort((quint16)iPort);
}
//-----------------------------------------------------------------

int CUdpSocket::GetSenderPort()
{
 return (int)localPort();
}
//-----------------------------------------------------------------

bool CUdpSocket::ListenOnPort(int iPort)
{
 return bind(iPort, QUdpSocket::DontShareAddress);
}
//-----------------------------------------------------------------

bool CUdpSocket::SendText(QString iData, QHostAddress iReceiverAddress, int iReceiverPort)
{
 if (writeDatagram(qPrintable(iData), iData.length(), iReceiverAddress, iReceiverPort) != -1)
    return true;

 return false;
}
//----------------------------------------------------------------

QString CUdpSocket::ReceiveText()
{
 if (!hasPendingDatagrams())
    return "";

 QByteArray RxBuffer(pendingDatagramSize(), 0);
 QHostAddress HostAddress;
 quint16 Port;

 readDatagram(RxBuffer.data(), RxBuffer.size(), &HostAddress, &Port);

 QString RxData(RxBuffer);

 return RxData;
}
//-----------------------------------------------------------------

bool CUdpSocket::WaitForReceiveText()
{
 return waitForReadyRead();
}
//-----------------------------------------------------------------

bool CUdpSocket::WaitForReceiveText(int iMilliSeconds)
{
 return waitForReadyRead(iMilliSeconds);
}
//-----------------------------------------------------------------
