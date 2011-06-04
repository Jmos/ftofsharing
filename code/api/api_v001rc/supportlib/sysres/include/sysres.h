#ifndef __SYSRES_H__
#define __SYSRES_H__
/**************************************************************************************
 *
 * Ricmar Technology GmbH   -   2011
 *
 * File sysres.h , 03.06.2011, created by muma
 *
 **************************************************************************************/

// Qt Includes
#include <QByteArray>
#include <QMap>
#include <QString>
// Std Includes

// Linux Includes
#include <typeinfo>
// Windows Includes

// Library Includes
#include "global_defines.h"
#include <iostream>
// Application Includes

// Defines

/**************************************************************************************/
/** @brief "write here short info"

 Detailed description follows here.
 @author muma
 @date 03.06.2011
 */

class ByteArrayWrapper
    {
public:
    ByteArrayWrapper();
    ~ByteArrayWrapper();

    static QByteArray getValue(int value);
    static QByteArray getValue(double value);
    static QByteArray getValue(short value);
    static QByteArray getValue(QString value);
    static QString getComplexValue(QByteArray value);
private:
    };

class SysRes
    {

public:
    /**
     * Constructor Docu
     */
    SysRes()
        {
        }
    /**
     * Destructor Docu
     */
    ~SysRes()
        {
        }

    static SysRes *get()
        {
        if (!m_pInstance)
            m_pInstance = new SysRes();

        return m_pInstance;
        }

    template<class T> void registerResource(T value, QString name)
        {
        if (typeid(T) == typeid(int))
            {
            std::cout << "int"<< "\n";
            m_oMap[name] = ByteArrayWrapper::getValue(value);
            std::cout << m_oMap[name].toInt() << "\n";
            }
        else if (typeid(T) == typeid(double))
            {
            std::cout << "double"<< "\n";
            m_oMap[name] = ByteArrayWrapper::getValue(value);
            }
        else if (typeid(T) == typeid(short))
            {
            std::cout << "short"<< "\n";
            m_oMap[name] = ByteArrayWrapper::getValue(value);
            }
        else if (typeid(T) == typeid(QString))
            {
            std::cout << "QString"<< "\n"; //NOT WORKING!!
            m_oMap[name] = ByteArrayWrapper::getValue(value);
            return;
            }
        else
            {
            std::cout << "unknown";
            }
        }

    bool unregisterResource(QString name)
        {
        if(m_oMap.contains(name))
            {
            m_oMap.remove(name);
            return true;
            }
        else
            {
            return false;
            }
        }

    template<class T> bool getResource(QString name, T &value)
        {
        if (typeid(T) == typeid(int))
            {
            std::cout << "int" << "\n";
            bool ok = true;
            value = m_oMap[name].toInt(&ok);
            return ok;
            }
        else if (typeid(T) == typeid(double))
            {
            std::cout << "double" << "\n";
            bool ok = true;
            value = m_oMap[name].toDouble(&ok);
            return ok;
            }
        else if (typeid(T) == typeid(QString))
            {
            std::cout << "QString" << "\n"; // NOT SUPPORTED
//            value = ByteArrayWrapper::getComplexValue(m_oMap[name]);
            return false;
            }
        else if (typeid(T) == typeid(short))
            {
            std::cout << "short" << "\n";
            bool ok = true;
            value = m_oMap[name].toShort(&ok);
            return ok;
            }
        else
            {
            std::cout << "unknown" << "\n";
            return false;
            }
        }
private:
    static SysRes* m_pInstance;
    QMap<QString,QByteArray> m_oMap;
    };

#endif //__SYSRES_H__
