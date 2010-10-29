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

CPop3Client::CPop3Client(QString iHostName, QString iUserName, QString iPassWord)
{
 cHostName=  iHostName;
 cUserName=  iUserName;
 cPassWord=  iPassWord;

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


 if (iHostName.isEmpty())
    {
     cLastError= INVALIDHOSTNAME;
     return -1;
    }
 if (iUserName.isEmpty())
    {
     cLastError= INVALIDUSERNAME;
     return -1;
    }
 if (iPassWord.isEmpty())
    {
     cLastError= INVALIDPASSWORD;
     return -1;
    }

 if (!ServerLogIn(iHostName, iUserName, iPassWord))
    return -1;

 if (!cTcpSocket.SendText("STAT\n\r"))
    {
     cLastError= SENDINGERROR;
     return -1;
    }


 if (!cTcpSocket.WaitForReceiveText(cMaxServerTimeOut))
    {
     cLastError= NORESPONSEFROMSERVER;
     return -1;
    }

 RxBuffer= cTcpSocket.WaitAndReceiveText(cMaxServerTimeOut);

 if (RxBuffer.isEmpty())
    {
     cLastError= NORESPONSEFROMSERVER;
     return -1;
    }

 if (RxBuffer.indexOf("+OK") == -1)
    {
     cLastError= UNKNOWNSERVERERROR;
     return -1;
    }

 EMailCount= RxBuffer.mid(4, RxBuffer.indexOf(" ", 4) - 4).toInt(&CorrectConversion);

 if (CorrectConversion)
    return EMailCount;
    else
        cLastError= UNKNOWNSERVERERROR;

 return -1;
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

