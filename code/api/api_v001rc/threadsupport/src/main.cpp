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
#include "timer.h"
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
    TempThread<Test,void*>* ploop1 = new TempThread<Test,void*>("loop", x, &Test::run1);
    TempThread<Test,void*>* ploop2 = new TempThread<Test,void*>("loop", x, &Test::run2);

    ploop1->start();
    ploop2->start();

    while(ploop1->isRunning() || ploop2->isRunning())
        sleep(1);

    return 0;
    }

