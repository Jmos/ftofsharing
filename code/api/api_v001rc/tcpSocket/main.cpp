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

 int i= pop.GetMailCount("pop.utanet.at", "saxlruth", "cibfhr1m");

 cout << i << endl;



 getchar();

 return 0xDEAD;
}
