//=================================================================
// Name:	    	main.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		14.10.2010
// Beschreibung:
//=================================================================

#include <QStringList>
#include <QDomDocument>
#include <QString>
#include <QFile>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <stdio.h>
#include <iostream>
#include <QObject>

#include "udpsocket.h"


using namespace std;


int main (int argc, char** argv)
{
 CUdpSocket UdpSocket;

 if (!UdpSocket.ListenOnPort(30555))
 {
    cout << "fail";
    getchar();
    return 0;
 }

 UdpSocket.SendText("Hallo Remote", QHostAddress::LocalHost, 30666);

 UdpSocket.WaitForReceiveText();
 cout << qPrintable(UdpSocket.ReceiveText()) << endl;

 getchar();

 return 0;
}
