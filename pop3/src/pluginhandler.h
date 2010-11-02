#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H
//
#include <QObject>
#include <QtCore>
#include "securityinterface.h"

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
class PluginHandler : public QObject
{
Q_OBJECT

private:
	QList<QPair<QString,QString> > plugins;
	QString activePlugin; //absolute path to active plugin
	bool enabled; //variable to control functionality of plugin
	QString lastErr;
	SecurityInterface *si;

public:
	PluginHandler();
	QString lastError(){return lastErr;};
	QString getActivePlugin(){return activePlugin;};
	QList<QPair<QString,QString> > getPlugins(){return plugins;};
	QString micalg(); //return hash-symbol for signatures
	bool pluginEnabled(){return enabled;};
	void setActivePlugin(const QString &aP);

//gateway-functions to plugins, forwarding to plugin
	QString encrypt(const QString &text, const QString &key="");
	QString decrypt(const QString &text, const QString &key="");
	QString sign(const QString &text, const QString &key="");
	QString verify(const QString &text, const QString &key="");
	QString generateKey(QString uId);
	bool removeKey(QString keyId);
	QStringList listKeys();


	
};
#endif
