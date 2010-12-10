
#include "tcpsocket.h"
//-----------------------------------------------------------------

/**
*  Constructor of CTcpSocket-Class
*
*  @param   iConnectionType Sets the connection type (unencrypted/SSL)
*  @param   iParent Is passed on to the QObject constructor.
*
********************************************************************************/
CTcpSocket::CTcpSocket(EConnectionType iConnectionType, QObject *iParent): QObject(iParent)
{
 cConnected= false;

 cConnectionType= iConnectionType;

 cTcpSocket.setSocketOption(QAbstractSocket::KeepAliveOption, false);

 connect(&cTcpSocket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
 connect(&cTcpSocket, SIGNAL(connected()), this, SLOT(Connected()));

 connect(&cSslSocket, SIGNAL(encrypted()), this, SLOT(EncryptedMode()));
}

/**
*  Signal: on unencrypted-mode set when connection established
*
********************************************************************************/
void CTcpSocket::Connected()
{
 if (cConnectionType == NONCRYPTEDMODE)
    cConnected= true;
}

/**
*  Signal: on encrypted-mode set when connection established
*
********************************************************************************/
void CTcpSocket::EncryptedMode()
{
 if (cConnectionType == SSLMODE)
    cConnected= true;
}

/**
*  Signal: called when connection is disconnected.
*
********************************************************************************/
void CTcpSocket::Disconnected()
{
 cConnected= false;
}

/**
*  Starts a connection to a host
*
*  @param   iHostAddress host to connect to
*  @param   iPort port where host is listening
*  @param   iTimeoutMS time in ms waiting for hostresponse
*  @return  Returns \c true when connection established
*
********************************************************************************/
bool CTcpSocket::ConnectTo(QHostAddress iHostAddress, int iPort, int iTimeoutMS)
{
 if (cConnected || cConnectionType == SSLMODE)
    return false;

 cTcpSocket.connectToHost(iHostAddress, (quint16)iPort);

 return cTcpSocket.waitForConnected();
}

/**
*  Starts a connection to a host
*
*  @param   iHostName host to connect to
*  @param   iPort port where host is listening
*  @param   iTimeoutMS time in ms waiting for hostresponse
*  @return  Returns \c true when connection established
*
********************************************************************************/
bool CTcpSocket::ConnectTo(QString iHostName, int iPort, int iTimeoutMS)
{
 if (cConnected)
    return false;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     cTcpSocket.connectToHost(iHostName, (quint16)iPort);
     return cTcpSocket.waitForConnected(iTimeoutMS);
    }
    else
       {
        cSslSocket.connectToHostEncrypted(iHostName, (quint16)iPort);
        return cSslSocket.waitForEncrypted(iTimeoutMS);
       }
}

/**
*  Disconnects a connection to a host
*
********************************************************************************/
void CTcpSocket::Disconnect()
{
 if (!cConnected)
    return;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     cTcpSocket.disconnectFromHost();
    }
    else
       {
        cSslSocket.disconnectFromHost();
       }
}

/**
*  Checks if the connection is alive
*
*  @return  Returns \c true when connection is alive, else \c false
*
********************************************************************************/
bool CTcpSocket::IsConnected()
{
 return cConnected;
}

/**
*  Sends data to the host.
*
*  @param   iData The data to send to the host.
*  @return  Returns \c true when sending was successful
*
********************************************************************************/
bool CTcpSocket::SendText(QString iData)
{
 if (!cConnected)
    return false;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     if (cTcpSocket.write(qPrintable(iData), iData.size()) != -1)
        {
         cTcpSocket.flush();
         return true;
        }
    }
    else
       {
        if (cSslSocket.write(qPrintable(iData), iData.size()) != -1)
           {
            cSslSocket.flush();
            return true;
           }
       }

 return false;
}

/**
*  Sends data to the host.
*
*  @param   iData The data to send to the host.
*  @return  Returns \c true when sending was successfully, else \c false
*
********************************************************************************/
bool CTcpSocket::SendText(QList<QString> iData)
{
 if (!cConnected)
    return false;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     for (int Index= 0; Index < iData.size(); Index++)
         {
          if (cTcpSocket.write(qPrintable(iData.at(Index) + "\n"), iData.at(Index).size() + 1) == -1)
             return false;
         }

     if (cTcpSocket.flush())
        return true;
    }
    else
       {
        for (int Index= 0; Index < iData.size(); Index++)
            {
             if (cSslSocket.write(qPrintable(iData.at(Index) + "\n"), iData.at(Index).size() + 1) == -1)
                return false;
            }

        if (cSslSocket.flush())
           return true;
       }

 return false;
}

/**
*  Receives data from the host.
*
*  @param   iTimeOutMs Time in ms to wait for data form the host.
*  @return  Returns \c true when data was received successfully, else \c false.
*
********************************************************************************/
QString CTcpSocket::ReceiveText(int iTimeOutMs)
{
 if (!cConnected)
    return "";

 QString RxData;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     if (cTcpSocket.waitForReadyRead(iTimeOutMs))
        {
         int CharIndex;

         RxData= cTcpSocket.readAll();

         CharIndex= RxData.indexOf("\n");

         if (CharIndex != -1)
            RxData= RxData.left(CharIndex);
        }
    }
    else
       {
        if (cSslSocket.waitForReadyRead(iTimeOutMs))
           {
            int CharIndex;

            RxData= cSslSocket.readAll();

            CharIndex= RxData.indexOf("\n");

            if (CharIndex != -1)
               RxData= RxData.left(CharIndex);
           }
       }

 return RxData;
}

/**
*  Receives a list of data until the line: [CR][LF].[CR][LF]
*
*  @param   oStringList Received list of data.
*  @param   iTimeOutMs Time in ms to wait for data form the host.
*  @return  Returns \c true when data was received successfully, else \c false.
*
********************************************************************************/
bool CTcpSocket::ReceiveLines(QList<QString> &oStringList, int iTimeOutMs)
{
 if (!cConnected)
    return false;

 QString RxData;
 int     CharIndex;
 bool    RetrunValue= false;

 if (cConnectionType == NONCRYPTEDMODE)
    {
     while (cTcpSocket.waitForReadyRead(iTimeOutMs))
     {
      RxData= cTcpSocket.readAll();

      RetrunValue= true;

      CharIndex= RxData.indexOf("\n.\r\n");

      if (CharIndex != -1)
         RxData= RxData.left(CharIndex);

      oStringList.append(RxData);

      if (CharIndex != -1)
         break;
     }
    }
    else
       {
        while (cSslSocket.waitForReadyRead(iTimeOutMs))
        {
         RxData= cSslSocket.readAll();

         RetrunValue= true;

         CharIndex= RxData.indexOf("\n.\r\n");

         if (CharIndex != -1)
            RxData= RxData.left(CharIndex);

         oStringList.append(RxData);

         if (CharIndex != -1)
            break;
        }
       }

 return RetrunValue;
}

/**
*  Checks the connection type.
*
*  @return  Returns the connection type of the current connection.
*
********************************************************************************/
CTcpSocket::EConnectionType CTcpSocket::GetConnectionType()
{
 return cConnectionType;
}



