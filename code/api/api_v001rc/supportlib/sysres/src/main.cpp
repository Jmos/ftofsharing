/**
 *
 * @file main.cpp
 * @author muma
 * @date 20.10.2010
 *
 * Ricmar Technology GmbH  -  2010
 * Introduction_to_file
 *
 */

// Qt Includes
#include <QString>
// Std Includes

// Linux Includes
#include "stdio.h"
// Windows Includes

// Library Includes

// Application Includes
#include "sysres.h"
#include <iostream>
#include <typeinfo>
// Defines

/**************************************************************************************/
template <typename T>
class AClass {
public:
    static bool compare() {
        return (typeid(T) == typeid(int));
    }
};

int main(int , char** )
    {
    {
    SysRes::get()->registerResource<int>(2,"hallo1");
    SysRes::get()->registerResource<double>(3,"hallo2");
    SysRes::get()->registerResource<short>(4,"hallo3");
    SysRes::get()->registerResource<QString>("test","hallo4");
    }

    {
    int value;
    double value2;
    short value3;
    QString value4;

    if(!SysRes::get()->getResource<int>("hallo1",value))
        {
        std::cout <<"getresource failed!"<< "\n";
        }
    else
        {
        std::cout << value << "\n";
        }

    if(!SysRes::get()->getResource<double>("hallo2",value2))
        {
        std::cout <<"getresource failed!"<< "\n";
        }
    else
        {
        std::cout << value2 << "\n";
        }

    if(!SysRes::get()->getResource<short>("hallo3",value3))
        {
        std::cout <<"getresource failed!"<< "\n";
        }
    else
        {
        std::cout << value3 << "\n";
        }

//    if(!SysRes::get()->getResource<QString>("hallo4",value4))
//        {
//        std::cout <<"getresource failed!"<< "\n";
//        }
//    else
//        {
//        std::cout << qPrintable(value4) << "\n";
//        }
    }

    }

