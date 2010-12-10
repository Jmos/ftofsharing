
#include "mailclient.h"
//-----------------------------------------------------------------

/**
*  Constructor of CMailClient-Class
*
*  @param   iProtocol Sets the protocol type (POP3/SMTP)
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*
********************************************************************************/
CMailClient::CMailClient(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType)
{
 Initialise(iProtocol, iConnectionType);
}

/**
*  Constructor of CMailClient-Class
*
*  @param   iProtocol Sets the protocol type (POP3/SMTP)
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*  @param   iHostName Defines the Host to connect to
*
********************************************************************************/
CMailClient::CMailClient (EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType, QString iHostName)
{
 cHostName= iHostName;

 Initialise (iProtocol, iConnectionType);
}

/**
*  Constructor of CMailClient-Class
*
*  @param   iProtocol Sets the protocol type (POP3/SMTP)
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*  @param   iHostName Defines the host to connect to
*  @param   iUserName Defines the username for authentication
*  @param   iPassWord Defines the password for authentication
*
********************************************************************************/
CMailClient::CMailClient(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType, QString iHostName, QString iUserName, QString iPassWord)
{
 cHostName= iHostName;
 cUserName= iUserName;
 cPassWord= iPassWord;

 Initialise(iProtocol, iConnectionType);
}

/**
*  This function should be called by the constructor only.
*
********************************************************************************/
void CMailClient::Initialise(EProtocolType iProtocol, CTcpSocket::EConnectionType iConnectionType)
{
 cMaxServerTimeOut=     5000;
 cLastError=            NOERROR;
 cProtocol=             iProtocol;

 cTcpSocket = new CTcpSocket(iConnectionType);

 switch (iProtocol)
 {
  case ProtocolPOP3:  cServerPort= 110;  break;
  case ProtocolSMTP:  cServerPort= 25;   break;
 }
}

/**
*  Destructor of CMailClient-Class
*
********************************************************************************/
CMailClient::~CMailClient()
{
 delete cTcpSocket;
}

/**
*  Returns the most recent error code
*
*  @return  Returns an \c EEMailErrorCode of the last error
*
********************************************************************************/
CMailClient::EEMailErrorCode CMailClient::GetLastErrorCode()
{
 EEMailErrorCode CurrentErrorCode= cLastError;
 cLastError= NOERROR;

 return CurrentErrorCode;
}

/**
*  Function to get the current server port.
*
*  @return  Returns the current server port.
*
********************************************************************************/
int CMailClient::GetServerPort()
{
 return cServerPort;
}

/**
*  Function to set the server port.
*
*  @param   iServerPort Port of the server
*
********************************************************************************/
void CMailClient::SetServerPort(int iServerPort)
{
 cServerPort= iServerPort;
}

/**
*  Function to get the current server timeout.
*
*  @return  Returns the server timeout in ms.
*
********************************************************************************/
int CMailClient::GetMaxServerTimeOut()
{
 return cMaxServerTimeOut;
}

/**
*  Function to set the server timeout.
*
*  @param   iMaxServerTimeOut timeout in ms.
*
********************************************************************************/
void CMailClient::SetMaxServerTimeOut(int iMaxServerTimeOut)
{
 cMaxServerTimeOut= iMaxServerTimeOut;
}

/**
*  Function to get the current connection type.
*
*  @return  Returns a \c EConnectionType.
*
********************************************************************************/
CTcpSocket::EConnectionType CMailClient::GetConnectionType()
{
 return cTcpSocket->GetConnectionType();
}

/**
*  Function to set the connection type.\n
*  Shouldn't be called while open connection.
*
*  @param   iConnectionType Defines the new connection type.
*
********************************************************************************/
void CMailClient::SetConnectionType(CTcpSocket::EConnectionType iConnectionType)
{
 if (iConnectionType != cTcpSocket->GetConnectionType())
    {
     delete cTcpSocket;
     cTcpSocket= new CTcpSocket(iConnectionType);
    }
}

/**
*  Function to check the commited parameters.
*
*  @param   iHostName Defines the host to connect to
*  @param   iUserName Defines the username for authentication
*  @param   iPassWord Defines the password for authentication
*  @return  Returns \c true when all parameters are ok, else \c false.
*
********************************************************************************/
bool CMailClient::CheckParams(QString iHostName, QString iUserName, QString iPassWord)
{
 if (iHostName.isEmpty())
 {
  cLastError= INVALIDHOSTNAME;
  return false;
 }
if (iUserName.isEmpty())
 {
  cLastError= INVALIDUSERNAME;
  return false;
 }
if (iPassWord.isEmpty())
 {
  cLastError= INVALIDPASSWORD;
  return false;
 }

 return true;
}
