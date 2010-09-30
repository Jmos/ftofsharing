/*************************************************************************
 *
 * Ricmar Tech - 2010
 *
 * File < main.cpp > created <15.09.2010 > by muma
 *
 **************************************************************************/

//QT Includes

//Library Includes

//Application Includes
#include "data_defines.h"
#include "xmldatalib.h"

//QT Includes
#include <QStringList>
#include <QDomDocument>
#include <QString>
#include <QFile>



int main (int argc, char** argv)
    {
	XmlDataLib<int> dat("obj","file");
    return 0;
    }
/****************************************************************************/
