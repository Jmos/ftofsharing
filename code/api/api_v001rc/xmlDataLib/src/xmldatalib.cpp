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
    bool bFound = false;
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

    QDomElement mainRoot = objectDoc.documentElement();
    QDomElement hierarchyRoot = mainRoot.firstChildElement();
    while (!hierarchyRoot.isNull() && !bFound)
	{
	QDomElement Elements = hierarchyRoot.firstChildElement();
	while(!Elements.isNull() && !bFound)
	    {
	    if(Elements.tagName() == objName)
		{
		if (Elements.hasAttribute("value"))
		    {
		    output=Elements.attribute("value", "30");
		    bFound=true;
		    }
		else
		    {
		    std::cout<<"no attribute called value!\n";
		    }
		}
	    Elements = Elements.nextSiblingElement();
	    }

	hierarchyRoot = hierarchyRoot.nextSiblingElement();
	}

    if(!bFound)
	{
	std::cout<<"******************************ERROR****************************";
	std::cout<<"*********************No element in Hierarchies called"<<qPrintable(objName)<<"***************";
	return false;
	}

    return true;
    }

bool CXmlHandler::writeData(QString objName,QString input)
    {

    bool bFound = false;
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

    QDomElement mainRoot = objectDoc.documentElement();
    std::cout<<qPrintable(mainRoot.tagName())<<"\n";
    QDomElement hierarchyRoot = mainRoot.firstChildElement();
    std::cout<<qPrintable(hierarchyRoot.tagName())<<"\n";
    while (!hierarchyRoot.isNull() && !bFound)
	{
	QDomElement Elements = hierarchyRoot.firstChildElement();
	std::cout<<qPrintable(Elements.tagName())<<"\n";
	std::cout<<qPrintable(objName)<<"\n";
	while(!Elements.isNull() && !bFound)
	    {
	    if(Elements.tagName() == objName)
		{
		if (Elements.hasAttribute("value"))
		    {
		    Elements.setAttribute("value", input);
		    std::cout << qPrintable(objectDoc.toString());
		    xmlData.open(QIODevice::ReadWrite|QIODevice::Truncate);
		    QTextStream out(&xmlData);
		    out << objectDoc.toString();
		    xmlData.close();
		    bFound=true;
		    }
		else
		    {
		    std::cout<<"NO ATTRIBUTE CALLED VALUE!" << std::endl;
		    }
		}
	    Elements = Elements.nextSiblingElement();
	    }
	hierarchyRoot = hierarchyRoot.nextSiblingElement();
	}

    if(!bFound)
	{
	std::cout<<"******************************ERROR****************************";
	std::cout<<"*********************No element in Hierarchies called"<<qPrintable(objName)<<"***************";
	return false;
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

