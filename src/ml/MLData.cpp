#include "MLData.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

MLData::MLData(const vector<int>& mask) : rng(time(0)) {
    all_in = all_out = NULL;
    train_in = train_out = NULL;
    validation_in = validation_out = NULL;

    nfeatures=0;
    ncategories=0;
    nsamples=0;
    
    timeDelta_ = TIME_DELTA;

	min = std::numeric_limits<float>::max();
	max = std::numeric_limits<float>::min();
	
	if(!mask.size())
		cout << "Feature Mask is empty!" << endl;
	featureMask = mask;
}

MLData::~MLData() 
{      
    if (all_in) cvReleaseMat(&all_in);
    if (all_out) cvReleaseMat(&all_out);
    if (train_in) cvReleaseMat(&train_in);
    if (train_out) cvReleaseMat(&train_out);
    if (validation_in) cvReleaseMat(&validation_in);
    if (validation_out) cvReleaseMat(&validation_out);
}

void MLData::clean()
{
    if (all_in) cvReleaseMat(&all_in);
    if (all_out) cvReleaseMat(&all_out);
    if (train_in) cvReleaseMat(&train_in);
    if (train_out) cvReleaseMat(&train_out);
    if (validation_in) cvReleaseMat(&validation_in);
    if (validation_out) cvReleaseMat(&validation_out);	

    all_in = all_out = NULL;
    train_in = train_out = NULL;
    validation_in = validation_out = NULL;
}

// apre i file memorizzati in vf (uno per ciascuna categoria e li carica in memoria)
bool MLData::open(const vector<string>& vf) 
{ 
    ncategories = vf.size();
    cout << "Number of categories = " << ncategories << endl;
    int isample = 0; // current sample

    for (int i = 0 ; i < ncategories ; i++)	{
		cout << "Processing file " << vf[i] << endl;
		// extract name of category from filename
		const char *p = strrchr (vf[i].c_str(), '/' );
		string name;
		if (p==NULL) name = vf[i];
		else name = string(p+1);
		categoryNames.push_back(name);
		cout << "Category: " << name << endl;
	
		// read the file
		ifstream ff(vf[i].c_str()); 
		if (!ff.is_open()) { cout << "Cannot open " << vf[i] << endl; return false; }

		const int bufsize=1024;
		char buf[bufsize];
	
		// read nr. of samples
		ff.getline (buf, bufsize); 
		strcat(buf," "); // just in case
		istringstream ss(buf);
		int ns;
		ss >> ns; nsamples += ns; 
		cout << "Number of samples = " << ns << endl;
	
		// read nr. of features and data
		nfeatures=0;
		while (ff.good()) {
	    	ff.getline (buf, bufsize); // if (nfeatures==0) cout << " ** buf: [-- " << buf << " --]" << endl;
	    	if (strlen(buf)<2) continue; // ignore empty lines	    
	    	strcat(buf," ");  // needed because sometimes it gets eof before reading the number!
	    
			istringstream ss(buf);
			float a[100]; int k=0; int j=0;
			if (nfeatures==0) {
				float val;
				ss >> val;
				while (ss.good()) {
					if(find(featureMask.begin(), featureMask.end(), (k+1)) != featureMask.end()) { // Feature selection
						a[nfeatures++]=val; 
					}
					ss >> val;
					k++;
				}
				cout << "Number of features = " << nfeatures << endl;
			}
	    	else {
				while (ss.good()) {
					float val;
					ss >> val;
					if(find(featureMask.begin(), featureMask.end(), (k+1)) != featureMask.end()) // Feature selection
						a[j++] = val;
					k++;
				}
	    	}
	    	
			if (!all_in) {
				all_in = cvCreateMat( nsamples*ncategories, nfeatures, CV_32FC1 );
				all_out = cvCreateMat( nsamples*ncategories, 1, CV_32FC1 );
			}
	   
			for (k=0; k<nfeatures; k++) {
				all_in->data.fl[isample*nfeatures+k] = a[k];
				if(a[k] < min)
					min = a[k];
				else if(a[k] > max)
					max = a[k];
			}
	    	all_out->data.fl[isample] = i; // value of this category
	    	isample++;
		}
	
		ff.close();
    } // while categories

    cout << "Total number of samples = " << nsamples << endl;

	// Feature Bitmask (if already set, do nothing, otherwise set all the features)
	if(!featureMask.size()) {
		for(int i = 1 ; i <= nfeatures ; i++) // BE CAREFUL: feature index starts from 1, vector index starts from 0!
			featureMask.push_back(i);
	}

    return true; 
}

