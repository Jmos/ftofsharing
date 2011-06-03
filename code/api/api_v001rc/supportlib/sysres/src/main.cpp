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
#include <QString>
// Std Includes

// Linux Includes
#include "stdio.h"
// Windows Includes

// Library Includes

// Application Includes
#include "sysres.h"
// Defines

/**************************************************************************************/
int main(int argc, char** argv)
    {
    {
    QString val("test");
    SysRes::get()->registerResource<QString>(val,"test");
    }
    {
    QString val;
    SysRes::get()->getResource<QString>("test",val);
    printf("%s",qPrintable(val));
    }

    }

