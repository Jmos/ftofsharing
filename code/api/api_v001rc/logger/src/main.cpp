/**
 *
 * @file main.cpp
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
#include <QObject>
#include "stdio.h"
// Defines

/**************************************************************************************/

int main (int argc, char** argv)
    {
    Counter a, b;
    QObject::connect(&a, SIGNAL(valueChanged(int)),&b, SLOT(setValue(int)));

    a.setValue(12);     // a.value() == 12, b.value() == 12
    printf("<%d>",b.m_value);
    getchar();
    b.setValue(48);     // a.value() == 12, b.value() == 48
    printf("<%d>",a.m_value);
    getchar();
    }
