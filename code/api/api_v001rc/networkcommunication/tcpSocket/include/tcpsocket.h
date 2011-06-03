
#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
//-----------------------------------------------------------------

#include <QTcpSocket>
#include <QSslSocket>
#include <QString>
#include <QObject>
#include <QHostAddress>
//-----------------------------------------------------------------

/**
*    @ingroup   api
*
*    @brief     Handling of a TCP-Socket
*    @author    Saxl Georg
*    @version   1.0
*    @date  	\a started:   27.10.2010  \n
*       	      \a completed: 29.10.2010
*
*    This class can easily handle a TCP-connection.\n
*
*   @par Version 1.1 (05.12.2010)
*     \arg SSL supported
*
*******************************************************************************/
class CTcpSocket : public QObject
{
 Q_OBJECT

 public:

  /**
  * Dieses enum definiert den Verbindungstypen des Sockets.
  */
  enum EConnectionType
  {
     NONCRYPTEDMODE,    ///< unverschlüsselte Tcp-Verbindung
     SSLMODE            ///< SSL-verschlüsselte Tcp-Verbindung
  };

  CTcpSocket(EConnectionType iConnectionType= NONCRYPTEDMODE, QObject *iParent= 0);

  bool    ConnectTo(QHostAddress iHostAddress, int iPort, int iTimeoutMS= 10000);
  bool    ConnectTo(QString iHostName, int iPort, int iTimeoutMS= 10000);
  void    Disconnect();
  bool    IsConnected();
  bool    SendText(QString iData);
  bool    SendText(QList<QString> iData);
  bool    ReceiveLines(QList<QString> &oStringList, int iTimeOutMs = 5000);
  QString ReceiveText(int iTimeOutMs = 5000);
  bool    ReceiveText(QString &oRxData, int iTimeOutMs = 5000);

  EConnectionType GetConnectionType();

 private:

  EConnectionType cConnectionType;
  QTcpSocket      cTcpSocket;
  QSslSocket      cSslSocket;
  bool            cConnected;

 private slots:

  void Connected();
  void EncryptedMode();
  void Disconnected();
};
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#endif
