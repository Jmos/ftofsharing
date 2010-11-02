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

#include "pop3client.h"


using namespace std;


int main (int argc, char** argv)
{
 CPop3Client pop;

 REMail *mail;

 mail= pop.GetMail(1 ,"popserver", "username", "password");

 if (mail != NULL)
 {
  cout << "\n\nTo: " << qPrintable(mail->To);
  cout << "\nFrom: " << qPrintable(mail->From);
  cout << "\nSubject: " << qPrintable(mail->Subject) << "\n\nBody:\n\n";

  for (int Index= 0; Index < mail->Body.size(); Index++)
      cout << qPrintable(mail->Body.at(Index)) << endl;
 }

 cout << "\n\nErrorCode: " << pop.GetLastErrorCode() << endl;

 getchar();

 return 0xDEAD;
}
