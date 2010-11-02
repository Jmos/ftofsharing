/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef BROWSER_H
#define BROWSER_H

#include <qwidget.h>

#include "lhmailbrowser.h"

class QLineEdit;
class QMultiLineEdit;
class QLabel;
class QPushButton;
class LHPop3;
class LHMail;
class QString;
class QListView;
class QPopupMenu;
class QAction;
class QTreeWidget;



class Browser : public QWidget
{
    Q_OBJECT

public:
    Browser( QWidget *parent = 0 );
    ~Browser();
	void newMail(LHMail* mail);
	

public slots:
	void finish();
	void activateLink(const QString& link);
	void popupForLink(const QString& link, QMenu* menu);
	void startWebBrowser();
	
private slots:
    void getMails();

private:
    QPushButton *get;
	QLabel * sendStatus;
	QTreeWidget * mails, *files;
	//QMultiLineEdit *message;
	LHMailBrowser * mb;
	QList<LHMail *> mail_list;
	QAction * actionWebBrowser;
	QString link;
};


#endif
