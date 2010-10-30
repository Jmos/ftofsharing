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
#include <iostream>


#include "msgQ.h"

using namespace std;

int main (int argc, char** argv)
{
 int msgQID;

 msgQID= MessageQ.AddMessageQ();

 cout << "MessageQID: " << msgQID << endl;
 getchar();

 MessageQ.SendMessage(msgQID, 6, "Hallo, das ist ein Test!");
 MessageQ.SendMessage(msgQID, 6, "Hallo, das ist noch ein Test!");

 cout << "Send OK" << endl;

 getchar();

 QString Message;

 MessageQ.ReceiveMessage(msgQID, 6, Message);
 std::cout << "Message1: " << qPrintable(Message) << endl;

 MessageQ.ReceiveMessage(msgQID, 6, Message);
 std::cout << "Message2: " << qPrintable(Message) << endl;

 getchar();

 MessageQ.DeleteMessageQ(msgQID);

 getchar();
 return 0;
}
