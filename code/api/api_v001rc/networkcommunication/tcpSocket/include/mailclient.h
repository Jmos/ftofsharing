
#ifndef _MAILCLIENT_H_
#define _MAILCLIENT_H_
//-----------------------------------------------------------------

#include <QString>
#include <QList>
#include "tcpsocket.h"
//-----------------------------------------------------------------

/**
*    @ingroup   api
*
*    @brief     Abstract class for a basic mail-client.
*    @author    Saxl Georg
*    @version   1.0
*    @date      \a started:    18.11.2010  \n
*               \a completed:  03.12.2010
*
*    This abstract class comprises functions and properties for a basic mail-client.\n
*    Inherited by CSmtpClient, CPop3Client.\n
*
*    @par Version 1.1 (05.12.2010)
*         \arg SSL supported
*
*******************************************************************************/
class CMailClient
{
 public:

  /**
  * This struct declares the data structure of an email.
  */
  struct REMail
  {
   QString From;           ///< sender-address
   QString To;             ///< receiver-address
   QString Subject;        ///< subject of the mail
   QList<QString> Body;    ///< body of the mail
  };

  /**
  * This enum defines errors while sending/receiving emails.
  */
  enum EEMailErrorCode
  {
      NOERROR,                ///< No Error occurred.
      INVALIDHOSTNAME,        ///< The commited hostname was invalid.
      UNAVAILABLEHOST,        ///< The commited host is unavailable.
      INVALIDUSERNAME,        ///< The commited username was invalid.
      INVALIDPASSWORD,        ///< The commited password was invalid.
      NORESPONSEFROMSERVER,   ///< The host doesnot respond.
      UNKNOWNSERVERERROR,     ///< A unknown error occurred on the server.
      SENDINGERROR,           ///< An error occurred while sending.
      MAILINDEXOUTOFBOUNDS,   ///< The commited Mailindex is out of bounds.
      LOSTCONNECTION,         ///< An unexpected disconnection occurred.
      AUTHENTICATIONERROR,    ///< Authentication failed.
      INVALIDTOADDRESS,       ///< The receiver-address is invalid.
      INVALIDFROMADDRESS,     ///< The sender-address is invalid.
      INVALIDMAIL             ///< The commited mail-struct is invalid.
  };

  /**
  * This enum defines the protocol type
  */
  enum EProtocolType
  {
      ProtocolPOP3,     ///< to use the protocol POP3
      ProtocolSMTP      ///< to use the protocol SMTP
  };


  CMailClient(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType);
  CMailClient(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType, QString iHostName);
  CMailClient(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType, QString iHostName, QString iUserName, QString iPassWord);

  ~CMailClient();

  int  GetServerPort();
  void SetServerPort(int iServerPort);

  int  GetMaxServerTimeOut();
  void SetMaxServerTimeOut(int iMaxServerTimeOut);

  CTcpSocket::EConnectionType GetConnectionType();
  void SetConnectionType(CTcpSocket::EConnectionType iConnectionType);

  EEMailErrorCode GetLastErrorCode();

 protected:

  CTcpSocket     *cTcpSocket;
  QString         cHostName;
  QString         cUserName;
  QString         cPassWord;
  EEMailErrorCode cLastError;
  EProtocolType   cProtocol;
  int             cMaxServerTimeOut;
  int             cServerPort;
  CTcpSocket::EConnectionType cConnectionType;

  void Initialise(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType);
  bool CheckParams(QString iHostName, QString iUserName, QString iPassWord);

  virtual bool ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)= 0;   ///< abstract function for login
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
