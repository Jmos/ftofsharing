/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef RECEIVER_H
#define RECEIVER_H

#include <qwidget.h>


class QLineEdit;
class QMultiLineEdit;
class QLabel;
class QPushButton;
class LHPop3;
class LHMail;
class QString;
class QListView;
class QCheckBox;
class LHMailBrowser;



class Receiver : public QWidget
{
    Q_OBJECT

public:
    Receiver( QWidget *parent = 0 );
	~Receiver();

private slots:
	void newMail(LHMail*);
    void getMails();
	

private:
    QLineEdit *pop3_server, *user, *pass;
    QPushButton *get;
	QLabel * sendStatus;
	LHPop3* pop3;
	QListView * mails, *files;
	//QMultiLineEdit *message;
	LHMailBrowser *message;
	QCheckBox *dele;
	QPtrList<LHMail> mail_list;
};


#endif
