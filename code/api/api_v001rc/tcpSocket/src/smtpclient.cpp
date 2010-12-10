
#include "smtpclient.h"
//-----------------------------------------------------------------


/**
*  Constructor of CSmtpClient-Class\n
*  and Parent-Constructor of CMailClient-Class*
*
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*
*  @see CMailClient
*
********************************************************************************/
CSmtpClient::CSmtpClient(CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolSMTP, iConnectionType)
{
}

/**
*  Constructor of CSmtpClient-Class\n
*  and Parent-Constructor of CMailClient-Class*
*
*  @param   iHostName Defines the Host to connect to.
*  @param   iConnectionType Sets the connection type (unencrypted/SSL).
*
*  @see CMailClient
*
********************************************************************************/
CSmtpClient::CSmtpClient(QString iHostName, CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolSMTP, iConnectionType, iHostName)
{
}

/**
*  Constructor of CSmtpClient-Class\n
*  and Parent-Constructor of CMailClient-Class*
*
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @param   iConnectionType Sets the connection type (unencrypted/SSL).
*
*  @see CMailClient
*
********************************************************************************/
CSmtpClient::CSmtpClient(QString iHostName, QString iUserName, QString iPassWord, CTcpSocket::EConnectionType iConnectionType): CMailClient(ProtocolSMTP, iConnectionType, iHostName, iUserName, iPassWord)
{
}

/**
*  Function to send an e-mail.
*
*  @param   iMail A Pointer to an instance of \c REMail.
*  @return  Returns \c true when sending was successfully, else \c false.
*
********************************************************************************/
bool CSmtpClient::SendMail(REMail *iMail)
{
 return SendMail(iMail, cHostName, cUserName, cPassWord);
}

/**
*  Function to send an e-mail.
*
*  @param   iMail A Pointer to an instance of \c REMail.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns \c true when sending was successfully, else \c false.
*
********************************************************************************/
bool CSmtpClient::SendMail(REMail *iMail, QString iUserName, QString iPassWord)
{
 return SendMail(iMail, cHostName, iUserName, iPassWord);
}

/**
*  Function to send an e-mail.
*
*  @param   iMail A Pointer to an instance of \c REMail.
*  @param   iHostName Defines the host to connect to.
*  @param   iUserName Defines the username for authentication.
*  @param   iPassWord Defines the password for authentication.
*  @return  Returns \c true when sending was successfully, else \c false.
*
********************************************************************************/
bool CSmtpClient::SendMail(REMail *iMail, QString iHostName, QString iUserName, QString iPassWord)
{
 QList<QString> MailData;

 if (!CheckParams(iHostName, iUserName, iPassWord))
     return false;

 if (iMail != NULL)
    {
     if (iMail->To.isEmpty() || !iMail->To.contains("@"))
        {
         cLastError= INVALIDTOADDRESS;
         return false;
        }
     if (iMail->From.isEmpty() || !iMail->From.contains("@"))
        {
         cLastError= INVALIDFROMADDRESS;
         return false;
        }
    }
    else
       {
        cLastError= INVALIDMAIL;
        return false;
       }

 MailData.append("From: <"   + iMail->From + ">");
 MailData.append("To: <"     + iMail->To   + ">");
 MailData.append("Subject: " + iMail->Subject + "\n");

 MailData.append(iMail->Body);

 MailData.append("\r\n.\r\n");

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
     return false;

 if (!cTcpSocket->SendText("MAIL FROM: <" + iMail->From + ">\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket->ReceiveText().left(3) != "250")
    {
     cLastError= INVALIDFROMADDRESS;
     return false;
    }
 if (!cTcpSocket->SendText("RCPT TO: <" + iMail->To + ">\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket->ReceiveText().left(3) != "250")
    {
     cLastError= INVALIDTOADDRESS;
     return false;
    }
 if (!cTcpSocket->SendText("DATA\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket->ReceiveText().left(3) != "354")
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }
 if (!cTcpSocket->SendText(MailData))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket->ReceiveText().left(3) != "250")
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }

 cTcpSocket->Disconnect();

 return true;
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
*  @see CMailClient, CPop3Client
*
********************************************************************************/
bool CSmtpClient::ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)
{
 QString    PCName= getenv("COMPUTERNAME");
 QByteArray UserName;
 QByteArray PassWord;

 UserName.append(iUserName);
 PassWord.append(iPassWord);

 if (cTcpSocket->ConnectTo(iHostName, cServerPort, cMaxServerTimeOut))
    {
     if (cTcpSocket->ReceiveText().left(3) != "220")
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }
     if (!cTcpSocket->SendText("EHLO " + PCName + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket->ReceiveText().left(3) != "250")
        {
         if (!cTcpSocket->SendText("HELO " + PCName + "\r\n"))
            {
             cLastError= SENDINGERROR;
             return false;
            }
         if (cTcpSocket->ReceiveText().left(3) != "250")
            {
             cLastError= UNKNOWNSERVERERROR;
             return false;
            }
        }
     if (!cTcpSocket->SendText("AUTH LOGIN\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket->ReceiveText().left(3) != "334")
        {
         cLastError= AUTHENTICATIONERROR;
         return false;
        }
     if (!cTcpSocket->SendText((QString)UserName.toBase64() + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket->ReceiveText().left(3) != "334")
        {
         cLastError= AUTHENTICATIONERROR;
         return false;
        }
     if (!cTcpSocket->SendText((QString)PassWord.toBase64() + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }

     QString s= cTcpSocket->ReceiveText();
     if (s.left(3) != "235")
        {
         printf(qPrintable("Auth3:  " + s));
         cLastError= AUTHENTICATIONERROR;
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

