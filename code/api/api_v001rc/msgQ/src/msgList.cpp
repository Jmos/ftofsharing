

#include "msgList.h"
//-----------------------------------------------------------------


void TMessageList::SendMessage(int iReceiverID, QString iMessage)
{
 RMessage Message;

 Message.Message= iMessage;
 Message.Timestamp= QTime::currentTime();
 Message.ReceiverID= iReceiverID;

 cMessageList.append(Message);
}
//-----------------------------------------------------------------

void TMessageList::SendMessage(QList<int> iReceiverID, QString iMessage)
{
 RMessage Message;

 Message.Message= iMessage;
 Message.Timestamp= QTime::currentTime();

 for (int Index= 0; Index < iReceiverID.size(); Index++)
	{
	 Message.ReceiverID= iReceiverID.at(Index);
	 cMessageList.append(Message);
	}
}
//-----------------------------------------------------------------

bool TMessageList::ReceiveMessage(int iMyID, QString &oMessage, QTime &oTimestamp)
{
 for (int Index= 0; Index < cMessageList.size(); Index++)
	 {
	  if (cMessageList.at(Index).ReceiverID == iMyID);
		 {
		  oMessage= cMessageList.at(Index).Message;
		  oTimestamp= cMessageList.at(Index).Timestamp;

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


