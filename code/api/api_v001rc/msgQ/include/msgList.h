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
 int ReceiverID;
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

  void SendMessage(int iReceiverID, QString iMessage);
  void SendMessage(QList<int> ReceiverID, QString iMessage);
  bool ReceiveMessage(int iMyID, QString &oMessage, QTime &oTimestamp);
  void ClearMessageList();
  int GetMessageQID();
};

//-----------------------------------------------------------------
#endif


