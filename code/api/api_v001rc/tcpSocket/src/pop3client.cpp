
#include "pop3client.h"
//-----------------------------------------------------------------

/**
*  Constructor of CPop3Client-Class\n
*  and Parent-Constructor of CMailClient-Class
*
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*
*  @see CMailClient
*
********************************************************************************/
CPop3Client::CPop3Client(CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolPOP3, iConnectionType)
{
}

/**
*  Constructor of CPop3Client-Class\n
*  and Parent-Constructor of CMailClient-Class
*
*  @param   iHostName Defines the Host to connect to.
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*
*  @see CMailClient
*
********************************************************************************/
CPop3Client::CPop3Client(QString iHostName, CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolPOP3, iConnectionType, iHostName)
{
}

/**
*  Constructor of CPop3Client-Class\n
*  and Parent-Constructor of CMailClient-Class
*
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*
*  @see CMailClient
*
********************************************************************************/
CPop3Client::CPop3Client(QString iHostName, QString iUserName, QString iPassWord, CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolPOP3, iConnectionType, iHostName, iUserName, iPassWord)
{
}

/**
*  This function checks the current number of mails in your mailbox.
*
*  @return  Returns the number of mails in your mailbox, on error -1.
*
********************************************************************************/
int CPop3Client::GetMailCount()
{
  return GetMailCount(cHostName, cUserName, cPassWord);
}

/**
*  This function checks the current number of mails in your mailbox.
*
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns the number of mails in your mailbox, on error -1.
*
********************************************************************************/
int CPop3Client::GetMailCount(QString iUserName, QString iPassWord)
{
 return GetMailCount(cHostName, iUserName, iPassWord);
}

/**
*  This function checks the current number of mails in your mailbox.
*
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns the number of mails in your mailbox, on error -1.
*
********************************************************************************/
int CPop3Client::GetMailCount(QString iHostName, QString iUserName, QString iPassWord)
{
 QString RxBuffer;
 bool    CorrectConversion;
 int     EMailCount;

 if (!CheckParams(iHostName, iHostName, iPassWord))
    return -1;

 if (cTcpSocket != NULL)
    delete cTcpSocket;

 cTcpSocket = new CTcpSocket(cConnectionType);

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return -1;

 if (!cTcpSocket->SendText("STAT\r\n"))
    {
     cLastError= SENDINGERROR;
     return -1;
    }

 RxBuffer= cTcpSocket->ReceiveText(cMaxServerTimeOut);

 cTcpSocket->Disconnect();

 if (RxBuffer.isEmpty())
    {
     if (cTcpSocket->IsConnected())
        cLastError= NORESPONSEFROMSERVER;
     else
        cLastError= LOSTCONNECTION;

     return -1;
    }

 if (!RxBuffer.contains("+OK"))
    {
     cLastError= UNKNOWNSERVERERROR;
     return -1;
    }

 EMailCount= RxBuffer.mid(4, RxBuffer.indexOf(" ", 4) - 4).toInt(&CorrectConversion);

 if (CorrectConversion)
    return EMailCount;

 cLastError= UNKNOWNSERVERERROR;
 return -1;
}

/**
*  This function receives a mail with help of a mail-index.\n
*  This mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*
*  @param   iMailIndex Index of the mail you want to receive.
*  @return  Returns a pointer to the received mail-record on success, on error NULL.
*
********************************************************************************/
CMailClient::REMail *CPop3Client::GetMail(int iMailIndex)
{
 return GetMail(iMailIndex, cHostName, cUserName, cPassWord);
}

/**
*  This function receives a mail with help of a mail-index.\n
*  This mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*
*  @param   iMailIndex Index of the mail you want to receive.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns a pointer to the received mail-record on success, on error NULL.
*
********************************************************************************/
CMailClient::REMail *CPop3Client::GetMail(int iMailIndex, QString iUserName, QString iPassWord)
{
 return GetMail(iMailIndex, cHostName, iUserName, iPassWord);
}

/**
*  This function receives a mail with help of a mail-index.\n
*  This mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*
*  @param   iMailIndex Index of the mail you want to receive.
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns a pointer to the received mail-record on success, on error NULL.
*
********************************************************************************/
CMailClient::REMail *CPop3Client::GetMail(int iMailIndex, QString iHostName, QString iUserName, QString iPassWord)
{
 REMail *Mail = new REMail();
 QList<QString> RxBuffer;

 if (!CheckParams(iHostName, iUserName, iPassWord))
    return NULL;

 if (cTcpSocket != NULL)
    delete cTcpSocket;

 cTcpSocket = new CTcpSocket(cConnectionType);

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return NULL;

 if (iMailIndex <= 0)
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return NULL;
    }

 if (!cTcpSocket->SendText("RETR " + QString::number(iMailIndex) + "\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }

 if (!cTcpSocket->ReceiveLines(RxBuffer, cMaxServerTimeOut))
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }

 cTcpSocket->Disconnect();

 if (!RxBuffer.at(0).contains("+OK"))
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return false;
    }

 ParseMailHeader(RxBuffer.at(1), Mail);

 for (int Index= 2; Index < RxBuffer.size(); Index++)
   {
    Mail->Body.append(RxBuffer.at(Index));
   }

 return Mail;
}

