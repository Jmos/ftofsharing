//=================================================================
// Name:          smtpclient.cpp
// Autor:         Saxl Georg
// Version:       1.0
// Datum:         19.11.2010
// Beschreibung:
//=================================================================


#include "smtpclient.h"
//-----------------------------------------------------------------

CSmtpClient::CSmtpClient(): CMailClient(ProtocolSMTP)
{
}
//-----------------------------------------------------------------

CSmtpClient::CSmtpClient(QString iHostName): CMailClient(ProtocolSMTP, iHostName)
{
}
//-- ---------------------------------------------------------------

CSmtpClient::CSmtpClient(QString iHostName, QString iUserName, QString iPassWord): CMailClient(ProtocolSMTP, iHostName, iUserName, iPassWord)
{
}
//-----------------------------------------------------------------

bool CSmtpClient::SendMail(REMail *iMail)
{
 return SendMail(iMail, cHostName, cUserName, cPassWord);
}
//-----------------------------------------------------------------

bool CSmtpClient::SendMail(REMail *iMail, QString iUserName, QString iPassWord)
{
 return SendMail(iMail, cHostName, iUserName, iPassWord);
}
//-----------------------------------------------------------------

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
 MailData.append("Subject: " + iMail->Subject);

 MailData.append(iMail->Body);

 MailData.append("\r\n.\r\n");

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
     return false;

 if (!cTcpSocket.SendText("MAIL FROM: <" + iMail->From + ">\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket.ReceiveText().left(3) != "250")
    {
     cLastError= INVALIDFROMADDRESS;
     return false;
    }
 if (!cTcpSocket.SendText("RCPT TO: <" + iMail->To + ">\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket.ReceiveText().left(3) != "250")
    {
     cLastError= INVALIDTOADDRESS;
     return false;
    }
 if (!cTcpSocket.SendText("DATA\r\n"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket.ReceiveText().left(3) != "354")
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }
 if (!cTcpSocket.SendText(MailData))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (cTcpSocket.ReceiveText().left(3) != "250")
    {
     cLastError= UNKNOWNSERVERERROR;
     return false;
    }

 cTcpSocket.Disconnect();

 return true;
}
//-----------------------------------------------------------------

bool CSmtpClient::ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)
{
 QString    PCName= getenv("COMPUTERNAME");
 QByteArray UserName;
 QByteArray PassWord;

 UserName.append(iUserName);
 PassWord.append(iPassWord);

 printf(qPrintable("Username: " + (QString)UserName.toBase64() + "\nPassword: " + (QString)PassWord.toBase64() + "\n\n"));

 if (cTcpSocket.ConnectTo(iHostName, cServerPort, cMaxServerTimeOut))
    {
     if (cTcpSocket.ReceiveText().left(3) != "220")
        {
         cLastError= UNKNOWNSERVERERROR;
         return false;
        }
     if (!cTcpSocket.SendText("EHLO " + PCName + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket.ReceiveText().left(3) != "250")
        {
         if (!cTcpSocket.SendText("HELO " + PCName + "\r\n"))
            {
             cLastError= SENDINGERROR;
             return false;
            }
         if (cTcpSocket.ReceiveText().left(3) != "250")
            {
             cLastError= UNKNOWNSERVERERROR;
             return false;
            }
        }
     if (!cTcpSocket.SendText("AUTH LOGIN\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket.ReceiveText().left(3) != "334")
        {
         cLastError= AUTHENTICATIONERROR;
         return false;
        }
     if (!cTcpSocket.SendText((QString)UserName.toBase64() + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }
     if (cTcpSocket.ReceiveText().left(3) != "334")
        {
         cLastError= AUTHENTICATIONERROR;
         return false;
        }
     if (!cTcpSocket.SendText((QString)PassWord.toBase64() + "\r\n"))
        {
         cLastError= SENDINGERROR;
         return false;
        }

     QString s= cTcpSocket.ReceiveText();
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
//-----------------------------------------------------------------
