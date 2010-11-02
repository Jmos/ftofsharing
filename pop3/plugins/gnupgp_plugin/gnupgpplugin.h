#ifndef GNUPGPPLUGIN_H
#define GNUPGPPLUGIN_H
//
#include <QtCore>
#include "gnupgpplugin.h"


#include <../../src/securityinterface.h>
//
class GnuPGPPlugin : public QObject, SecurityInterface
{
    Q_OBJECT
    Q_INTERFACES(SecurityInterface)

private:
	QString lastErr;


public:
     GnuPGPPlugin();
     ~GnuPGPPlugin();

	//actions
	QString encrypt(const QString &text, const QString &key="");
	QString decrypt(const QString &text, const QString &key="");
	QString hash(const QString &text, const QString &method="");
	QString sign(const QString &text, const QString &key="");
	QString signHash(const QString &hash, const QString &key="");
	QString verify(const QString &text, const QString &key="");
	
	//key management
	QString generateKey(QString uId);
	bool removeKey(QString keyId);
	QStringList listKeys();
	
	//Plugin Info
	QString about();
	bool autoHash(){return true;};
	bool ini();
	QString lastError(){return lastErr;};
	QString micalg(){return QString("pgp-sha1");};
	QString name(){return QString("GnuPGP Plugin");};
	
};
#endif
