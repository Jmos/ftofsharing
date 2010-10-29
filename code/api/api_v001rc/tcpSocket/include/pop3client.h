
#ifndef _POP3CLIENT_H_
#define _POP3CLIENT_H_
//-----------------------------------------------------------------


#include <QString>
#include "tcpsocket.h"
//-----------------------------------------------------------------


enum EErrorCode
{
    NOERROR,
    INVALIDHOSTNAME,
    UNAVAILABLEHOST,
    INVALIDUSERNAME,
    INVALIDPASSWORD,
    NORESPONSEFROMSERVER,
    UNKNOWNSERVERERROR,
    SENDINGERROR
};
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

class CPop3Client
{
  CTcpSocket    cTcpSocket;
  QString       cHostName;
  QString       cUserName;
  QString       cPassWord;
  EErrorCode    cLastError;
  int           cMaxServerTimeOut;

  void Initialise();
  bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord);

 public:

  CPop3Client();
  CPop3Client(QString iHostName, QString iUserName= "", QString iPassWord= "");

  int GetMailCount();
  int GetMailCount(QString iUserName, QString iPassWord);
  int GetMailCount(QString iHostName, QString iUserName, QString iPassWord);

  int  GetMaxServerTimeOut();
  void SetMaxServerTimeOut(int iMaxServerTimeOut);

  EErrorCode GetLastErrorCode();
};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
