

#include "msgQ.h"
//-----------------------------------------------------------------

/**
*  Erstellt eine neue MessageQue.
*
*  @return	Liefert bei Erfolg eine einzigartige MessageQueID, ansonsten -1.
*
********************************************************************************/
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

/**
*  Löscht eine MessageQue
*
*  @param	iMessageQueID Gibt an welche MessageQue gelöscht werden soll.
*  @return	Bei Erfolg true, ansonsten false.
*
********************************************************************************/
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

/**
*  Sendet eine Nachricht an eine MessageQue
*
*  @param   iMessageQueID Gibt an welche MessageQue verwendet wird.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQue einzigartig sein)
*  @param   iRemoteID Gibt die ID des Empfängers an. (Muss in einer MessageQue einzigartig sein)
*  @param   iMessage Gibt den Inhalt der Message an.
*  @return	Bei Erfolg true, ansonsten false.
*
********************************************************************************/
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

/**
*  Empfängt eine Nachricht aus einer MessageQue
*
*  @param	iMessageQueID Gibt die zu verwendente MessageQue an.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQue einzigartig sein).
*  @param   oRemoteID Liefert die ID des Absenders an.
*  @param   oMessage Liefert die empfangene Nachricht.
*  @param   oTimestamp Liefert den Zeitpunkt an dem die Nachricht gesendet wurde.
*  @return	Liefert ture wenn eine Nachricht empfangen wurde, ansonsten false.
*
********************************************************************************/
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

/**
*  Empfängt eine Nachricht aus einer MessageQue
*
*  @param   iMessageQueID Gibt die zu verwendente MessageQue an.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQue einzigartig sein).
*  @param   oRemoteID Liefert die ID des Absenders an.
*  @param   oMessage Liefert die empfangene Nachricht.
*  @return  Liefert ture wenn eine Nachricht empfangen wurde, ansonsten false.
*
********************************************************************************/
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
