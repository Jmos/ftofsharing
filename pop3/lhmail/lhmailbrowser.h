
/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAILBROWSER_H
#define LHMAILBROWSER_H

#include <QTextBrowser>

class QString;
class QDateTime;
class LHMail;
class LHMailBase;
class LHMailAddr;
class LHMailHeader;
class LHMailPart;
class LHSmtp;
class QTextStream;

class QPopupMenu;

class LHMailBrowserPrivate;

/**
 * This class represents widget to show mail with all attachments to user. <br>
 * Example:
 * <pre>
 * LHMailBrowser browser = new LHMailBrowser;
 * 
 * LHMail* mail;
 * mail->setData(received_mail_data);
 * 
 * browser->setMail(mail)
 * </pre>
 * LHMailBrowser shows HTML obtained from LHMail using @ref LHMail::getShowText.
 * You can extend functionality by handling signals @ref activateLink and @ref popupForLink.
 * For complete expample look at examples/browser.<br>
 * External appplications are handled using @ref LHExternalApp.
 *  
 * @short Widget for showing mail with attachments.
 * @see LHMail
 * @see LHMailPart
 * 
*/

#       ifdef Q_OS_WIN32
        __declspec(dllexport)
#       endif

class LHMailBrowser : public QTextBrowser
{
    Q_OBJECT

public:
	/**
	 * Constructs a LHMailBrowser widget.
	*/

	LHMailBrowser(QWidget* parent=0);
	
	/**
	 * Set mail to be shown. All temp data from current mail are cleaned now.
	 */ 
	virtual void setMail(LHMail* mail);
	
        virtual ~LHMailBrowser();
	
	/**
	 * Overloaded method. Do not use this method directly.
	 */ 
	virtual void setSource( const QString &name );
	
	/**
	 * Overloaded method. Do not use this method directly.
	 */ 
	QMenu* createPopupMenu( const QPoint& pos );
	
signals:
	/**
	 * Emited when user clicks on link and @ref LHMailBrowser is not handling it.
	 * Can be used to handle mailto:, http:// and other links.
	 * @param link Content of the link
	 */ 
	void activateLink(const QString& link);

	/**
	 * Emited when used clickes with right mouse button on link.
	 * By handling this signal you can add your specific actions for 
	 * specific links. Simply add actions to menu.
	 * @param link Content of the link
	 * @param menu Menu that will be showed to user
	 */ 
	void popupForLink(const QString& link, QMenu* menu);
	
private:

	LHMailBrowserPrivate* d;
};

#endif
