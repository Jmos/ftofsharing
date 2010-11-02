#include "pluginhandler.h"
//
PluginHandler::PluginHandler(  ) 
	: QObject()
{
	si=0;
	//populate the plugin-list
	QDir pluginsDir = QDir(qApp->applicationDirPath());
	pluginsDir.cd("../../mail_engine/plugins");
	

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) {
			SecurityInterface *securityInterface = qobject_cast<SecurityInterface *>(plugin);
			if(securityInterface){
				plugins.append(QPair<QString, QString>(fileName,pluginsDir.absoluteFilePath(fileName)));
			}
		}
	}
}

QString PluginHandler::micalg()
{
	if(si->autoHash()) return si->micalg(); //if the plugin hashes automatically, use its hash-symbol
	
	/*
	* TODO routine to return hash-symbol of other plugins 
	*/
	return "";
}

void PluginHandler::setActivePlugin(const QString &aP)
{
	enabled=false;
	
	activePlugin=aP;
	QPluginLoader loader(activePlugin);
	if(loader.errorString()!="") lastErr=loader.errorString();
	QObject *plugin = loader.instance();
	if(plugin){
		si=qobject_cast<SecurityInterface *>(plugin);
		if(!si) lastErr=loader.errorString();
	} else lastErr=loader.errorString();
	
	if(!si->ini()) lastErr=si->lastError();
	
	if(lastErr=="") enabled=true;
}

QString PluginHandler::sign(const QString &text, const QString &key)
{
	if (si->autoHash()){
		//the plugin signs only a hash of the given input, there is no need of generating a hash
		//on our own
		
	}else{
		
	}
}


//
