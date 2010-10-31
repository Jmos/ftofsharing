//=================================================================
// Name:	    	pop3client.cpp
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		28.10.2010
// Beschreibung:
//=================================================================


#include "pop3client.h"
//-----------------------------------------------------------------

CPop3Client::CPop3Client()
{
 Initialise();
}
//-----------------------------------------------------------------

CPop3Client::CPop3Client(QString iHostName)
{
 cHostName= iHostName;

 Initialise();
}
//-----------------------------------------------------------------

CPop3Client::CPop3Client(QString iHostName, QString iUserName, QString iPassWord)
{
 cHostName= iHostName;
 cUserName= iUserName;
 cPassWord= iPassWord;

 Initialise();
}
//-----------------------------------------------------------------

void CPop3Client::Initialise()
{
 cLastError=            NOERROR;
 cMaxServerTimeOut=     5000;
}
//-----------------------------------------------------------------

EErrorCode CPop3Client::GetLastErrorCode()
{
 EErrorCode CurrentErrorCode= cLastError;
 cLastError= NOERROR;

 return CurrentErrorCode;
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount()
{
 return GetMailCount(cHostName, cUserName, cPassWord, true);
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount(QString iUserName, QString iPassWord)
{
 return GetMailCount(cHostName, iUserName, iPassWord, true);
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount(QString iHostName, QString iUserName, QString iPassWord)
{
 return GetMailCount(iHostName, iUserName, iPassWord, true);
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount(QString iHostName, QString iUserName, QString iPassWord, bool iDisconnect)
{
 QString RxBuffer;
 bool    CorrectConversion;
 int     EMailCount;

 if (!CheckParams(iHostName, iHostName, iPassWord))
    return -1;

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return -1;

 if (!cTcpSocket.SendText("STAT\n\r"))
    {
     cLastError= SENDINGERROR;
     return -1;
    }

 RxBuffer= cTcpSocket.WaitAndReceiveText(cMaxServerTimeOut);

 if (iDisconnect)
    cTcpSocket.Disconnect();

 if (RxBuffer.isEmpty())
    {
     cLastError= NORESPONSEFROMSERVER;
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
//-----------------------------------------------------------------

REMail *CPop3Client::GetMail(int iMailIndex)
{
 return GetMail(iMailIndex, cHostName, cUserName, cPassWord);
}
//-----------------------------------------------------------------

REMail *CPop3Client::GetMail(int iMailIndex, QString iUserName, QString iPassWord)
{
 return GetMail(iMailIndex, cHostName, iUserName, iPassWord);
}
//-----------------------------------------------------------------

REMail *CPop3Client::GetMail(int iMailIndex, QString iHostName, QString iUserName, QString iPassWord)
{
 REMail *Mail = new REMail();
 QList<QString> RxBuffer;
 //QString RxBuffer;
 int MailCount;

 if (!CheckParams(iHostName, iUserName, iPassWord))
    return NULL;

 MailCount= GetMailCount(iHostName, iUserName, iPassWord, true);

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return NULL;

 if (iMailIndex <= 0)
    {
     cLastError= MAILINDEXOUTOUBOUNDS;
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
     cLastError= MAILINDEXOUTOUBOUNDS;
     return NULL;
    }

 if (!cTcpSocket.SendText("RETR " + QString::number(iMailIndex) + "\n\r"))
    {
     cLastError= SENDINGERROR;
     return false;
    }
 if (!cTcpSocket.WaitForReceiveText(cMaxServerTimeOut))
    {
     cLastError= NORESPONSEFROMSERVER;
     return false;
    }

 cTcpSocket.ReceiveLines(RxBuffer);

 //cTcpSocket.ReceiveText();

  //std::cout << RxBuffer.size();
  //std::cout << qPrintable(RxBuffer);

 return Mail;
}
//-----------------------------------------------------------------

int CPop3Client::GetMaxServerTimeOut()
{
 return cMaxServerTimeOut;
}
//-----------------------------------------------------------------

void CPop3Client::SetMaxServerTimeOut(int iMaxServerTimeOut)
{
 cMaxServerTimeOut= iMaxServerTimeOut;
}
//-----------------------------------------------------------------

bool CPop3Client::ServerLogIn(QString iHostName, QString iUserName, QString iPassWord)
{
 if (cTcpSocket.ConnectTo(iHostName, 110, cMaxServerTimeOut))
    {
     if (cTcpSocket.WaitForReceiveText(cMaxServerTimeOut))
        {
         if (!cTcpSocket.ReceiveText().contains("+OK"))
            {
             cLastError= UNKNOWNSERVERERROR;
             return false;
            }
         if (!cTcpSocket.SendText("USER " + iUserName + "\n\r"))
            {
             cLastError= SENDINGERROR;
             return false;
            }
         if (!cTcpSocket.WaitForReceiveText(cMaxServerTimeOut))
            {
             cLastError= NORESPONSEFROMSERVER;
             return false;
            }
         if (!cTcpSocket.ReceiveText().contains("+OK"))
            {
             cLastError= UNKNOWNSERVERERROR;
             return false;
            }
         if (!cTcpSocket.SendText("PASS " + iPassWord + "\n\r"))
            {
             cLastError= SENDINGERROR;
             return false;
            }
         if (!cTcpSocket.WaitForReceiveText(cMaxServerTimeOut))
            {
             cLastError= NORESPONSEFROMSERVER;
             return false;
            }
         if (!cTcpSocket.ReceiveText().contains("+OK"))
            {
             cLastError= UNKNOWNSERVERERROR;
             return false;
            }

         return true;
        }
        else
        {
         cLastError= NORESPONSEFROMSERVER;
        }
    }
    else
    {
     cLastError= UNAVAILABLEHOST;
    }

 return false;
}
//-----------------------------------------------------------------

bool CPop3Client::CheckParams(QString iHostName, QString iUserName, QString iPassWord)
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


