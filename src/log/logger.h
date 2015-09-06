#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <dataset.h>

namespace WiiC {
	/*
	 * LOG_START: open a dataset
	 * LOG_STOP: close the dataset
	 */
	enum LogStatus {
		WIIC_LOG_START = 0x0,
		WIIC_LOG_STOP = 0x1
	};
}

using namespace std;
using namespace WiiC;

class Logger {
public:
	Logger() : deviceAddr(""), logFile(""), relativeTs(0.0), logEnabled(false), 
		logType(WIIC_LOG_NONE), logStatus(WIIC_LOG_STOP) { }
	Logger(const Logger& logger) : deviceAddr(logger.deviceAddr), logFile(logger.logFile), 
			startTs(logger.startTs), relativeTs(logger.relativeTs), logEnabled(logger.logEnabled), 
			logType(logger.logType), logStatus(logger.logStatus) { 
		if(out.is_open()) out.close();
		if(logger.out.is_open()) out.open(logFile.c_str(),ios::app); 
	}
	
	~Logger() { 
		if(out.is_open()) out.close(); 
	}
	
	Logger& operator=(const Logger& logger) {
		deviceAddr = logger.deviceAddr;
		logFile = logger.logFile;
		logType = logger.logType;
		logStatus = logger.logStatus;
		logEnabled = logger.logEnabled;
		startTs = logger.startTs;
		relativeTs = logger.relativeTs;
		if(out.is_open()) out.close();
		if(logger.out.is_open()) out.open(logFile.c_str(),ios::app); 
		
		return *this;
	}
		
	void InitLog();
	void LogAcc(float x, float y, float z);
	void LogGyro(float x, float y, float z);
	void SaveLogHeader();

	void SetLogLevel(LogStatus status, int type =WIIC_LOG_NONE, const string& file ="");	
	inline int GetLogType() const { return logType; }
	inline string GetLogFilename() const { return logFile; }
	inline void SetLogStatus(const LogStatus status) { logStatus = status; }
	inline bool isLogEnabled() { return logEnabled; }
	
	inline void ResetTimestamp() { gettimeofday(&startTs,0); }
	inline void SetDeviceAddress(const string& addr) { deviceAddr = addr;}

protected:
	bool CheckLogFile();
	
private:
	string deviceAddr;
	
	// Log file info
	string logFile;
	ofstream out;
	
	// Timestamp info
	struct timeval startTs;
	double relativeTs;
	
	// Logging info
	bool logEnabled;
	int logType;
	LogStatus logStatus;
};

#endif
