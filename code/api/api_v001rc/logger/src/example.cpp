/**
 *
 * @file example.cpp
 * @author muma
 * @date 24.10.2010
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
#include "example.h"
#include "stdio.h"
// Defines

/**************************************************************************************/

void Counter::setValue(int value)
    {
    LOG_DECLARE;
    if (value != m_value)
	{
	m_value = value;
	emit valueChanged(value);
	}
    }

