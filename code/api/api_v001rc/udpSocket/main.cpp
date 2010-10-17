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
#ifdef WIN32
#include <conio.h>
#endif

#include "udpsocket.h"

using namespace std;


int main (int argc, char** argv)
{
 TUdpSocket UdpSocket;

 if (UdpSocket.ListenOnPort(30555))
    cout << "UDP Socket!" << endl;


 getchar();

 return 0;
}
