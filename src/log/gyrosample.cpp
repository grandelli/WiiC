#include "gyrosample.h"

using namespace std;

GyroSample::GyroSample(float r, float p, float y) 
	: Sample()
{ 
	roll_ = r; 
	pitch_ = p; 
	yaw_ = y;
	logType = WIIC_LOG_GYRO; 
}

GyroSample::GyroSample(const string& line)
	: Sample()
{
	istringstream inLine(line);
	inLine >> relTimestamp >> roll_ >> pitch_ >> yaw_; 
	logType = WIIC_LOG_GYRO; 
}

void GyroSample::save(ofstream& out)
{
	out << "GYRO " << getTimestampFromGestureStart() << " " << roll_ << " " << pitch_ << " " << yaw_ << endl;
}
