
#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
//-----------------------------------------------------------------

#include <QTcpSocket>
#include <QSslSocket>
#include <QString>
#include <QObject>
#include <QHostAddress>
#include <iostream>

//-----------------------------------------------------------------

/**
*    @ingroup   api
*
*    Mit dieser Klassen kann ein TCP-Socket auf einfache Weise verwaltet und verwendet werden.
*
*    @brief     Verwaltungsklasse für ein TCP-Socket
*    @author    Saxl Georg
*    @version   1.0
*    @date  	\a Begonnnen: 27.10.2010  \n\n
*       	    \a Abgeschlossen:
*
*******************************************************************************/

class CTcpSocket : public QObject
{
 Q_OBJECT

  QTcpSocket   cTcpSocket;
  QSslSocket   cSslSocket;
  bool         cConnected;
  QTextStream *cSocketStream;

 public:

  CTcpSocket();

  bool    ConnectTo(QHostAddress iHostAddress, int iPort, int iTimeoutMS= 10000);
  bool    ConnectTo(QString iHostName, int iPort, int iTimeoutMS= 10000);
  void    Disconnect();
  bool    IsConnected();
  bool    SendText(QString iData);
  bool    SendText(QList<QString> iData);
  bool    ReceiveLines(QList<QString> &oStringList, int iTimeOutMs = 5000);
  QString ReceiveText(int iTimeOutMs = 5000);

 private slots:

  void Connected();
  void Disconnected();
};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
