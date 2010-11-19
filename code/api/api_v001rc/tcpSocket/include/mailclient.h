
#ifndef _MAILCLIENT_H_
#define _MAILCLIENT_H_
//-----------------------------------------------------------------

#include <QString>
#include <QList>
#include "tcpsocket.h"

//-----------------------------------------------------------------

struct REMail
{
 QString From;
 QString To;
 QString Subject;
 QList<QString> Body;
};
//-----------------------------------------------------------------

enum EEMailErrorCode
{
 AUTHENTICATIONERROR1,
 AUTHENTICATIONERROR2,
 AUTHENTICATIONERROR3,
    NOERROR,
    INVALIDHOSTNAME,
    UNAVAILABLEHOST,
    INVALIDUSERNAME,
    INVALIDPASSWORD,
    NORESPONSEFROMSERVER,
    UNKNOWNSERVERERROR,
    SENDINGERROR,
    EMPTYMAILBOX,
    MAILINDEXOUTOFBOUNDS,
    LOSTCONNECTION,
    AUTHENTICATIONERROR,
    INVALIDTOADDRESS,
    INVALIDFROMADDRESS,
    INVALIDMAIL
};
//-----------------------------------------------------------------

enum EProtocolType
{
    ProtocolPOP3,
    ProtocolSMTP
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
*    @date      \a Begonnnen: 18.11.2010  \n\n
*               \a Abgeschlossen:
*
*******************************************************************************/

class CMailClient
{
  friend class CPop3Client;
  friend class CSmtpClient;

  CTcpSocket      cTcpSocket;
  QString         cHostName;
  QString         cUserName;
  QString         cPassWord;
  EEMailErrorCode cLastError;
  EProtocolType   cProtocol;
  int             cMaxServerTimeOut;
  int             cServerPort;

  void Initialise(EProtocolType iProtocol);
  bool CheckParams(QString iHostName, QString iUserName, QString iPassWord);

 public:

  virtual bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)= 0;

  CMailClient(EProtocolType iProtocol);
  CMailClient(EProtocolType iProtocol, QString iHostName);
  CMailClient(EProtocolType iProtocol, QString iHostName, QString iUserName, QString iPassWord);

  int  GetServerPort();
  void SetServerPort(int iServerPort);

  int  GetMaxServerTimeOut();
  void SetMaxServerTimeOut(int iMaxServerTimeOut);

  EEMailErrorCode GetLastErrorCode();
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
