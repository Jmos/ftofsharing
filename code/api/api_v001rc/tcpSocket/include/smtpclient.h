
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
*    @brief     Simple SMTP-Client
*    @author    Saxl Georg
*    @version   1.0
*    @date      \a started:    19.11.2010  \n
*               \a completed:  04.12.2010
*
*    With this class you can simply send E-Mails over SMTP.\n
*
*    @par Version 1.1 (05.12.2010)
*         \arg SSL supported
*
*******************************************************************************/
class CSmtpClient : public CMailClient
{
  bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord);

 public:

  CSmtpClient(CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);
  CSmtpClient(QString iHostName, CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);
  CSmtpClient(QString iHostName, QString iUserName, QString iPassWord, CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);

  bool SendMail(REMail *iMail);
  bool SendMail(REMail *iMail, QString iUserName, QString iPassWord);
  bool SendMail(REMail *iMail, QString iHostName, QString iUserName, QString iPassWord);
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
