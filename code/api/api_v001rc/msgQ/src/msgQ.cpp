

#include "msgQ.h"
//-----------------------------------------------------------------

/**
*  Erstellt eine neue MessageQ.
*
*  @return	Liefert bei Erfolg eine einzigartige MessageQID, ansonsten -1.
*
********************************************************************************/
int TMessageQ::AddMessageQ()
{
 cMutex.lock();


 int MessageQID= FreeMessageQID();

 if (MessageQID == -1)
    {
     cMutex.unlock();
     return -1;
    }

 TMessageList *MessageList = new TMessageList(MessageQID);
 cMessageQs.append(MessageList);


 cMutex.unlock();

 return MessageQID;
}

/**
*  Löscht eine MessageQ
*
*  @param	iMessageQID Gibt an welche MessageQ gelöscht werden soll.
*  @return	Bei Erfolg true, ansonsten false.
*
********************************************************************************/
bool TMessageQ::DeleteMessageQ(int iMessageQID)
{
 cMutex.lock();


 for (int Index= 0; Index < MaxMessageQCount; Index++)
     {
      if (cMessageQs.at(Index)->GetMessageQID() == iMessageQID)
         {
          delete cMessageQs.at(Index);
          cMessageQs.removeAt(Index);
          cMutex.unlock();
          return true;
         }
     }


 cMutex.unlock();

 return false;
}

/**
*  Sendet eine Nachricht an eine MessageQ
*
*  @param   iMessageQID Gibt an welche MessageQ verwendet wird.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQ einzigartig sein)
*  @param   iRemoteID Gibt die ID des Empfängers an. (Muss in einer MessageQ einzigartig sein)
*  @param   iMessage Gibt den Inhalt der Message an.
*  @return	Bei Erfolg true, ansonsten false.
*
********************************************************************************/
bool TMessageQ::SendMessage(int iMessageQID, int iMyID, int iRemoteID, QString iMessage)
{
 cMutex.lock();

 int Index= MessageQID2Index(iMessageQID);

 if (Index != -1)
	{
     cMessageQs.at(Index)->SendMessage(iMyID, iRemoteID, iMessage);
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
*  Empfängt eine Nachricht aus einer MessageQ
*
*  @param	iMessageQID Gibt die zu verwendente MessageQ an.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQ einzigartig sein).
*  @param   oRemoteID Liefert die ID des Absenders an.
*  @param   oMessage Liefert die empfangene Nachricht.
*  @param   oTimestamp Liefert den Zeitpunkt an dem die Nachricht gesendet wurde.
*  @return	Liefert ture wenn eine Nachricht empfangen wurde, ansonsten false.
*
********************************************************************************/
bool TMessageQ::ReceiveMessage(int iMessageQID, int iMyID, int &oRemoteID, QString &oMessage, QTime &oTimestamp)
{
 cMutex.lock();

 int Index= MessageQID2Index(iMessageQID);

 if (Index != -1)
    {
     cMessageQs.at(Index)->ReceiveMessage(iMyID, oMessage, oRemoteID, oTimestamp);
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
*  Empfängt eine Nachricht aus einer MessageQ
*
*  @param   iMessageQID Gibt die zu verwendente MessageQ an.
*  @param   iMyID Gibt die eigene ID an. (Muss in einer MessageQ einzigartig sein).
*  @param   oRemoteID Liefert die ID des Absenders an.
*  @param   oMessage Liefert die empfangene Nachricht.
*  @return  Liefert ture wenn eine Nachricht empfangen wurde, ansonsten false.
*
********************************************************************************/
bool TMessageQ::ReceiveMessage(int iMessageQID, int iMyID, int &oRemoteID, QString &oMessage)
{
 QTime Timestamp;

 return ReceiveMessage(iMessageQID, iMyID, oRemoteID, oMessage, Timestamp);
}
//-----------------------------------------------------------------

int TMessageQ::FreeMessageQID()
{
 bool freeID;

 for (int MessageQID= 0; MessageQID <= MaxMessageQCount; MessageQID++)
     {
      freeID= true;

      for (int Index= 0; Index < cMessageQs.size(); Index++)
          {
           if (cMessageQs.at(Index)->GetMessageQID() == MessageQID)
              {
               freeID= false;
               break;
              }
          }

      if (freeID)
         return MessageQID;
     }

 return -1;
}
//-----------------------------------------------------------------

int TMessageQ::MessageQID2Index(int iMessageQID)
{
 for (int Index= 0; Index < cMessageQs.size(); Index++)
     {
      if (cMessageQs.at(Index)->GetMessageQID() == iMessageQID)
         return Index;
     }

 return -1;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------

TMessageQ MessageQ;
//-----------------------------------------------------------------
//-----------------------------------------------------------------
