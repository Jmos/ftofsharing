/*************************************************************************
 *
 * Ricmar Tech - 2010
 *
 * File < main.cpp > created <15.09.2010 > by muma
 *
 **************************************************************************/

//QT Includes

//Library Includes
#include "stdio.h"
#include "iostream"

//Application Includes
#include "data_defines.h"
#include "xmldatalib.h"

//QT Includes
#include <QStringList>
#include <QDomDocument>
#include <QString>
#include <QFile>


//typedef struct __FLIP_SETTINGS__
//    {
//    int dFlipUpAngle;
//    int dFlipDownAngle;
//    }__attribute__((__packed__)) FlipSettings;

int main (int argc, char** argv)
    {
    CXmlDataLib x(DATA_INTVALUE2);
    std::cout<<qPrintable(x.data())<<"\n";
    QString xhallo;
    xhallo="1";
    x=xhallo;
    std::cout<<qPrintable(x.giveType());
//
//    x.Write2Xml(xhallo);
//
    x.refresh();
    std::cout<<qPrintable(x.data())<<"\n";
    return 0;
    }
/****************************************************************************/
