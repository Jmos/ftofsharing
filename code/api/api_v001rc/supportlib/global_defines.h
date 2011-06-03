#ifndef __GLOBALDEFINES_H__
#define __GLOBALDEFINES_H__
/**************************************************************************************
 *
 * Ricmar Technology GmbH   -   2011
 *
 * File global_defines.h , 03.06.2011, created by muma
 *
 **************************************************************************************/

// Qt Includes
#include <QByteArray>
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

typedef enum
    {
    eSysResUndef = -1,
    eSysResInt = 0,
    eSysResDouble = 1,
    eSysResString = 2,
    eSysResShort = 3,
    } SysResTypes;

typedef struct __SYSRES_DATA__
    {
    QByteArray data;
    SysResTypes type;
    }SysResData;

#endif //__GLOBALDEFINES_H__
