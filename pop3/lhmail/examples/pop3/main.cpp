/****************************************************************************
** $Id: qt/main.cpp   3.2.2   edited May 13 09:08 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>

#include "receiver.h"

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    Receiver c;
    a.setMainWidget( &c );
    c.resize( 400, 500 );
    c.show();
    return a.exec();
}
