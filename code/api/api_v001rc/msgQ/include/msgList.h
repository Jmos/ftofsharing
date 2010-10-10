//=================================================================
// Name:	    	msgQ.h
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		09.10.2010
// Beschreibung:
//=================================================================

#ifndef _MSGLIST_H_
#define _MSGLIST_H_
//-----------------------------------------------------------------

#include <QMutex>
#include <QString>
#include <QList>
#include <QTime>
#include <stdio.h>
#include <conio.h>
#include <iostream>
//-----------------------------------------------------------------

struct RMessage
{
 int Transmitter;
 int Receiver;
 QString Message;
 QTime Timestamp;
};


class TMessageList
{
  QList<RMessage> cMessageList;
  int cMessageQueID;

 public:

  TMessageList(int iMessageQueID)
  {
   cMessageQueID= iMessageQueID;
  }

  ~TMessageList()
  {
  }

  void SendMessage(int TxID, int RxID, QString MessageString);
  void SendMessage(int TxID, QList<int> RxID, QString MessageString);
  bool ReceiveMessage(int PeerID, QString &Message, int &RxID, QTime &Timestamp);
  void ClearMessageList();
  int GetMessageQueID();
};

//-----------------------------------------------------------------
#endif


