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
#include "smtpclient.h"


using namespace std;


int main (int argc, char** argv)
{

 CSmtpClient smtp;

 REMail mail;

 mail.To= "georg1990@hotmail.com";
 mail.From= "h.saxl@tele.at";
 mail.Subject= "Testmail";
 mail.Body.append("Hallo,");
 mail.Body.append("Das ist eine Testmail.");
 mail.Body.append("Test");
 mail.Body.append("blablabla");

 smtp.SetServerPort(587);
 smtp.SendMail(&mail, "mailserver", "username", "password");


 cout << "\n\nErrorCode: " << smtp.GetLastErrorCode() << endl;

 /*
 CPop3Client pop;

 REMail *mail;

 mail= pop.GetMail(1 ,"pop.utanet.at", "saxlruth", "coolpix");

 if (mail != NULL)
 {
  cout << "\n\nTo: " << qPrintable(mail->To);
  cout << "\nFrom: " << qPrintable(mail->From);
  cout << "\nSubject: " << qPrintable(mail->Subject) << "\n\nBody:\n\n";

  for (int Index= 0; Index < mail->Body.size(); Index++)
      cout << qPrintable(mail->Body.at(Index)) << endl;
 }

 cout << "\n\nErrorCode: " << pop.GetLastErrorCode() << endl;
*/

 getchar();

 return 0xDEAD;
}
