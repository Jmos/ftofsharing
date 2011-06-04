/**
 *
 * @file sysres.cpp
 * @author muma
 * @date 04.06.2011
 *
 * Ricmar Technology GmbH  -  2011
 * Introduction_to_file
 *
 */

// Qt Includes
#include <QByteArray>
// Std Includes

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes
#include "sysres.h"
// Defines

/**************************************************************************************/
SysRes *SysRes::m_pInstance = NULL;


ByteArrayWrapper::ByteArrayWrapper()
    {
    }

ByteArrayWrapper::~ByteArrayWrapper()
    {
    }

QByteArray ByteArrayWrapper::getValue(QString value)
    {
    QByteArray returnValue;
    return returnValue.append(value);
    }

QByteArray ByteArrayWrapper::getValue(double value)
    {
    return QByteArray::number(value);
    }

QByteArray ByteArrayWrapper::getValue(int value)
    {
    return QByteArray::number(value);
    }

QByteArray ByteArrayWrapper::getValue(short value)
    {
    return QByteArray::number(value);
    }

QString ByteArrayWrapper::getComplexValue(QByteArray value)
    {
    return QString(value);
    }