bool MLData::save(string savefile, bool add_flag)
{
	ofstream out;
	
	if(add_flag)
		out.open(savefile.c_str(),ios::app);
	else
		out.open(savefile.c_str(),ios::trunc);
		
	if(!out.is_open()) {
		cout << "Unable to save " << savefile << endl;
		return false;		
	}
	
	for(int i = 0 ; i < nsamples ; i++) {
		for(int j = 0 ; j < nfeatures ; j++)
			out << all_in->data.fl[i*nfeatures+j] << " ";
		out << endl;
	}
	out.close();
}

bool MLData::loadTraining(const Training* t)
{
	return loadTraining(t,TIME_DELTA);
}

bool MLData::loadTraining(const Training* t, float timeDelta)
{
	vector<double> features;
	timeDelta_ = timeDelta;
	
	// Extract features
	computeDisplacement(t,features);
	computeAttitude(t,features);
	computeSpeed(t,features);
	
	// Parameter init
	nsamples = 1;
	ncategories = 1;
	nfeatures = 8;
	
	// Store in OpenCV data structure
	clean();
	all_in = cvCreateMat( nsamples*ncategories, nfeatures, CV_32FC1 );
	all_out = cvCreateMat( nsamples*ncategories, 1, CV_32FC1 );
	
	// FIXME - qui manca il controllo della feature mask

	for (int k = 0; k < nfeatures ; k++) {
		all_in->data.fl[k] = features[k];
		if(features[k] < min)
			min = features[k];
		else if(features[k] > max)
			max = features[k];
	}
}

void MLData::computeDisplacement(const Training* t, vector<double>& features)
{
	double xspace, yspace, zspace;
	double xvel, yvel, zvel;
	xspace = yspace = zspace = xvel = yvel = zvel = 0.0;
	
	// Compute displacement and update speed
	for(int i = 0 ; i < t->size() ; i++) {
		const AccSample* sample = reinterpret_cast<const AccSample*>(t->sampleAt(i));
		xspace = xspace + xvel*timeDelta_ + 0.5*sample->x()*timeDelta_*timeDelta_*GRAV_ACC; 
		xvel = xvel + sample->x()*timeDelta_*GRAV_ACC;
 		yspace = yspace + yvel*timeDelta_ + 0.5*sample->y()*timeDelta_*timeDelta_*GRAV_ACC; 
 		yvel = yvel + sample->y()*timeDelta_*GRAV_ACC;
 		zspace = zspace + zvel*timeDelta_ + 0.5*(sample->z()-1)*timeDelta_*timeDelta_*GRAV_ACC; 
		zvel = zvel + (sample->z()-1)*timeDelta_*GRAV_ACC;	
	}
	
	features.push_back(xspace);
	features.push_back(yspace);
	features.push_back(zspace);
}

void MLData::computeAttitude(const Training* t, vector<double>& features)
{
	double roll, pitch;
	roll = pitch = 0.0;
	
	int i = t->size() - 1;
	const AccSample* sample = reinterpret_cast<const AccSample*>(t->sampleAt(i));
	roll = -atan2(sample->x(),sample->z());
	pitch = asin(sample->y());
	if(isnan(pitch))
		pitch = 0.0;
	
	features.push_back(roll);
	features.push_back(pitch);
}

