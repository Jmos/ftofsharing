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

#include <QString>
#include <QList>
#include <QTime>
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
  int cMessageQID;

 public:

  TMessageList(int iMessageQID)
  {
   cMessageQID= iMessageQID;
  }

  ~TMessageList()
  {
  }

  void SendMessage(int TxID, int RxID, QString MessageString);
  void SendMessage(int TxID, QList<int> RxID, QString MessageString);
  bool ReceiveMessage(int PeerID, QString &Message, int &RxID, QTime &Timestamp);
  void ClearMessageList();
  int GetMessageQID();
};

//-----------------------------------------------------------------
#endif


