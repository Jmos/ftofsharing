#ifndef __LOGGERLIB_H__
#define __LOGGERLIB_H__
/**************************************************************************************
 *
 * API_v001rc   -   2010
 *
 * File loggerlib.h , 15.10.2010, created by muma
 *
 **************************************************************************************/

// Qt Includes

// Std Includes

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes
#include "logger.h"

// Defines

/**************************************************************************************/

/**
*  Declares a new Logger for a new function. Needs to stay at the beginning of every function
*/
#define LOG_DECLARE 				logger __lets_log()
/**
*  Logs a Message to the logger.
*  FIXME Loglevel, Classname, methodname , threadname, linenumber should get into this logmessage
*/
#define LOG(loglevel, logtext) 			__lets_log().log(loglevel, logtext)
/**
* Declares a new Logger for a new logger. Needs to get executed on programstart, and do not destroy!
*/
#define LOGLOOP_INIT  				LogLoop __loggerloop()
/**
*  Declares a new filelogger. This should be executed after LOGLOOP_INIT
*/
#define LOGLOOP_DECFILELOG(filename)		__loggerloop().setFileLogger(filename)
/**
*  This Macro will start the loggerloop.
*/
#define LOGLOOP_START				__loggerloop().start()

#endif //__LOGGERLIB_H__
