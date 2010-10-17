//=================================================================
// Name:	    	udpsocket.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		14.10.2010
// Beschreibung:
//=================================================================

#include "udpsocket.h"
//-----------------------------------------------------------------

TUdpSocket::TUdpSocket()
{
 connect(this, SIGNAL(readyRead()), this, SLOT(OnReadRead()));
}
//-----------------------------------------------------------------

void TUdpSocket::SetLocalPort(int iPort)
{
 setLocalPort((quint16)iPort);
}
//-----------------------------------------------------------------

int TUdpSocket::GetLocalPort()
{
 return (int)localPort();
}
//-----------------------------------------------------------------

bool TUdpSocket::ListenOnPort(int iPort)
{
 return bind((quint16)iPort, QUdpSocket::DontShareAddress);
}
//-----------------------------------------------------------------

bool TUdpSocket::SendText(QString iData, QHostAddress iReceiverAddress, int iReceiverPort)
{
 if (writeDatagram(qPrintable(iData), iData.length(), iReceiverAddress, iReceiverPort) != -1)
    return true;

 return false;
}
//-----------------------------------------------------------------

void TUdpSocket::OnReadRead()
{
 QApplication::beep ();

 QByteArray RxBuffer(pendingDatagramSize(), 0);
 QHostAddress HostAddress;
 quint16 Port;

 readDatagram(RxBuffer.data(), RxBuffer.size(), &HostAddress, &Port);

 QString RxData(RxBuffer);

 std::cout << qPrintable(RxData);

 emit OnSocketRead(RxData, HostAddress, (int)Port);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
