/*************************************************************************
 *
 * Ricmar Tech - 2010
 *
 * File < main.cpp > created <12.09.2010 > by muma
 *
 **************************************************************************/

//QT Includes
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDomDocument>

//Library Includes
#include <stdio.h>
//Application Includes

typedef struct _XMLStruct
    {
    QString name;
    QString Struct;
    } XMLStruct;

int main()
    {
    //Xmlfile has to be in res/objects.xml!
    QFile resData("./res/objects.xml");

    //
    if (!resData.open(QIODevice::ReadOnly))
	{
	printf("***************couldnt open file objects.xml!***************\n");
	return -1;
	}
    else
	{
	printf("***************opening file objects.xml!***************\n");
	}

    QFile dataHeader("include/data_defines.h");
    if (dataHeader.exists("data_defines.h"))
	dataHeader.remove("data_defines.h");

    dataHeader.open(QIODevice::WriteOnly);

    QTextStream out(&dataHeader);
    out << "#ifndef __DATA_DEFINES_H_ \n#define __DATA_DEFINES_H_\n\n\n";

    out << "// Begin writing data from objects.xml \n\n\n";

    QDomDocument objectDoc("objectType");
    QString errMsg;
    if (!objectDoc.setContent(&resData, &errMsg))
	{
	resData.close();
	printf(
		"***************couldn't set content for DOM!. Errmsg: <%s>***************\n",
		qPrintable(errMsg));
	return -1;
	}
    resData.close();

    QDomElement root = objectDoc.documentElement();
    QList<XMLStruct> fileEntrys;
    while (!root.isNull())
	{
	QDomElement e = root.toElement();
	if (!e.isNull())
	    {
	    XMLStruct values;
	    values.name = e.tagName();
	    values.Struct = e.attribute("type", "");

	    printf("******************name of object:<%s> objectstruct:<%s>\n",
		    qPrintable(values.name), qPrintable(values.Struct));
	    fileEntrys.append(values);
	    //                }
	    }
	root = root.nextSiblingElement();
	}
    for (int i = 0; i < fileEntrys.size(); i++)
	{
	out << "#define DATA_" << fileEntrys[i].name.toUpper();
	out << "\t\t\t" << "\"" << fileEntrys[i].name;
	out << "\"\t\t\t" << "//" << fileEntrys[i].Struct;
	out << "\n";
	}
    out << "\n\n\n#endif";
    dataHeader.close();
    return 0;
    }
/****************************************************************************/
