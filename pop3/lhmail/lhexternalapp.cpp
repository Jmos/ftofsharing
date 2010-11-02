
/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhexternalapp.h"					   
#include "lhmailbase.h"
#include "lhmime.h"
#include "lhfiletools.h"

#include <qstring.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qfiledialog.h>


QString LHExternalApp::assignAppUser(const QString& ct)
{
	QString app = QFileDialog::getOpenFileName(0,tr("Please find application for type: %1").arg(ct));
	if (app.isEmpty())
	{
		return app;
	}

	int answer = QMessageBox::question(
								  0,
								  tr("Assign application?"),
								  tr("Do you want to assign this application %1 to all attachments of this type?")
								  .arg(app),
								  tr("&Yes"), tr("&No"));

	if (answer == 0)
	{
		LHMime::setAppForMimeType(ct, app);
		qDebug()<<"LHMailBrowser::setSource, setting app: "<<app<<" content-type: "<<ct;
	}
	
	return app;
}

bool LHExternalApp::showInAppUser(LHMailBase* mb)
{
	const QString& ct = mb->contentType();
	QString app = LHMime::getAppForMimeType(ct);
	
	if (app.isEmpty())
	{
		app = assignAppUser(ct);
	}
	
	if (app.isEmpty())
	{
		return false;
	}
	
	while(1)
	{
		int answer = QMessageBox::question(
					 0,
					 tr("Open attachment?"),
					 tr("Do you want to open attachment in application %1?")
					 .arg(app),
					 tr("&Yes"), tr("&No"), tr("&Change App"));
		
		if (answer == 1)
		{
			return false;
		}
		
		if (answer == 2)
		{
			app = assignAppUser(ct);
		}
		
		if (app.isEmpty())
		{
			return false;
		}
		
		if (answer == 0)
		{
			break;
		}
	}
	
	return showInApp(mb);
};

bool LHExternalApp::showInApp(LHMailBase* mb)
{
	QString fname = mb->saveInTemp();
	QString ct = mb->contentType();
	qDebug()<<"LHMailBase::showInApp, fname: "<<fname<<", content-type: "<<ct;
	return startAppForMimeType( ct, fname);
}

bool LHExternalApp::startAppForMimeType(const QString& content_type, const QString& fname)
{
	qDebug()<<"LHExternalApp::startAppForMimeType 0, content_type: "<<content_type;
	
	QString app = LHMime::getAppForMimeType(content_type);
	
	if (app.isEmpty())
	{
		qWarning()<<"LHExternalApp::startAppForMimeType, could not find app for content_type: "<<content_type;
		return false;
	}
	
	qDebug()<<"LHExternalApp::startAppForMimeType, app: "<<app<<", content_type: "<<content_type;
		
	QProcess *proc = new QProcess();
	proc->start(app,QStringList(fname));

	/*
	connect( proc, SIGNAL(readyReadStdout()),
			 this, SLOT(readFromStdout()) );
	*/
	if (!proc->waitForStarted())
	{
		qWarning("LHExternalApp::startAppForMimeType, Can not start application");
		return false;
	}
	
	return true;
}

