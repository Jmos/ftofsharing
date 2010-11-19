
#ifndef _POP3CLIENT_H_
#define _POP3CLIENT_H_
//-----------------------------------------------------------------


#include <QString>
#include <QList>
#include "mailclient.h"
//-----------------------------------------------------------------


/**
*    @ingroup   api
*
*    ....
*
*    @brief     ...
*    @author    Saxl Georg
*    @version   1.0
*    @date      \a Begonnnen: 28.10.2010  \n\n
*               \a Abgeschlossen:
*
*******************************************************************************/

class CPop3Client : public CMailClient
{
  void ParseMailHeader(QString iMailHeader, REMail *oMail);
  bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord);

 public:

  CPop3Client();
  CPop3Client(QString iHostName);
  CPop3Client(QString iHostName, QString iUserName, QString iPassWord);

  int GetMailCount();
  int GetMailCount(QString iUserName, QString iPassWord);
  int GetMailCount(QString iHostName, QString iUserName, QString iPassWord);

  REMail *GetMail(int iMailIndex);
  REMail *GetMail(int iMailIndex, QString iUserName, QString iPassWord);
  REMail *GetMail(int iMailIndex, QString iHostName, QString iUserName, QString iPassWord);
};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
