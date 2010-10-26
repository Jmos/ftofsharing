//=================================================================
// Name:	    	udpsocket.h
// Autor: 	  		Saxl Georg
// Version:	  		1.0
// Datum: 	  		14.10.2010
// Beschreibung:
//=================================================================


#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_
//-----------------------------------------------------------------

#include <QUdpSocket>
#include <QObject>
#include <QString>
#include <iostream>
#include <QApplication>
//-----------------------------------------------------------------

/**
*    @ingroup   api
*
*    Mit dieser Klassen kann ein UDP-Socket auf einfache Weise verwaltet und verwendet werden.
*
*    @brief     Verwaltungsklasse für ein UDP-Socket
*    @author    Saxl Georg
*    @version   1.0
*    @date  	\a Begonnnen: 14.10.2010  \n\n
*       	    \a Abgeschlossen:
*
*******************************************************************************/

class CUdpSocket : private QUdpSocket
{

 public:

  QString ReceiveText();

  void SetSenderPort(int iPort);
  int  GetSenderPort();
  bool ListenOnPort(int iPort);
  bool SendText(QString iData, QHostAddress iReceiverAddress, int iReceiverPort);
  bool WaitForReceiveText();
  bool WaitForReceiveText(int iMilliSeconds);

};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