/**
*  This function deletes a mail by it's mail-index.
*  The mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*  <b>Caution: mail-index "-1" deletes the whole mailbox irrevocably!!!</b>
*
*  @param   iMailIndex Index of the mail you want to delete.
*  @return  Returns \c true when deleted successfully, else \c false.
*
********************************************************************************/
bool CPop3Client::DeleteMail(int iMailIndex)
{
 return DeleteMail(iMailIndex, cHostName, cUserName, cPassWord);
}

/**
*  This function deletes a mail by it's mail-index.
*  The mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*  <b>Caution: mail-index "-1" deletes the whole mailbox irrevocably!!!</b>
*
*  @param   iMailIndex Index of the mail you want to receive.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns \c true when deleted successfully, else \c false.
*
********************************************************************************/
bool CPop3Client::DeleteMail(int iMailIndex, QString iUserName, QString iPassWord)
{
 return DeleteMail(iMailIndex, cHostName, iUserName, iPassWord);
}

/**
*  This function deletes a mail by it's mail-index.
*  The mail-index must be even or smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index "1".
*  <b>Caution: mail-index "-1" deletes the whole mailbox irrevocably!!!</b>
*
*  @param   iMailIndex Index of the mail you want to receive.
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns \c true when deleted successfully, else \c false.
*
********************************************************************************/
bool CPop3Client::DeleteMail(int iMailIndex, QString iHostName, QString iUserName, QString iPassWord)
{
 QString RxBuffer;

 if (!CheckParams(iHostName, iHostName, iPassWord))
    return false;

 if (iMailIndex == 0 || iMailIndex < -1)
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return false;
    }

 if (cTcpSocket != NULL)
    delete cTcpSocket;

 cTcpSocket = new CTcpSocket(cConnectionType);

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return false;

 if (!cTcpSocket->SendText("DELE " + QString::number(iMailIndex) + "\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }

 RxBuffer= cTcpSocket->ReceiveText(cMaxServerTimeOut);

 cTcpSocket->Disconnect();

 if (RxBuffer.isEmpty())
    {
     if (cTcpSocket->IsConnected())
        cLastError= NORESPONSEFROMSERVER;
     else
        cLastError= LOSTCONNECTION;

     return false;
    }

 if (!RxBuffer.contains("+OK"))
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return false;
    }

 return true;
}

/**
*  Private function to parse the important information out of the mail-header.
*
*  @param   iMailHeader Contains the complete mail-header.
*  @param   oMail Pointer to the mail record.
*
********************************************************************************/
void CPop3Client::ParseMailHeader(QString iMailHeader, REMail *oMail)
{
 QString TempBuffer= iMailHeader;
 QString Temp;
 int     CharIndex;

 for (int Index= 0; Index < 3; Index++)
 {
  switch(Index)
  {
  case 0:  Temp= "To: ";       break;
  case 1:  Temp= "From: ";     break;
  case 2:  Temp= "Subject: ";  break;
  }

  if ((CharIndex= TempBuffer.indexOf(Temp)) != -1)
  {
   TempBuffer= TempBuffer.remove(0, CharIndex + Temp.length());

   if ((CharIndex= TempBuffer.indexOf("\n")) != -1)
      {
       TempBuffer= TempBuffer.remove(CharIndex, TempBuffer.length());
       switch(Index)
       {
        case 0:  oMail->To= TempBuffer;       break;
        case 1:  oMail->From= TempBuffer;     break;
        case 2:  oMail->Subject= TempBuffer;  break;
       }
      }
  }

  TempBuffer= iMailHeader;
 }
}

/**
*  Implementation of abstract class CMailClient.\n
*  Private function to login at the current host.
*
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns \c true when login was successfully, else \c false.
*
*  @see CMailClient, CSmtpClient
*
********************************************************************************/
bool CPop3Client::ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)
{
 if (cTcpSocket->ConnectTo(iHostName, cServerPort, cMaxServerTimeOut))
    {
     if (!cTcpSocket->ReceiveText().contains("+OK"))
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }

     if (!cTcpSocket->SendText("USER " + iUserName + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (!cTcpSocket->ReceiveText().contains("+OK"))
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }

     if (!cTcpSocket->SendText("PASS " + iPassWord + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (!cTcpSocket->ReceiveText().contains("+OK"))
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }

      return true;
    }
    else
        cLastError= UNAVAILABLEHOST;

 return false;
}



