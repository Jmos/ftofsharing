
#ifndef _SMTPCLIENT_H_
#define _SMTPCLIENT_H_
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
*    @date      \a Begonnnen: 19.11.2010  \n\n
*               \a Abgeschlossen:
*
*******************************************************************************/
class CSmtpClient : public CMailClient
{
  bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord);

 public:

  CSmtpClient();
  CSmtpClient(QString iHostName);
  CSmtpClient(QString iHostName, QString iUserName, QString iPassWord);

  bool SendMail(REMail *iMail);
  bool SendMail(REMail *iMail, QString iUserName, QString iPassWord);
  bool SendMail(REMail *iMail, QString iHostName, QString iUserName, QString iPassWord);
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
