/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhfiletools.h"

#include <stdlib.h>
#include <QtCore>
#include <QApplication>

class LHFileToolsPrivate
{
public:
	static QString tmp_dir;
	static QString app_tmp_dir;
};

//TODO: windows, get user name, get app unige name/id
QString LHFileToolsPrivate::tmp_dir = "/tmp";

//Will be initialized when first time called getAppTempDir
QString LHFileToolsPrivate::app_tmp_dir;

void LHFileTools::cleanUp()
{
	removeAppTempDir();
}

QString LHFileTools::findUniqueDir(const QString& name, const QString& prefix)
{
	QDir d;
	QString rand_dir;
	
	while(1)
	{
#ifdef Q_OS_WIN32
		rand_dir = prefix + QString("%1").arg(rand());
#else
        rand_dir = prefix + QString("%1").arg(random());
#endif		
		d.setPath(name + QDir::separator () + rand_dir);
		
		if (!d.exists())
		{
			break;
		}
	}
	
	return rand_dir;
}

QString LHFileTools::writeTempFile(const QString& fname, const QByteArray& barr)
{
	
		
	QDir tmp_dir(getAppTempDir());
	if ( !tmp_dir.exists() )
	{
		tmp_dir.mkdir(getAppTempDir());
		//connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(cleanUp()));
	}
	
	QString name = findUniqueDir(getAppTempDir());
	
	QDir d(getAppTempDir() + QDir::separator () + name);
	
	d.mkdir(d.path());
	
	QString out = d.filePath(fname);
	QFile f(out);
	if (!f.open(QIODevice::WriteOnly))
	{
		qWarning()<<"Could not open tmp file: "<<out;
		return "";
	}

	f.write(barr);
	f.close();
	
	return out;
}

void LHFileTools::removeTemp(const QString& fname)
{
	QFileInfo fi(fname);
	QString dir = fi.absolutePath();
	cleanDir(dir, true);
}

void LHFileTools::cleanDir(const QString& dir, bool DEL)
{
	qDebug()<<"LHFileTools::cleanDir, removing dir: "<<dir;
	QDir d(dir);
	if ( !d.exists() )
	{
		qDebug()<<"LHFileTools::cleanDir, dir does not exists exiting: "<<dir;
		return;
	}
	
	d.setFilter(QDir::AllEntries);

	QFileInfoList list = d.entryInfoList();
	QFileInfo fi;

	while (list.count()>3)
	{
		qDebug()<<"LHFileTools::cleanDir, next file: "<<fi.fileName();
		fi=list.takeLast();
		if (fi.fileName() == "." || fi.fileName() == "..")
		{
			//continue;
			break;
		}
		if (!fi.isDir())
		{
			qDebug()<<"LHFileTools::cleanDir, removing file: "<<fi.fileName();
			d.remove(fi.fileName());
		} else if (fi.isDir())
		{
			cleanDir(fi.filePath());
		}
	}
	
	if (DEL)
		d.rmdir(dir);
}

void LHFileTools::cleanTemps()
{
	cleanDir(getAppTempDir(), FALSE);
}

void LHFileTools::setTempDir(const QString& tmp_dir)
{
	LHFileToolsPrivate::tmp_dir = tmp_dir;
}

QString LHFileTools::getTempDir()
{
	return LHFileToolsPrivate::tmp_dir;
}

QString LHFileTools::getAppTempDir()
{
	if (LHFileToolsPrivate::app_tmp_dir.isEmpty())
	{
		LHFileToolsPrivate::app_tmp_dir = findUniqueDir(LHFileToolsPrivate::tmp_dir);
	}
	
	return LHFileToolsPrivate::tmp_dir + QDir::separator() + LHFileToolsPrivate::app_tmp_dir;
}

void LHFileTools::removeAppTempDir()
{
	QString td = getAppTempDir();
	qDebug()<<"LHFileTools::removeAppTempDir, "<<td;
	cleanDir(td, TRUE);
}


