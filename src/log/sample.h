#ifndef SAMPLE_H
#define SAMPLE_H

#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define WIIC_LOG_NONE	0x0
#define WIIC_LOG_ACC	0x1
#define WIIC_LOG_GYRO	0x2

using namespace std;

class Sample
{
public:
	Sample() : relTimestamp(0) { }
	~Sample() {}
	virtual void save(ofstream& out) =0;
	
	inline void setLogType(int l) { logType = l; }
	inline int getLogType() const { return logType; }
	inline void setTimestampFromGestureStart(unsigned long t) { relTimestamp = t; }
	inline unsigned long getTimestampFromGestureStart() const { return relTimestamp; }
	
protected:	

	unsigned long relTimestamp; // msec (from the beginning of the gesture)
	int logType;
};

#endif

