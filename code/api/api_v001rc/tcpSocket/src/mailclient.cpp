//=================================================================
// Name:            mailclient.cpp
// Autor:           Saxl Georg
// Version:         1.0
// Datum:           18.11.2010
// Beschreibung:
//=================================================================

#include "mailclient.h"
//-----------------------------------------------------------------


CMailClient::CMailClient(EProtocolType iProtocol)
{
 Initialise(iProtocol);
}
//-----------------------------------------------------------------

CMailClient::CMailClient (EProtocolType iProtocol, QString iHostName)
{
 cHostName= iHostName;

 Initialise (iProtocol);
}
//-----------------------------------------------------------------

CMailClient::CMailClient(EProtocolType iProtocol, QString iHostName, QString iUserName, QString iPassWord)
{
 cHostName= iHostName;
 cUserName= iUserName;
 cPassWord= iPassWord;

 Initialise(iProtocol);
}
//-----------------------------------------------------------------

void CMailClient::Initialise(EProtocolType iProtocol)
{
 cMaxServerTimeOut=     5000;
 cLastError=            NOERROR;
 cProtocol=             iProtocol;

 switch (iProtocol)
 {
  case ProtocolPOP3:  cServerPort= 110;  break;
  case ProtocolSMTP:  cServerPort= 25;   break;
 }
}
//-----------------------------------------------------------------

EEMailErrorCode CMailClient::GetLastErrorCode()
{
 EEMailErrorCode CurrentErrorCode= cLastError;
 cLastError= NOERROR;

 return CurrentErrorCode;
}
//-----------------------------------------------------------------

int CMailClient::GetServerPort()
{
 return cServerPort;
}
//-----------------------------------------------------------------

void CMailClient::SetServerPort(int iServerPort)
{
 cServerPort= iServerPort;
}
//-----------------------------------------------------------------

int CMailClient::GetMaxServerTimeOut()
{
 return cMaxServerTimeOut;
}
//-----------------------------------------------------------------

void CMailClient::SetMaxServerTimeOut(int iMaxServerTimeOut)
{
 cMaxServerTimeOut= iMaxServerTimeOut;
}
//-----------------------------------------------------------------

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
//-----------------------------------------------------------------
