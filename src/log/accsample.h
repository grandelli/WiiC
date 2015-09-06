#ifndef ACC_SAMPLE_H
#define ACC_SAMPLE_H

#include "sample.h"

class AccSample : public Sample
{
public:
	AccSample(float x, float y, float z);
	AccSample(const string& line);
	~AccSample() {}
	inline float x() const { return ax;	}
	inline float y() const { return ay;	}
	inline float z() const { return az;	}
	
	virtual void save(ofstream& out);

private:
	float ax;
	float ay;
	float az;
};

#endif

