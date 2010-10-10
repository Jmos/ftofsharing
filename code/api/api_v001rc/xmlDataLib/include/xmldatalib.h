#ifndef __XMLDATALIB_H__
#define __XMLDATALIB_H__

/*************************************************************************
 *
 * Ricmar Tech - 2010
 *
 * File < xmldatalib.h > created <15.09.2010 > by muma
 *
 **************************************************************************/

//QT Includes
#include <QStringList>
#include <QDomDocument>
#include <QString>
#include <QFile>

//Library Includes

class XmlRead
    {
public:
    bool readData(void* pTargetData, QString Filename, QString Name)
	{
	if (Filename.isEmpty() || Name.isEmpty())
	    return false; //FIXME LOGGING!

	QString fileAndPath;
	fileAndPath.append(Filename);
	QFile xmlData(fileAndPath);

	QDomDocument objectDoc("objectType");
	QString errMsg;
	QByteArray readData;

	if (!objectDoc.setContent(&xmlData, &errMsg))
	    {
	    xmlData.close();
	    printf("***************ERROR****************************************************\n");
	    printf("***************Path to file: <%s>***************************************\n",qPrintable(fileAndPath));
	    printf("***************couldn't set content for DOM!. Errmsg: <%s>**************",qPrintable(errMsg));
	    return false;
	    }

	if(!objectDoc.hasChildNodes())
	    {
	    printf("***************ERROR****************************************************\n");
	    printf("***************objectdoc has no Child nodes*****************************\n");
	    return false;
	    }

	QDomNodeList nodes = objectDoc.elementsByTagName(Name);

	for (int i = 0; i < nodes.size(); i++)
	    {
	    QDomNode n = nodes.item(i);
	    if(!n.hasChildNodes())
		{
		printf("***************ERROR*************************************************************\n");
		printf("***************Node:<%s> has no Child nodes**************************************\n");
		}
	    QDomNodeList subNodes = n.childNodes();
	    for(int x = 0; x< subNodes.size();x++)
		{
		if(!subNodes.item(x).isElement())
		    {
		    printf("***************ERROR*************************************************************\n");
		    printf("***************Node is no element!***********************************************\n");
		    return false;
		    }
		QDomElement e = subNodes.item(x).toElement();
		if(e.hasAttribute("value"))
		    {
		    printf("<%s>\n",qPrintable(e.attribute("value","30")));
		    readData.append(e.attribute("value","30").toInt());
		    }
		else
		    {
		    printf("***************ERROR*************************************************************\n");
		    printf("***************Node found as number <%d> has no elementattribute called value****\n",i);
		    printf("***************Elementname: <%s>, Attributename: <%s>****************************\n",qPrintable(e.tagName()),qPrintable(e.text()));
		    return false;
		    }
		}
	    }

	memcpy(pTargetData, readData.constData(), sizeof(pTargetData));
	return true;
	}
private:
    };

template<class T> class XmlDataLib
    {
public:

    XmlDataLib(QString objName, QString Filename = "")
	{
	initData();
	XmlRead getData;

	if (!getData.readData(&m_data, "objects.xml", objName))//fixme xmlfile irgendwie dynamisch übergeben. Konstruktor?
	    printf("shitz happenedz!\n"); //FIXME loggerklasse einbauen
	}

    ~XmlDataLib()
	{
	}

    T& data()
	{
	return m_data;
	}

private:

    void initData()
	{
	memset(&m_data, 0, sizeof(T));
	}

    T m_data; ///< die Daten von \a m_plName
    };

//Application Includes

/****************************************************************************/

#endif
