/**
 *
 * @file logger.cpp
 * @author muma
 * @date 20.10.2010
 *
 * Ricmar Technology GmbH  -  2010
 * logger.cpp allows us to log different things into files and console. So this is a supportclass
 *
 */

// Qt Includes
#include <QString>
// Std Includes

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes
#include "loggerlib.h"
#include "logger.h"
// Defines

/**************************************************************************************/

/**
 * 0
 */
Logger::Logger()
{
}

/**
 * 0
 */
Logger::~Logger()
{
}

/** log() logs a text to a file or console
 *@param QString Text
 *@return void
 *
 * From the specified makro in loggerlib.h, this method will be called.
 */
void Logger::log_base(QString text)
    {
    //Msgq send!
    }

/********************************************************************************************/

LogLoop::LogLoop()
    {
    }

LogLoop::~LogLoop()
    {
    }

void LogLoop::setFileLoggerOn(QString Filename)
    {
    if(Filename.isEmpty())
	return;
    internFilename = Filename;
    fileLogger = true;
    }

void LogLoop::run()
    {
    return;
    }

