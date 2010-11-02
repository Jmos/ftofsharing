/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHEXTERNALAPP_H
#define LHEXTERNALAPP_H
#define UNIT_API __declspec(dllexport)
#include <QObject>
#include <QDebug>

class QString;
class LHMailBase;

//class LHMimePrivate;

/**
 * Helper class to manage calling, finding, assigning etc. of external applications
 * Used by @ref LHMailBrowser.
 * @short Helper class for handlig external applications.
 */
 
#ifdef Q_OS_WIN32
	UNIT_API
#endif

class LHExternalApp : public QObject
{
    Q_OBJECT

public:
	
	/**
	 * Used to ask user to assign application for given content type (like image/gif).
	 * @return Name of application
	 */ 
	static QString assignAppUser(const QString& ct);
	
	/**
	 * Used to show mail or mail part data in application. User is asked before, and
	 * has a chance to change application.
	 * @return True is succcess
	 */ 
	static bool showInAppUser(LHMailBase* mb);
	
	/**
	 * Calls application assigned to content type of mail or mail part.
	 * @return True is succcess
	 */ 
	static bool showInApp(LHMailBase* mb);
	
	/**
	 * Starts new process that represents application assigned to given mime type
	 * and gives file name as calling argument.
	 * @return True is succcess
	 */ 
	static bool startAppForMimeType(const QString& content_type, const QString& fname);

};

#endif
