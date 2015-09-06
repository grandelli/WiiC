#include "accsample.h"

using namespace std;

AccSample::AccSample(float x, float y, float z)
	: Sample() 
{ 
	ax = x; 
	ay = y; 
	az = z; 
	logType = WIIC_LOG_ACC;
}

AccSample::AccSample(const string& line)
	: Sample()
{
	istringstream inLine(line);
	inLine >> relTimestamp >> ax >> ay >> az; 
	logType = WIIC_LOG_ACC;
}

void AccSample::save(ofstream& out)
{
	out << "ACC " << getTimestampFromGestureStart() << " " << ax << " " << ay << " " << az << endl;
}
