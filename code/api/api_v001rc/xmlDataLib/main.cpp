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

//QT Includes
#include <QStringList>
#include <QDomDocument>
#include <QString>
#include <QFile>

//Library Includes


int main (int argc, char** argv)
    {
	XmlDataLib<int> dat("obj","file");
    return 0;
    }

class XmlRead
{
public:
	bool readData(void* pTargetData,QString Filename,QString Name)
	{
	  if(Filename.isEmpty()||Name.isEmpty())
			return false;

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
			printf("shitz happenedz!");				//FIXME loggerklasse einbauen
	}

	~XmlDataLib();

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

/****************************************************************************/
