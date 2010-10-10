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
#include <conio.h>
#include <iostream>

#include "msgQ.h"

using namespace std;

int main (int argc, char** argv)
{
 int msgQID;

 msgQID= MessageQue.AddMessageQue();

 cout << "MessageQueID: " << msgQID << endl;
 getch();

 MessageQue.SendMessage(msgQID, 5, 6, "Hallo, das ist ein Test!");
 MessageQue.SendMessage(msgQID, 5, 6, "Hallo, das ist noch ein Test!");

 cout << "Send OK" << endl;
 getch();

 int RxID;
 QString Message;

 MessageQue.ReceiveMessage(msgQID, 6, RxID, Message);
 std::cout << RxID << " sagt: " << qPrintable(Message) << endl;

 MessageQue.ReceiveMessage(msgQID, 6, RxID, Message);
 std::cout << RxID << " sagt: " << qPrintable(Message) << endl;

 getch();

 MessageQue.DeleteMessageQue(msgQID);
 cout << "Deleted.";

 getch();

 return 0;
}
