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
	bool readData(void* pTargetData,QString Filename,QString Name)
	{
	  if(Filename.isEmpty()||Name.isEmpty())
			return false;                       //FIXME LOGGING!

		QFile xmlData(Filename);

		QDomDocument objectDoc("objectType");
		QString errMsg;
		QByteArray readData;

		if(!objectDoc.setContent(&xmlData,&errMsg))
			{
			xmlData.close();
			printf("***************couldn't set content for DOM!. Errmsg: <%s>***************\n",qPrintable(errMsg));
			return false;
			}
		xmlData.close();

		QDomNodeList nodes = objectDoc.elementsByTagName(Name);

		for(int i =0;i<=nodes.size();i++)
			{
			QDomElement e = nodes.item(i).toElement();
			readData.append(e.attribute("value,"""));
			}

		memcpy(pTargetData,readData.constData(),sizeof(pTargetData));
	}
private:

};


template <class T> class XmlDataLib
{
	public:

	XmlDataLib(QString objName,QString Filename)
	{
		initData();
		XmlRead getData;

		if(!getData.readData(&m_data,Filename,objName))
			printf("shitz happenedz!\n");				//FIXME loggerklasse einbauen
	}

	~XmlDataLib()
	    {

	    }

	T& data()
	    {
	        return m_data;
	    };

	private:

	void initData()
	{
		memset(&m_data,0,sizeof(T));
	}

	T m_data;             ///< die Daten von \a m_plName
};

//Application Includes

/****************************************************************************/

#endif

