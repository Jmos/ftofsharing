#include "gnupgpplugin.h"
//
GnuPGPPlugin::GnuPGPPlugin() : QObject(), SecurityInterface()
{
	// TODO
}

GnuPGPPlugin::~GnuPGPPlugin()
{
	// TODO
}

//actions
QString GnuPGPPlugin::encrypt(const QString &text, const QString &key)
{
	return QString("");
}

QString GnuPGPPlugin::decrypt(const QString &text, const QString &key)
{
	return QString("");
}

QString GnuPGPPlugin::hash(const QString &text, const QString &method)
{
	return QString("");
}

bool GnuPGPPlugin::ini()
{
	
	
	
	if(lastErr=="") return true;
	else return false;
}

QString GnuPGPPlugin::sign(const QString &text, const QString &key)
{
	return QString("");
}

QString GnuPGPPlugin::signHash(const QString &hash, const QString &key)
{
	return QString("");
}

QString GnuPGPPlugin::verify(const QString &text, const QString &key)
{
	return QString("");
}

//key management
QString GnuPGPPlugin::generateKey(QString uId)
{
	return QString("");
}

bool GnuPGPPlugin::removeKey(QString keyId)
{
	return false;
}

QStringList GnuPGPPlugin::listKeys()
{
	return QStringList();
}

//Plugin Info
QString GnuPGPPlugin::about()
{
	return QString("");
}

Q_EXPORT_PLUGIN2(gnupgp_plugin, GnuPGPPlugin)
