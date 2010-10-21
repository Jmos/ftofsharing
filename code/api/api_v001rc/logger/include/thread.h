    #ifndef __THREAD_H__
#define __THREAD_H__
/**************************************************************************************
 *
 * Ricmar Technology GmbH   -   2010
 *
 * File thread.h , 20.10.2010, created by muma
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

class ThrSupport : public QThread
    {

    public:
	ThrSupport(int a);
	~ThrSupport();
	void run();
    private:
	int x;
    };

#endif //__THREAD_H__
