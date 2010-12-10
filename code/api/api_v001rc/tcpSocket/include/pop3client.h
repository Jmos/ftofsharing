
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
*    @brief     Simple POP3-Client
*    @author    Saxl Georg
*    @version   1.0
*    @date      \a started:    28.10.2010  \n
*               \a completed:  14.11.2010
*
*    With this class you can simply receive E-Mails over POP3.\n
*
*    @par Version 1.1 (05.12.2010)
*         \arg SSL supported
*
*******************************************************************************/
class CPop3Client : public CMailClient
{
  void ParseMailHeader(QString iMailHeader, REMail *oMail);
  bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord);

 public:

  CPop3Client(CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);
  CPop3Client(QString iHostNamem, CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);
  CPop3Client(QString iHostName, QString iUserName, QString iPassWord, CTcpSocket::EConnectionType iConnectionType= CTcpSocket::NONCRYPTEDMODE);

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
