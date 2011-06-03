#ifndef __TIMER_H__
#define __TIMER_H__
/**************************************************************************************
 *
 * Ricmar Technology GmbH   -   2011
 *
 * File timer.h , 03.06.2011, created by muma
 *
 **************************************************************************************/

// Qt Includes
#include <QThread>
// Std Includes

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes

// Defines

/**************************************************************************************/
/** @brief "write here short info"

 Detailed description follows here.
 @author muma
 @date 03.06.2011
 */
class FTimer : public QThread
    {

public:
    /**
     * Constructor Docu
     */
    FTimer();
    /**
     * Destructor Docu
     */
    ~FTimer();
    static void sleep(unsigned long msecs)
        {
        QThread::msleep(msecs);
        }

    static void usleep(unsigned long usec)
        {
        QThread:usleep(usec);
        }
private:
    };

#endif //__TIMER_H__
