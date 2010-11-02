/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef COMPOSER_H
#define COMPOSER_H

#include <qwidget.h>


class QLineEdit;
class QMultiLineEdit;
class QTextEdit;
class QLabel;
class QPushButton;
class LHSmtp;
class LHMail;
class QString;
class QListView;
class QCheckBox;


class Composer : public QWidget
{
    Q_OBJECT

public:
    Composer( QWidget *parent = 0 );
    ~Composer();
    int testAddresses(const QString& addr, const QString& section);
    LHMail* createMessage();

private slots:
    void sendMessage();
    void saveMessage();
    void enableSend();
    void sentMail(LHMail* mail);
    void attachFile();
    void attachForward();
    void setBold();

private:
    QLineEdit *smtp_server, *from, *to, *cc, *bcc, *subject, *enc;
    QTextEdit *message;
    QLabel * sendStatus;
    QPushButton * send, *attach, *save, *forward, *bold;
    QListView * files;
    QCheckBox *html;
    LHSmtp* smtp;
};


#endif
