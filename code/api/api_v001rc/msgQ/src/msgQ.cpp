

#include "msgQ.h"
//-----------------------------------------------------------------


int TMessageQue::AddMessageQue()
{
 cMutex.lock();


 int MessageQueID= FreeMessageQueID();

 if (MessageQueID == -1)
    {
     cMutex.unlock();
     return -1;
    }

 TMessageList *MessageList = new TMessageList(MessageQueID);
 cMessageQues.append(MessageList);


 cMutex.unlock();

 return MessageQueID;
}
//-----------------------------------------------------------------

bool TMessageQue::DeleteMessageQue(int iMessageQueID)
{
 cMutex.lock();


 for (int Index= 0; Index < MaxMessageQueCount; Index++)
     {
      if (cMessageQues.at(Index)->GetMessageQueID() == iMessageQueID)
         {
          delete cMessageQues.at(Index);
          cMessageQues.removeAt(Index);
          cMutex.unlock();
          return true;
         }
     }


 cMutex.unlock();

 return false;
}
//-----------------------------------------------------------------

bool TMessageQue::SendMessage(int iMessageQueID, int iMyID, int iRemoteID, QString iMessage)
{
 cMutex.lock();

 int Index= MessageQueID2Index(iMessageQueID);

 if (Index != -1)
	{
     cMessageQues.at(Index)->SendMessage(iMyID, iRemoteID, iMessage);
	}
    else
       {
        cMutex.unlock();
        return false;
       }

 cMutex.unlock();

 return true;
}
//-----------------------------------------------------------------

bool TMessageQue::ReceiveMessage(int iMessageQueID, int iMyID, int &oRemoteID, QString &oMessage, QTime &oTimestamp)
{
 cMutex.lock();

 int Index= MessageQueID2Index(iMessageQueID);

 if (Index != -1)
    {
     cMessageQues.at(Index)->ReceiveMessage(iMyID, oMessage, oRemoteID, oTimestamp);
    }
    else
       {
        cMutex.unlock();
        return false;
       }

 cMutex.unlock();

 return true;
}
//-----------------------------------------------------------------

bool TMessageQue::ReceiveMessage(int iMessageQueID, int iMyID, int &oRemoteID, QString &oMessage)
{
 QTime Timestamp;

 return ReceiveMessage(iMessageQueID, iMyID, oRemoteID, oMessage, Timestamp);
}
//-----------------------------------------------------------------

int TMessageQue::FreeMessageQueID()
{
 bool freeID;

 for (int MessageQueID= 0; MessageQueID <= MaxMessageQueCount; MessageQueID++)
     {
      freeID= true;

      for (int Index= 0; Index < cMessageQues.size(); Index++)
          {
           if (cMessageQues.at(Index)->GetMessageQueID() == MessageQueID)
              {
               freeID= false;
               break;
              }
          }

      if (freeID)
         return MessageQueID;
     }

 return -1;
}
//-----------------------------------------------------------------

int TMessageQue::MessageQueID2Index(int iMessageQueID)
{
 for (int Index= 0; Index < cMessageQues.size(); Index++)
     {
      if (cMessageQues.at(Index)->GetMessageQueID() == iMessageQueID)
         return Index;
     }

 return -1;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------

TMessageQue MessageQue;
//-----------------------------------------------------------------
//-----------------------------------------------------------------
