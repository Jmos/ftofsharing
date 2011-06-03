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
#include <QString>
// Std Includes

// Linux Includes
#include "stdio.h"
#include "unistd.h"
// Windows Includes

// Library Includes

// Application Includes
#include "test.h"
#include "msgQ.h"
// Defines
/**************************************************************************************/

Test::Test()
    {
    m_msgQID= MessageQ.AddMessageQ();
    }

Test::~Test()
    {
    }

void Test::run1(void*)
    {
     MessageQ.SendMessage(m_msgQID, 6, "Hallo, das ist ein Test!");
    }

void Test::run2(void*)
    {
    QString Message;
    MessageQ.ReceiveMessage(m_msgQID,6,Message);
    printf("%s",qPrintable(Message));
    }
