#include "training.h"

/** 
 * Dataset destructor, which calls clear.
 */
Training::~Training()
{
	clear();
}

/**
 * Load a training stored in a file.
 *
 * @param training File stream of the contained training
 */
bool Training::loadTraining(ifstream& training)
{
	string sample, cmd;
	
	// Get a sample	
	getline(training, sample);
	
	// Parse every sample to get the log type
	istringstream sStr(sample);
	sStr >> cmd;
	
	while(!training.eof() && cmd != "END" && cmd != "") {	
		if(cmd == "ACC") 
			addSample(new AccSample(sStr.str()));
		else if(cmd == "GYRO") 
			addSample(new GyroSample(sStr.str()));
		else {
			cout << "[Error] Bad log type." << endl;	
			return false;	
		}
		
		getline(training, sample);
		sStr.str(sample);
		sStr >> cmd;
	}		
	
	return true;
}

/**
 * Save the training into a file for training and recognition.
 *
 * @param out Stream of the destination file
 */
void Training::save(ofstream& out) const
{	
	// Training Header
	out << "START " << timestamp << endl;
	
	// Samples
	for(unsigned int i = 0 ; i < samples.size() ; i++)
		samples[i]->save(out);
		
	out << "END" << endl;
}

/** 
 * Add a new training to the dataset.
 * 
 * @param sample Add a sample to the training set.
 */
void Training::addSample(Sample* sample)
{
	// We retrieve the overall gesture timestamp
	struct timeval t;
	gettimeofday(&t,0);
	unsigned long sampleTs = (t.tv_sec % 86400) * 1000 + t.tv_usec / 1000;
		
	// We compute the relative timestamp in msec
	unsigned long deltaT = sampleTs - timestamp;
	
	if(sample) {
		sample->setTimestampFromGestureStart(deltaT);
		samples.push_back(sample);
	}
}

/**
 * Delete all samples and clear the buffer. This method will take 
 * care of freeing the memory of each sample in the training set, 
 * hence you don't need to free them in your code.
 */
void Training::clear()
{
	for(unsigned int i = 0 ; i < samples.size() ; i++) {
		if(samples[i]) {
			delete samples[i];
			samples[i] = 0;
		}
	}
	samples.clear();
}
