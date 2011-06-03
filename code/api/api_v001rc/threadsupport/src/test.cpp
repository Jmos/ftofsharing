/**
 *
 * @file test.cpp
 * @author muma
 * @date 03.06.2011
 *
 * Ricmar Technology GmbH  -  2011
 * Introduction_to_file
 *
 */

// Qt Includes

// Std Includes

// Linux Includes
#include "stdio.h"
#include "unistd.h"
// Windows Includes

// Library Includes

// Application Includes
#include "test.h"
// Defines

/**************************************************************************************/

Test::Test()
    {
    }

Test::~Test()
    {
    }

void Test::run(void*)
    {
    printf("log");
    sleep(1);
    printf("log1");
    sleep(1);
    printf("log2");
    }
