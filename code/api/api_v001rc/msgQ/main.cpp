//=================================================================
// Name:	    	main.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		09.10.2010
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
#ifdef WIN32
#include <conio.h>
#endif
#include <iostream>

#include "msgQ.h"

using namespace std;

int main (int argc, char** argv)
{
 int msgQID;

 msgQID= MessageQ.AddMessageQ();

 cout << "MessageQID: " << msgQID << endl;
#ifdef WIN32
 getch();
#else
 getchar();
#endif

 MessageQ.SendMessage(msgQID, 5, 6, "Hallo, das ist ein Test!");
 MessageQ.SendMessage(msgQID, 5, 6, "Hallo, das ist noch ein Test!");

 cout << "Send OK" << endl;

#ifdef WIN32
 getch();
#else
 getchar();
#endif

 int RxID;
 QString Message;

 MessageQ.ReceiveMessage(msgQID, 6, RxID, Message);
 std::cout << RxID << " sagt: " << qPrintable(Message) << endl;

 MessageQ.ReceiveMessage(msgQID, 6, RxID, Message);
 std::cout << RxID << " sagt: " << qPrintable(Message) << endl;

#ifdef WIN32
 getch();
#else
 getchar();
#endif

 MessageQ.DeleteMessageQ(msgQID);

#ifdef WIN32
 getch();
#else
 getchar();
#endif
 return 0;
}