void MLData::computeSpeed(const Training* t, vector<double>& features)
{
	double xvel, yvel, zvel;
	xvel = yvel = zvel = 0.0;
	
	// Compute speed
	for(int i = 0 ; i < t->size() ; i++) {
		const AccSample* sample = reinterpret_cast<const AccSample*>(t->sampleAt(i));
		xvel = xvel + sample->x()*timeDelta_*GRAV_ACC;
 		yvel = yvel + sample->y()*timeDelta_*GRAV_ACC;
		zvel = zvel + (sample->z()-1)*timeDelta_*GRAV_ACC;	
	}	
	
	xvel = xvel/t->size();
	yvel = yvel/t->size();
	zvel = zvel/t->size();
	
	features.push_back(xvel);
	features.push_back(yvel);
	features.push_back(zvel);
}

void MLData::generateTrainingAndValidationData(float perc)  // genera random perc % training data (in train) e il resto come validation
{ 
    if (train_in) cvReleaseMat(&train_in);
    if (train_out) cvReleaseMat(&train_out);
    if (validation_in) cvReleaseMat(&validation_in);
    if (validation_out) cvReleaseMat(&validation_out);

    train_in = train_out = NULL;
    validation_in = validation_out = NULL;

    if (fabs(perc-1.0f)<1e-9) {
	// copy alldata in train data (no validation set);
	train_in = cvCloneMat(all_in);
	train_out = cvCloneMat(all_out);
    }
    else {
	int ts = (int)round(perc*nsamples), vs = nsamples-ts;
	cout << "Training samples = " << ts << ", Validation samples = " << vs << endl;
	
	// alloc mem
	if(ts != 0) {
		train_in = cvCreateMat( ts, nfeatures, CV_32FC1 );
		train_out = cvCreateMat( ts, 1, CV_32FC1 );
	}
	
	if(vs != 0) {
		validation_in = cvCreateMat( vs, nfeatures, CV_32FC1 );
		validation_out = cvCreateMat( vs, 1, CV_32FC1 );
	}
	
	vector<bool> forvalidation; forvalidation.reserve(nsamples);
	for (int k=0; k<nsamples; k++) forvalidation[k]=false;
	// random selection of validation samples
	int cvs=0; // current validation samples selected
	while (cvs < vs) {
	    int r = nsamples*cvRandReal(&rng);
	    if (!forvalidation[r]) {
		forvalidation[r]=true;
		for (int k=0; k<nfeatures; k++) {
		    validation_in->data.fl[cvs*nfeatures+k] = all_in->data.fl[r*nfeatures+k];
		}
		validation_out->data.fl[cvs] =  all_out->data.fl[r];
		cvs++;
	    }
	}
	int cts=0; // current validation samples
	for (int r=0; r<nsamples; r++) {
	    if (!forvalidation[r]) {
		for (int k=0; k<nfeatures; k++) {
		    train_in->data.fl[cts*nfeatures+k] = all_in->data.fl[r*nfeatures+k];
		}
		train_out->data.fl[cts] =  all_out->data.fl[r];
		cts++;
	    }
	}
	
    }
}

void MLData::getRandomSample(CvMat *sample, float &cat)
{
    int r = nsamples*cvRandReal(&rng);
    for (int k=0; k<nfeatures; k++) {
		sample->data.fl[k] = all_in->data.fl[r*nfeatures+k];
    }
    cat = all_out->data.fl[r];
}

void MLData::normalize(const int minN, const int maxN)
{
	for(int i = 0 ; i < nsamples*nfeatures ; i++) {
		float val = (all_in->data.fl[i] - min)/(max - min)*(maxN - minN) + minN;
		all_in->data.fl[i] = val;
	}
		
	min = minN;
	max = maxN;
}


int MLData::getNumFeatures() // nr. of features
{ return nfeatures; }

int MLData::getNumCategories() // nr. of categories
{ return ncategories; }

int MLData::getNumSamples() // nr. of samples
{ return nsamples; }

string MLData::categoryName(int i) // name of k-th category
{ 
	if(i < categoryNames.size())
		return categoryNames[i];
	else {
		ostringstream oss;
		oss << i;
		return oss.str();
	}
}

CvMat * MLData::getTrainingInputData() // training input data
{ return train_in; }

CvMat * MLData::getTrainingOutputData() // training output data
{ return train_out; }

CvMat * MLData::getValidationInputData() // validation input data
{ return validation_in; }

CvMat * MLData::getValidationOutputData() // validation output data
{ return validation_out; }



