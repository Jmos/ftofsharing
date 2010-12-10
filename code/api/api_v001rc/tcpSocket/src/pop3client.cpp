
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

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return -1;

 if (!cTcpSocket->SendText("STAT\n\r"))
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
*  This mail-index must be smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index 0.
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
*  This mail-index must be smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index 0.
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
*  This mail-index must be smaller than the current number of mails\n
*  in your mailbox. First mail you get with mail-index 0.
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
 int            MailCount;

 if (!CheckParams(iHostName, iUserName, iPassWord))
    return NULL;

 MailCount= GetMailCount(iHostName, iUserName, iPassWord);

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return NULL;

 if (iMailIndex <= 0)
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return NULL;
    }
 if (MailCount == -1)
    return NULL;
 if (MailCount == 0)
    {
     cLastError= EMPTYMAILBOX;
     return NULL;
    }
 if (MailCount < iMailIndex)
    {
     cLastError= MAILINDEXOUTOFBOUNDS;
     return NULL;
    }

 if (!cTcpSocket->SendText("RETR " + QString::number(iMailIndex) + "\n\r"))
    {
     cLastError= SENDINGERROR;
     return false;
    }

 if (!cTcpSocket->ReceiveLines(RxBuffer, cMaxServerTimeOut))
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }

 if (!RxBuffer.at(0).contains("+OK"))
    {
     cLastError= UNKNOWNSERVERERROR;
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
     if (!cTcpSocket->SendText("USER " + iUserName + "\n\r"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (!cTcpSocket->ReceiveText().contains("+OK"))
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }
     if (!cTcpSocket->SendText("PASS " + iPassWord + "\n\r"))
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
    {
     cLastError= UNAVAILABLEHOST;
    }

 return false;
}



