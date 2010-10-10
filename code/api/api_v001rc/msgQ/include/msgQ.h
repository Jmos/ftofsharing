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

#define MaxMessageQCount 		10000   ///< Legt die maximale Anzahl von MessageQs fest.
//-----------------------------------------------------------------

#include <QMutex>

#include "msgList.h"
//-----------------------------------------------------------------
/**
*    @ingroup   api
*
*    Mit dieser Klasse können auf einfache Weise MessageQs erstellt,
*    verwendet und gelöscht werden.
*
*    @brief     Verwaltungsklasse für MessageQs
*    @author    Saxl Georg
*    @version   1.0
*    @date  	\a Begonnnen: 09.10.2010  \n\n
*       	    \a Abgeschlossen:
*
*******************************************************************************/

class TMessageQ
{
  QList<TMessageList *> cMessageQs;
  QMutex cMutex;

  int FreeMessageQID();
  int MessageQID2Index(int iMessageQID);

 public:

  TMessageQ()
  {
  }

  ~TMessageQ()
  {
  }

  int AddMessageQ();
  bool DeleteMessageQ(int iMessageQID);
  bool SendMessage(int iMessageQID, int iMyID, int iRemoteID, QString iMessage);
  bool ReceiveMessage(int iMessageQID, int iMyID, int &oRemoteID, QString &oMessage);
  bool ReceiveMessage(int iMessageQID, int iMyID, int &oRemoteID, QString &oMessage, QTime &oTimestamp);
};
//-----------------------------------------------------------------

extern TMessageQ MessageQ;
//-----------------------------------------------------------------

//-----------------------------------------------------------------
#endif
