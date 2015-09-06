#include "logger.h"
#include "wiicpp.h"
#include "wiic_internal.h"

void Logger::SetLogLevel(LogStatus status, int type, const string& file)
{		
	// Avoid repetitions
	if(status == logStatus && type == logType && file == logFile)
		return;
		
	if(status == WIIC_LOG_START) {
		ResetTimestamp();	
	
		if((file == "" && logFile != "") || (file == logFile)) { // Same file
			if(logEnabled) 
				out << "END" << endl;
				
			// New training
			unsigned long trainingTs = (startTs.tv_sec % 86400) * 1000 + startTs.tv_usec / 1000;
			out << "START " << trainingTs << endl;
		}
		else if(file != "") { // Open a new log file
			if(out.is_open()) { // In case a previous log file is still open
				if(logEnabled)
					out << "END" << endl;
				out.close();
			}
						
			logFile = file;
			
			// Check if the file already exists, and whether it is a WiiC logfile
			bool resume = CheckLogFile();
			
			out.open(logFile.c_str(),ios::app);
			if(!out.is_open()) 
				return;
		
			// Sometimes we want to modify a log file, not to create a new one
			if(!resume)
				SaveLogHeader();

			// New training				
			unsigned long trainingTs = (startTs.tv_sec % 86400) * 1000 + startTs.tv_usec / 1000;
			out << "START " << trainingTs << endl;
		}
		else // How can I log if you don't provide me with a file???
			return;
				
		// Status init
		logType = type;
		logStatus = WIIC_LOG_START;
		logEnabled = true;
	}
	else if(status == WIIC_LOG_STOP) {
		out << "END" << endl;
		if(out.is_open())
			out.close();
			
		// Status reset
		logStatus = WIIC_LOG_STOP;
		logType = WIIC_LOG_NONE;
		logEnabled = false;
		logFile = "";
	}
	else {
		return;
	}	
}

bool Logger::CheckLogFile()
{
	ifstream in(logFile.c_str());
	if(!in.is_open())
		return false;
		
	string wiicToken;
	int logVersion;
	in >> wiicToken >> logVersion;
	in.close();
	
	if(wiicToken == "WiiC" && logVersion <= WIIC_LOG_VERSION)
		return true;
		
	return false;
}

/**
 * Save the dataset header into a file.
 *
 * @param out Output file stream
 * @param addr Wii device MAC address
 */	
void Logger::SaveLogHeader()
{
	// Log Version
	out << "WiiC " << WIIC_LOG_VERSION << endl;

	// Date
    time_t tim=time(NULL);
    char *s=ctime(&tim);
    s[strlen(s)-1]=0;        // remove \n
	out << s << endl;

	// Mac Address
	out << deviceAddr << endl;
}

void Logger::InitLog()
{
	// Compute the relative timestamp
	struct timeval t;
	gettimeofday(&t,0);
	relativeTs = ((t.tv_sec*1000000.0 + t.tv_usec) - (startTs.tv_sec*1000000.0 + startTs.tv_usec))/1000.0;
}

void Logger::LogAcc(float x, float y, float z)
{
	out << "ACC " << relativeTs << " " << x << " " << y << " " << z << endl;	
}

void Logger::LogGyro(float roll, float pitch, float yaw)
{
	out << "GYRO " << relativeTs << " " << roll << " " << pitch << " " << yaw << endl;
}
