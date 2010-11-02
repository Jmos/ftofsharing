


/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHFILETOOLS_H
#define LHFILETOOLS_H

#include <QObject>

class QString;

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

/**
 * Helper class to manage temp files
 */

class LHFileTools : public QObject
{
    Q_OBJECT

public:
		
	/**
	 * Finds name of direcotry that does not exists in given directory
	 */ 
	static QString findUniqueDir(const QString& name, const QString& prefix = "");
	
	/**
	 * Deletes all content of directory (recursivly).
	 * @param DEL True if you want to delete directory as well
	 */ 
	static void cleanDir(const QString& dir, bool DEL = TRUE);
	
	/**
	 * Writes data to temp file. File has name fname and is unique directory.
	 * @return Name of temp file with full path
	 */ 
	static QString writeTempFile(const QString& fname, const QByteArray& barr);
	
	/**
	 * Removes file and his temp directory. See @ref writeTempFile.
	 */ 
	static void removeTemp(const QString& fname);
	
	/**
	 * Cleans all content of this application temp directory.
	 */ 
	static void cleanTemps();
	  
	/**
	 * Set top temp dir. In Unix it can be p.e. '/temp'
	 */ 
	static void setTempDir(const QString& name);
	
	/**
	 * Retrieve the name of top temp directory
	 */ 
	static QString getTempDir();
	
	/**
	 * Each application has unique dir in temp dir. Using this method
	 * you can find out the name of this directory for this running application.
	 */ 
	static QString getAppTempDir();

	/**
	 * This method is called automaticaly when application finishes.
	 * It cleans all content and deletes applications temp directory.
	 */ 
	static void removeAppTempDir();
	
private slots:
	void cleanUp();
	
};

#endif
