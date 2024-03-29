#ifndef __XMLDATALIB_H
#define __XMLDATALIB_H

/*************************************************+
 *
 * File xmldatalib2.h created 26.10.2010 from marco
 *
 */
//QT Includes
#include <QString>
//API Includes

typedef struct __READ_DATA_RETURN__
	{
	QString value;
	QString type;
	}readDataReturn;


/**
 * @brief CXmlHandler is the private "under the hood" access class to our XML Files
 * This class is not designed for standalone using. Please use CXmlDataLib to access the Data
 *@author marco
 *@date 26.10.2010
 */
class CXmlHandler
    {
public:
    /**
     * no special Constructor behaviour
     */
    CXmlHandler();
    /**
     * no special Destructor behaviour
     */
    ~CXmlHandler();

    /**
     *The method readData accesses the XML Data, and returns it to you.
     *See xml2header documentation and the example file in xml2header
     *@param objName - objName stands here for the to accessing object
     *@param output - The Value, which is now actual in this object
     *@return true if it worked, false if not. If false see log
     */
    bool readData(QString objName, readDataReturn &output);
    /**
     *The method writeData accesses an object with objName and writes input in the object
     *@param objName - objName stands here for the to accessing object
     *@param input - The Value you want to write in the object
     *@return true if it worked, false if not. If false see log
     */
    bool writeData(QString objName, readDataReturn input);

private:
    };
/**
 *@brief CXmlDataLib is the public accessClass to our configuration XML Files
 *CXmlDataLib gives you an easy access to our XML DataFiles. Important for this handling is, that the
 *file has to have the correct name ("objects.xml") and it has to be in the same directory, the binary file is.
 *Also, if you have errors, please check the logging, it should get you access to what problems you had.
 *
 *@author marco
 *@date 26.10.2010
 */
class CXmlDataLib: private CXmlHandler
    {
public:
    /**
     * The Constructor XmlDatalib reads per default de value of the objects.xml file with the given
     * object name
     * @param objName -  objectName you want to access
     */
    CXmlDataLib(QString objName);
    /**
     * No special destructor behaviour
     */
    ~CXmlDataLib();

    /*
     *With the method data, you can get access to the data, you read before with the constructor or the refresh function
     *@return data from the initialized objectname
     */
    QString data();

    /**
     *The refresh method "refreshes" the values of the objectData and objectType
     */
    bool refresh();

    /**
     * This operator overload is used, to write data into the xmlFile we have initialized with the constructor
     * @param iData - this Data will be written into the XMLFile
     */
    CXmlDataLib operator=(QString);

    /**
     * This operator overload is also used to write data into the xmlFile.
     * You should only use this, when you want to change the type of the entry
     * @see operator=(QString)
     * @param readDataReturn iData - incoming data
     */
    CXmlDataLib operator=(readDataReturn);

    /**
     * For special purposes you might want to know the Type of the data you read. With the Method "giveType" this is possible
     * @param bool foreRefresh - if true, it refreshes the complete data (internally it calls only the refresh method)
     * @return Type of the initialized objectname
     */
    QString giveType(bool forceRefresh=false);


private:


    QString objData;
    QString objType;
    QString readObjName;
    };

#endif
