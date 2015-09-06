/** \file dataset.h
 * Header file of the class Dataset.
 */
#ifndef DATASET_H
#define DATASET_H

#include "training.h"

/** 
 * \class Dataset 
 *
 * A Dataset is a collection of trainings, which can represent multiple gestures 
 * acquired over time, with different lenghts, and with different approaches.
 */
class Dataset
{
public:
	/** 
	 * Default constructor.
	 */
	Dataset() : loaded(false) { }
	
	/** 
	 * Creates a Dataset object from a log file.
	 * 
	 * @param filename Pathname of the dataset to load
	 */
	Dataset(const string& filename) : loaded(false) { loadDataset(filename); }
	~Dataset();

	/** 
	 * Retrieves the dataset's size. 
	 *
	 * \return the number of trainings collected in the current dataset
	 */
	inline unsigned int size() const { return trainings.size(); }

	/** 
	 * Returns a training in the dataset as a constant pointer. 
	 * 
	 * @param[in] i The training index in the dataset
	 *
	 * \return A constant pointer to the Training object corresponding to the i-th element in the dataset
	 */
	inline const Training* trainingAt(const unsigned int i) const { 
		if(i < trainings.size())
			return trainings[i]; 
		else {
			cout << "[Error]: requested out of array bound index in dataset." << endl;
			return 0;
		}
	}

	/** 
	 * Returns a training in the dataset as a pointer. 
	 * 
	 * @param[in] i The training index in the dataset
	 *
	 * \return A pointer to the Training object corresponding to the i-th element in the dataset
	 */
	inline Training* trainingAt(const unsigned int i) { 
		if(i < trainings.size())
			return trainings[i]; 
		else {
			cout << "[Error]: requested out of array bound index in dataset." << endl;
			return 0;
		}
	}

	/** 
	 * Adds a training to the dataset. The new element will be indexed as the last element and
	 * will not be copied. It will be responsability of the destructor of the class 
	 * Dataset to deallocate this element.
	 * 
	 * @param[in] training The training to add
	 */
	void addTraining(Training* training);

	/** 
	 * Loads a new dataset from file, erasing the current one (if not saved on file). 
	 * 
	 * @param[in] filename Filename of the dataset to load
	 */
	bool loadDataset(const string& filename);
	
	/** 
	 * Saves the current dataset in a file. 
	 * 
	 * @param[in] filename Desired filename of the saved dataset
	 * @param[in] addr MAC address of the source device for the dataset
	 */
	bool save(const char* filename, const char* addr) const;
	
	/** 
	 * Deallocates and deletes all the current trainings in the dataset.
	 */
	void clear() ;

	/**
	 * Checks if the instance has a valid dataset loaded.
	 *
	 * \return TRUE if a dataset is correctly loaded, FALSE otherwise
	 */
	inline bool isValid() const { return loaded; }

protected:
	/**
	 * Saves the initial header of WiiC's log files.
	 *
	 * @param[in] out The output stream of the log file
	 * @param[in] addr MAC address of the source device for the dataset
	 */
	void saveHeader(ofstream& out, const char* addr) const;

private:
	vector<Training*> trainings;
	bool loaded;
};

#endif
