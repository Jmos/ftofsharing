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

// Windows Includes

// Library Includes

// Application Includes
#include "thread.h"
#include "stdio.h"
// Defines

/**************************************************************************************/
int main (int argc, char** argv)
    {
    ThrSupport *x = new ThrSupport(1);
    ThrSupport *y = new ThrSupport(2);
    ThrSupport *z = new ThrSupport(3);

    x->start(QThread::NormalPriority);
    y->start(QThread::NormalPriority);
    z->start(QThread::NormalPriority);
    getchar();
    return 0;
    }

