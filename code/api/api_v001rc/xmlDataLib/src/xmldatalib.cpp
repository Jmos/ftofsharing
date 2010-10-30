/**
 *
 * @file xmldatalib.cpp
 * @author marco
 * @date 26.10.2010
 *
 */

// Qt Includes
#include <QString>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
// Std Includes

// Linux Includes
#include "stdio.h"
#include "iostream"
// Windows Includes

// Library Includes

// Application Includes
#include "xmldatalib.h"

// Defines

CXmlDataLib::CXmlDataLib(QString objName)
    {
    readObjName = objName;
    if(!refresh())
	{
	std::cout<<"***********ERROR***************";
	std::cout<<"******failed to read data. Objectname was:"<<qPrintable(objName)<<"*********";
	}
    }

CXmlDataLib::~CXmlDataLib()
    {
    }

QString CXmlDataLib::data()
    {
    return objData;
    }

bool CXmlDataLib::refresh()
    {
    if(!readData(readObjName,objData))
	{
	std::cout<<"failed to refresh data!";
	}
    return true;
    }

QString CXmlDataLib::giveType()
    {
    std::cout<<"This method is not available!";
    return false;
    }

CXmlDataLib CXmlDataLib::operator = (QString iData)
{
    std::cout << "test\n";
    if (!writeData (readObjName, iData))
    {
        std::cout << "Failed to write data!";
    }
}


/**************************************************************************************/

CXmlHandler::CXmlHandler()
    {

    }

CXmlHandler::~CXmlHandler()
    {

    }

bool CXmlHandler::readData(QString objName,QString& output)
    {
    if(objName.isEmpty())
	{
	std::cout<<"***********ERROR*********\n";
	std::cout<<"objName is empty, please check objName\n";
	return false;
	}

    QFile xmlData("objects.xml");

    QDomDocument objectDoc("objectType");
    QString errMsg;
    QByteArray readData;

    if (!objectDoc.setContent(&xmlData, &errMsg))
	{
	xmlData.close();
	std::cout<<"***************ERROR****************************************************\n";
	std::cout<<"***************couldn't set content for DOM!. Errmsg: <%s>**************\n" << qPrintable(errMsg);
	return false;
	}

    if (!objectDoc.hasChildNodes())
	{
	std::cout<<"***************ERROR****************************************************\n";
	std::cout<<"***************objectdoc has no Child nodes*****************************\n";
	return false;
	}

    QDomNodeList nodes = objectDoc.elementsByTagName(objName);

    if (nodes.size() > 1)
	{
	std::cout<<"***************WARNING********************************************************\n";
	std::cout<<"********There is more than one object with that name! Taking first one********\n";
	}

    QDomNode n = nodes.item(0);
    if (!n.hasChildNodes())
	{
	std::cout<<"***************ERROR*************************************************************\n";
	std::cout<<"***************Node has no Child nodes*************************************\n";
	}
    QDomNodeList subNodes = n.childNodes();
    for (int x = 0; x < subNodes.size(); x++)
	{
	if (!subNodes.item(x).isElement())
	    {
	    std::cout<<"***************ERROR*************************************************************\n";
	    std::cout<<"***************Node is no element!***********************************************\n";
	    return false;
	    }
	QDomElement e = subNodes.item(x).toElement();
	if (e.hasAttribute("value"))
	    {
	    printf("<%s>\n", qPrintable(e.attribute("value", "30")));
	    output=e.attribute("value", "30");
	    }
	else
	    {
	    std::cout<<"***************ERROR*************************************************************\n";
	    std::cout<<"***************Node found as number <%d> has no elementattribute called value****\n";
	    std::cout<<"***************Elementname:"<<qPrintable(e.tagName())<<", Attributename: "<<qPrintable(e.text())<<"****************************\n";
	    return false;
	    }
	}
    return true;
    }

bool CXmlHandler::writeData(QString objName,QString input)
    {

    if(objName.isEmpty() || input.isEmpty())
	{
	std::cout<<"***********ERROR*********\n";
	std::cout<<"objName is empty, please check objName\n";
	return false;
	}

    std::cout<<"bla\n";
    QFile xmlData("objects.xml");
    xmlData.open(QIODevice::ReadOnly);
    QDomDocument objectDoc("objectType");
    QString errMsg;

    if (!objectDoc.setContent(&xmlData, &errMsg))
	{
	xmlData.close();
	std::cout<<"***************ERROR****************************************************\n";
	std::cout<<"***************couldn't set content for DOM!. Errmsg:"<<qPrintable(errMsg)<<"**************";
	return false;
	}
    xmlData.close();

    if (!objectDoc.hasChildNodes())
	{
	std::cout<<"***************ERROR****************************************************\n";
	std::cout<<"***************objectdoc has no Child nodes*****************************\n";
	return false;
	}
    QDomNodeList nodes = objectDoc.elementsByTagName(objName);

    QDomNode n = nodes.item(0);
    if (!n.hasChildNodes())
	{
	std::cout<<"***************ERROR*************************************************************\n";
	std::cout<<"***************Node has no Child nodes**************************************\n";
	}
    QDomNodeList subNodes = n.childNodes();

    if (!subNodes.item(0).isElement())
	{
	std::cout<<"***************ERROR*************************************************************\n";
	std::cout<<"***************Node is no element!***********************************************\n";
	return false;
	}

    QDomElement e = subNodes.item(0).toElement();
    if (e.hasAttribute("value"))
	{
	e.setAttribute("value", input);
	std::cout << qPrintable(objectDoc.toString());
	xmlData.open(QIODevice::ReadWrite|QIODevice::Truncate);
	QTextStream out(&xmlData);
	out << objectDoc.toString();
	xmlData.close();
	}
    else
	{
	std::cout<<"NO ATTRIBUTE CALLED VALUE!" << std::endl;
	}
    return true;
    }

/**************************************************************************************/

void CXmlDataLib::Write2Xml(QString iData)
{
    std::cout << "test\n";
    if (!writeData (readObjName, iData))
    {
        std::cout << "Failed to write data!";
    }
}

