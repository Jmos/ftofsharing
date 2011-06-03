/**
 *
 * @file main.cpp
 * @author muma
 * @date 20.10.2010
 *
 * Ricmar Technology GmbH  -  2010
 * Introduction_to_file
 *
 */

// Qt Includes
// Std Includes

// Linux Includes
#include "unistd.h"
// Windows Includes

// Library Includes

// Application Includes
#include "thread.h"
#include "test.h"
#include "stdio.h"
// Defines

/**************************************************************************************/
int main(int argc, char** argv)
    {
    //    ThrSupport *x = new ThrSupport(1);
    //    ThrSupport *y = new ThrSupport(2);
    //    ThrSupport *z = new ThrSupport(3);

    //
    //    x->start(QThread::NormalPriority);
    //    y->start(QThread::NormalPriority);
    //    z->start(QThread::NormalPriority);
    //    getchar();

    Test *x = new Test;
    TempThread<Test,void*>* ploop = new TempThread<Test,void*>("loop", x, &Test::run);

    ploop->start();

    while(ploop->isRunning())
        sleep(1);

    return 0;
    }

