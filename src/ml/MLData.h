#ifndef _MLDATA_H_
#define _MLDATA_H_

#ifdef CV_OLD // Support for old OpenCV versions (less than 2.2)
	#include <cv.h>
	#include <ml.h>
#else
	#include "opencv2/highgui/highgui.hpp"
	#include <opencv2/core/core.hpp>
	#include <opencv2/ml/ml.hpp>
#endif
#include <vector>
#include <string>
#include <dataset.h>

#define GRAV_ACC	9.81
#define	TIME_DELTA	0.01

using namespace std;

class MLData
{
public:
       MLData(const vector<int>& mask);
       ~MLData();

       	bool open(const vector<string>& vf); // Open files stored vf (one for each category)
		bool save(string savefile, bool add_flag =false); // Save data in a file
		bool loadTraining(const Training* t); // Extracts features from WiiC data (one Training per gesture) 
		bool loadTraining(const Training* t, float timeDelta); // Extracts features from WiiC data (one Training per gesture) with a delta time
      	void generateTrainingAndValidationData(float perc);  // Generate training and testing set according to the desired percentage
		void clean();
		
       	int getNumFeatures(); // nr. of features
       	int getNumCategories(); // nr. of categories
       	int getNumSamples(); // nr. of samples
       	string categoryName(int i); // name of k-th category
       	CvMat * getTrainingInputData(); // training input data (do not delete the output matrix!!!)
       	CvMat * getTrainingOutputData(); // training output data
       	CvMat * getValidationInputData(); // validation input data
       	CvMat * getValidationOutputData(); // validation output data
		inline CvMat * getInputData() {return all_in;}; // input data
		inline CvMat * getOutputData() {return all_out;} // output data
       
       	void getRandomSample(CvMat *sample, float &cat);
		void normalize(const int minN =0, const int maxN =1); // This will replace old data

protected:
		void computeDisplacement(const Training* t, vector<double>& features);
		void computeAttitude(const Training* t, vector<double>& features);
		void computeSpeed(const Training* t, vector<double>& features);

private:
      	CvMat *all_in, *train_in, *validation_in;
      	CvMat *all_out, *train_out, *validation_out;
      	vector<string> categoryNames;
		vector<int> featureMask;
		int ncategories,nfeatures,nsamples;
		float min, max;
		float timeDelta_;
		CvRNG rng;
};

#endif

