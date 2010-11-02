#ifndef SECURITYINTERFACE_H
#define SECURITYINTERFACE_H
//
#include <QString>
#include <QStringList>

#ifdef Q_OS_WIN32
 #ifdef BUILD_DLL
  #define DLL_MAKRO __declspec(dllexport)
 #else
  #define DLL_MAKRO __declspec(dllimport)
 #endif
#else
 #define DLL_MAKRO
#endif
//
class SecurityInterface{

public:
	virtual ~SecurityInterface() {};
	
//actions
	virtual QString encrypt(const QString &text, const QString &key="") = 0;
	virtual QString decrypt(const QString &text, const QString &key="") = 0;
	virtual QString sign(const QString &text, const QString &key="") = 0;
	virtual QString hash(const QString &text, const QString & method="") = 0;
	virtual QString verify(const QString &text, const QString &key="") = 0;
	
	//key management
	virtual QString generateKey(QString uId) = 0;
	virtual bool removeKey(QString keyId) = 0;
	virtual QStringList listKeys() = 0;
	
	//Plugin Info
	virtual QString about() = 0;
	virtual bool autoHash() = 0;
	virtual bool ini() = 0;
	virtual QString lastError() = 0;
	virtual QString micalg() = 0;
	virtual QString name() = 0;
};

Q_DECLARE_INTERFACE(SecurityInterface,"NuntiusLeo.MailEngine.SecurityInterface/1.0")

#endif
