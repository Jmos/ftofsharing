

#include "msgList.h"
//-----------------------------------------------------------------


void TMessageList::SendMessage(int TxID, int RxID, QString MessageString)
{
 RMessage Message;

 Message.Message= MessageString;
 Message.Timestamp= QTime::currentTime();
 Message.Transmitter= TxID;
 Message.Receiver= RxID;

 cMessageList.append(Message);
}
//-----------------------------------------------------------------

void TMessageList::SendMessage(int TxID, QList<int> RxID, QString MessageString)
{
 RMessage Message;

 Message.Message= MessageString;
 Message.Timestamp= QTime::currentTime();
 Message.Transmitter= TxID;

 for (int Index= 0; Index < RxID.size(); Index++)
	{
	 Message.Receiver= RxID.at(Index);
	 cMessageList.append(Message);
	}
}
//-----------------------------------------------------------------

bool TMessageList::ReceiveMessage(int MyID, QString &Message, int &TxID, QTime &Timestamp)
{
 for (int Index= cMessageList.size()-1; Index >= 0; Index--)
	 {
	  if (cMessageList.at(Index).Receiver == MyID);
		 {
		  Message= cMessageList.at(Index).Message;
		  TxID= cMessageList.at(Index).Transmitter;
		  Timestamp= cMessageList.at(Index).Timestamp;

		  cMessageList.removeAt(Index);

		  return true;
		 }
	 }

 return false;
}
//-----------------------------------------------------------------

void TMessageList::ClearMessageList()
{
 cMessageList.clear();
}
//-----------------------------------------------------------------

int TMessageList::GetMessageQID()
{
 return cMessageQID;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------


