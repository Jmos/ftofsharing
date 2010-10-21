/**
 *
 * @file thread.cpp
 * @author muma
 * @date 20.10.2010
 *
 * Ricmar Technology GmbH  -  2010
 * Example for thread
 *
 */

// Qt Includes

// Std Includes

// Linux Includes
#include "stdio.h"

// Windows Includes

// Library Includes

// Application Includes
#include "thread.h"
// Defines

/**************************************************************************************/

/**
 * Constructor_Intro
 */
ThrSupport::ThrSupport(int a)
{
    x=a;
}

/**
 * Destructor_Intro
 */
ThrSupport::~ThrSupport()
{
}

void ThrSupport::run()
    {
    for(;;)
	{
	printf("hallo,<%d>",x);
	printf("hallo,<%d>",x);
	}

    }
