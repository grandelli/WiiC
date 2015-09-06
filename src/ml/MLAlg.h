#include "MLData.h"
#include <string>

static string MLAlgName[] = { "KNN", "Bayes", "SVM", "DT", "ANN", "Boost", "RT", "None" };

using namespace std;

class MLAlg
{
public:
    enum type {KNN,Bayes,SVM,DT,ANN,Boost,RT,NONE,NUMTYPES};
	MLAlg(type t=KNN);
  	inline void setType(type t) { tp = t; }
	void setType(const string& st);
	inline string getType() { return MLAlgName[tp]; }
	inline void setCategoryNames(const vector<string>& names) { categoryNames = names; }
	string categoryName(int i);
	void train(const CvMat *trainIn, const CvMat *trainOut);
	void validate(const CvMat *validateIn, const CvMat *validateOut);
	void recognize(const CvMat *recognizeIn, CvMat *recognizeOut);
	void save(const char* filename) const;
	void load(const char* filename);

private:  
    CvKNearest knn;
    CvNormalBayesClassifier bayes;
    CvSVM svm;
    CvSVMParams svmparams;
	CvDTree dt;
	CvDTreeParams dtparams;
	CvRTrees rt;
	CvRTParams rtparams;
	CvBoost boost;
	CvBoostParams boostparams;
	CvANN_MLP ann;
	CvANN_MLP_TrainParams annparams;
    
    int K;
    type tp;
	vector<string> categoryNames;
};
