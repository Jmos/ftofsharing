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
// Application Includes

// Defines

/**************************************************************************************/
/** @brief "write here short info"

 Detailed description follows here.
 @author muma
 @date 03.06.2011
 */
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
        SysResData data;
        data = getData<T> (value);

        m_oMap[name] = data;
        }
    bool unregisterResource(QString name)
        {
        if (m_oMap.contains(name))
            {
            m_oMap.remove(name);
            return true;
            }
        else
            {
            return true;
            }
        }
    template<class T> bool getResource(QString name, T &value)
        {
        if (m_oMap.contains(name))
            {
            value = getData<T> (name);
            return true;
            }
        else
            {
            return false;
            }
        }
private:
    template<class T> SysResData getData(T value)
        {
        SysResData returnValue;
        if (typeid(value) == typeid(int))
            {
            returnValue.type = eSysResInt;
            returnValue.data = QByteArray((const char*) &value, sizeof value);
            }
        else if (typeid(value) == typeid(double))
            {
            returnValue.type = eSysResDouble;
            returnValue.data = QByteArray((const char*) &value, sizeof value);
            }
        else if (typeid(value) == typeid(QString))
            {
            returnValue.type = eSysResString;
            returnValue.data = QByteArray(value);
            }
        else if (typeid(value) == typeid(short))
            {
            returnValue.type = eSysResShort;
            returnValue.data = QByteArray((const char*) &value, sizeof value);
            }
        else
            {
            returnValue.type = eSysResUndef;
            returnValue.data = QByteArray();
            }
        }


    template<class T> T getData(QString name)
        {
        if (m_oMap[name].type == eSysResInt)
            {
            return m_oMap[name].data.toInt();
            }
        else if (m_oMap[name].type == eSysResDouble)
            {
            return m_oMap[name].data.toDouble();
            }
        else if (m_oMap[name].type == eSysResString)
            {
//            return QString(m_oMap[name].data());
            }
        else if (m_oMap[name].type == eSysResShort)
            {
            return m_oMap[name].data.toShort();
            }
        else
            {
            return T();
            }
        }
    static SysRes* m_pInstance;
    QMap<QString, SysResData> m_oMap;
};

SysRes *SysRes::m_pInstance = NULL;

#endif //__SYSRES_H__
