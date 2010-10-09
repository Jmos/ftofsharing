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


typedef struct __FLIP_SETTINGS__
    {
    int dFlipUpAngle;
    int dFlipDownAngle;
    }__attribute__((__packed__)) FlipSettings;

int main (int argc, char** argv)
    {
    XmlDataLib<FlipSettings> dat(DATA_OBJECTTYPE);
    printf("%d \n",dat.data().dFlipDownAngle);
    return 0;
    }
/****************************************************************************/
