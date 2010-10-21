#ifndef __LOGGER_H__
#define __LOGGER_H__
/**************************************************************************************
 *
 * api_v001rc   -   2010
 *
 * File logger.h , 15.10.2010, created by muma
 *
 **************************************************************************************/

// Qt Includes
#include <QString>
#include <QThread>
#include <QFile>
// Std Includes

// Linux Includes

// Windows Includes

// Library Includes
#include "msgQ.h"
// Application Includes

// Defines

/**************************************************************************************/

class Logger
    {

    public:
    Logger();

    ~Logger();

    void log_base(QString text);
    private:
    };

class LogLoop : public QThread
    {

    public:
	LogLoop();
	~LogLoop();
	void setFileLoggerOn(QString Filename);
	void run();
    private:
	void mainloop();

	QString internFilename;
	QString logText;
	bool fileLogger;
	bool consoleLogger;
    };


//FIXME not used!
class LogCluster
    {
    public:
    LogCluster();
    ~LogCluster();

    //single instance global!
    static LogCluster *get();
    int getLoglevel();

    private:

    int m_Loglevel;
    };

#endif //__LOGGER_H__
