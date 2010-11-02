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
 return GetMailCount(cHostName, cUserName, cPassWord);
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount(QString iUserName, QString iPassWord)
{
 return GetMailCount(cHostName, iUserName, iPassWord);
}
//-----------------------------------------------------------------

int CPop3Client::GetMailCount(QString iHostName, QString iUserName, QString iPassWord)
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

 RxBuffer= cTcpSocket.ReceiveText(cMaxServerTimeOut);

 cTcpSocket.Disconnect();

 if (RxBuffer.isEmpty())
    {
     if (cTcpSocket.IsConnected())
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
 int            MailCount;

 if (!CheckParams(iHostName, iUserName, iPassWord))
    return NULL;

 MailCount= GetMailCount(iHostName, iUserName, iPassWord);

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

 if (!cTcpSocket.ReceiveLines(RxBuffer, cMaxServerTimeOut))
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
     if (!cTcpSocket.ReceiveText().contains("+OK"))
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

 /*//Debug
 std::cout << "\n\nTo: " << qPrintable(oMail->To);
 std::cout << "\nFrom: " << qPrintable(oMail->From);
 std::cout << "\nSubject: " << qPrintable(oMail->Subject);
 */
}
//-----------------------------------------------------------------



