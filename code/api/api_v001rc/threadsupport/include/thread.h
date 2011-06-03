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

template <class X, class arg, class ret = void> class TempThread : public QThread
    {
    public:
        TempThread<X,arg,ret>* get(QString name, X* obj, ret (X::*runFunc)(arg))
            {
            return new TempThread<X,arg,ret>(name, obj, runFunc);
            }
        /**
         * @brief we use the run function, given in QThread to call tmpinstances
         */
        void run()
            {
            (*m_pObj.*m_ftpArg)(m_arg);
            }

        TempThread(QString name, X* obj, ret (X::*runFunc)(arg))
            {
            m_ftpArg = runFunc;
            m_pObj = obj;
            }

    private:


        ~TempThread()
            {
            }

    private:
        ret (X::*m_ftpArg)(arg);
        X* m_pObj;
        arg m_arg;
    };

#endif //__THREAD_H__
