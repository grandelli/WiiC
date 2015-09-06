#ifndef TRAINING_H
#define TRAINING_H

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "sample.h"
#include "accsample.h"
#include "gyrosample.h"

using namespace std;

/**
 * Class Training to save the individual training of a gesture  
 */
class Training
{
public:
	/**
	 * Default constructor.
	 */
	Training() { }
	~Training();

	bool loadTraining(ifstream&);
	void save(ofstream&) const;
	void addSample(Sample*);
	void clear();

	/** 
	 * Returns the i-th sample of the training as a constant pointer. 
	 * 
	 * @param i Sample index in the training set
	 */
	inline const Sample* sampleAt(unsigned int i) const { 
		if(i < samples.size())
			return samples[i]; 
		else {
			cout << "[Error]: requested out of array bound index in training." << endl;
			return 0;
		}
	}

	/** 
	 * Returns the i-th sample of the training as a pointer. 
	 * 
	 * @param i Sample index in the training set
	 */
	inline Sample* sampleAt(unsigned int i) { 
		if(i < samples.size())
			return samples[i]; 
		else {
			cout << "[Error]: requested out of array bound index in training." << endl;
			return 0;
		}
	}
	
	/** 
	 * Number of samples in the current training. 
	 */
	inline unsigned int size() const { return samples.size(); }
	
	inline void setTimestampFromMidnight(unsigned long ts) { timestamp = ts; }

private:
	vector<Sample*> samples;
	unsigned long timestamp; // Training timestamp (beginning of the gesture)
};

#endif
