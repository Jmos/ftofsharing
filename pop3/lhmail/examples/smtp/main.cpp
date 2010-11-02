/****************************************************************************
** $Id: qt/main.cpp   3.2.2   edited May 13 09:08 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#       include <qapplication.h>
#       include "composer.h"

// TEST
#       include "lhmime.h"
#       include "lhmail.h"
#       include "lhmailsigcheck.h"

int main( int argc, char **argv )
{
/*
        LHMail::initMailSystem ();
        if (LHMailSigCheck::check (NULL))
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "OK");
        else
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "ERROR");

        return 0;
*/
        // LHMailSignature::load ("/home/iwasz/req.pem", "/home/iwasz/key.pem", "/home/iwasz/file.txt");
        // return 0;
    QApplication a( argc, argv );

    Composer c;
    a.setMainWidget( &c );
    c.resize( 400, 500 );
    c.show();
    return a.exec();
}

