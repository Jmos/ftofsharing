
#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
//-----------------------------------------------------------------

#include <QTcpSocket>
#include <QString>
#include <QObject>
#include <QHostAddress>
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

  QTcpSocket cTcpSocket;

  bool cConnected;

 public:

  CTcpSocket();

  bool ConnectTo(QHostAddress iHostAddress, int iPort, int iTimeoutMS= 30000);
  bool ConnectTo(QString iHostName, int iPort, int iTimeoutMS= 30000);
  void Disconnect();
  bool IsConnected();
  bool SendText(QString iData);
  bool WaitForReceiveText();
  bool WaitForReceiveText(int iTimeOutMs);
  QString ReceiveText();
  QString WaitAndReceiveText();
  QString WaitAndReceiveText(int iTimeOutMs);


 private slots:

  void Connected();
  void Disconnected();

};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
