//=================================================================
// Name:	    	msgQ.h
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		09.10.2010
// Beschreibung:
//=================================================================


#ifndef _MSGQ_H_
#define _MSGQ_H_
//-----------------------------------------------------------------

#define MaxMessageQueCount 		100000
//-----------------------------------------------------------------

#include <iostream>
#include <QMutex>
#include <stdio.h>
#include <conio.h>
#include <iostream>


#include "msgList.h"
//-----------------------------------------------------------------

class TMessageQue
{
  QList<TMessageList *> cMessageQues;
  QMutex cMutex;

  int FreeMessageQueID();
  int MessageQueID2Index(int iMessageQueID);

 public:

  TMessageQue()
  {
  }

  ~TMessageQue()
  {
  }

  int AddMessageQue();
  bool DeleteMessageQue(int iMessageQueID);
  bool SendMessage(int iMessageQueID, int iMyID, int iRemoteID, QString iMessage);
  bool ReceiveMessage(int iMessageQueID, int iMyID, int &oRemoteID, QString &oMessage);
  bool ReceiveMessage(int iMessageQueID, int iMyID, int &oRemoteID, QString &oMessage, QTime &oTimestamp);
};
//-----------------------------------------------------------------

extern TMessageQue MessageQue;
//-----------------------------------------------------------------

//-----------------------------------------------------------------
#endif
