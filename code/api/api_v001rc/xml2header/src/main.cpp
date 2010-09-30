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
}XMLStruct;

int main(int argc, char** argv)
    {
    QFile resData("res.dat");
    if(!resData.exists("res.dat"))
        {
        printf("****************res.dat does not exist or is in the wrong folder. Please check if res.dat exists*********************\n");
        return -1;
        }

    //in Stream einlesen.
    QTextStream in(&resData);
    QStringList data;
    QStringList objectFiles;
    QStringList dataFiles;
    QStringList tmp;


    //Wir brauchen nur ReadOnly, file muss nicht beschreibbar sein --> Paranoia = readonly
    if(!resData.open(QIODevice::ReadOnly))
        {
        printf("***************res.dat couldnt be opened. Please check the rights and owner of the file and program******************\n");
        return -1;
        }

    //lesen der Zeilen
    while(!in.atEnd())
        {
        data.append(in.readLine());
        }

    resData.close();

    //Paranoiacheck
    if(data.size()==0)
        {
        printf("***************Size of List is 0. Seems like nothing is in the file. Please check the file******************\n");
        return -1;
        }


    for(int i = 0;i<data.size();i++)
        {
        tmp = data[i].split("=");

        if(tmp.size()>2)
            {
            printf("***************Line %d has wrong format. Please check the file  ******************\n",i);
            return -1;
            }

        if(tmp[0]=="objectsfile")
            objectFiles.append(tmp[1]);
        else if(tmp[0]=="datafile")
            dataFiles.append(tmp[1]);
        }

    for(int i = 0; i<objectFiles.size();i++)
        {
        printf("***************Data Objects: <%s>***************\n",qPrintable(objectFiles[i]));
        }


    for (int i = 0; i<dataFiles.size();i++)
        {
        printf("***************Data only files: <%s>***************\n",qPrintable(dataFiles[i]));
        }

    QFile dataHeader("../data_defines.h");
    if(dataHeader.exists("data_defines.h"))
        dataHeader.remove("data_defines.h");

    dataHeader.open(QIODevice::WriteOnly);

    QTextStream out(&dataHeader);
    out<<"#ifndef __DATA_DEFINES_H_ \n#define __DATA_DEFINES_H_\n\n\n";

    for(int i =0;i<objectFiles.size();i++)
        {
        out<<"// Begin writing data from "<<objectFiles[i]<<"\n\n\n";
        resData.setFileName(objectFiles[i]);
        if(!resData.open(QIODevice::ReadOnly))
            {
            printf("***************couldnt open file <%s>!***************\n",qPrintable(objectFiles[i]));
            return -1;
            }

        QDomDocument objectDoc("objectType");
        QString errMsg;
        if(!objectDoc.setContent(&resData,&errMsg))
            {
            resData.close();
            printf("***************couldn't set content for DOM!. Errmsg: <%s>***************\n",qPrintable(errMsg));
            return -1;
            }
        resData.close();

//        QDomElement root = objectDoc.documentElement();
//
//        if(root.tagName() != "objectType")
//            {
//            printf("***************tagname is wrong. Check tagname of file <%s>***************\n",qPrintable(objectFiles[i]));
//            return -1;
//            }

        QDomElement root = objectDoc.documentElement();
//        QDomNode n = root.firstChild();
        QList<XMLStruct> fileEntrys;
            while( !root.isNull() )
            {
              QDomElement e = root.toElement();
              if( !e.isNull() )
              {
//                if( e.tagName() == "Object" )
//                {
              XMLStruct values;
              values.name = e.tagName();
              values.Struct = e.attribute("type","");

              printf("******************name of object:<%s> objectstruct:<%s>\n",qPrintable(values.name),qPrintable(values.Struct));
              fileEntrys.append(values);
//                }
              }

            root=root.nextSiblingElement();
            }
        for(int i = 0;i<fileEntrys.size();i++)
            {
            out<<"#define DATA_"<<fileEntrys[i].name.toUpper();
            out<<"\t\t\t"<<"\""<<fileEntrys[i].name;
            out<<"\"\t\t\t"<<"//"<<fileEntrys[i].Struct;
            out<<"\n";
            }
        }
    out<<"\n\n\n#endif";
    dataHeader.close();
    return 0;
    }
/****************************************************************************/
