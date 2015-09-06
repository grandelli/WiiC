#include "dataset.h"
#include "wiic_internal.h"

/** 
 * Dataset destructor, which calls clear.
 */
Dataset::~Dataset()
{
	clear();
}

/** 
 * Add a new training to the dataset.
 * 
 * @param training Add a training to the dataset.
 */
void Dataset::addTraining(Training* t)
{
	if(t)
		trainings.push_back(t);
		
	loaded = true;
}

/**
 * Load a dataset stored in a file.
 *
 * @param filename Dataset filename
 */
bool Dataset::loadDataset(const string& nomefile)
{
	int version; 
	string line, dummy;
	ifstream infile(nomefile.c_str());

	if(!infile.is_open()) {
		cout << "[Error] Unable to open the dataset file" << endl;
		return false;
	}
	else {
		trainings.clear();
		
		// Version
		getline(infile,line); // First line is WiiC log version
		if(line.find("WiiC") == string::npos) {
			cout << "[Error] Bad log format." << endl;
			return false;
		}
		istringstream iline(line);
		iline >> dummy >> version;
		if(version > WIIC_LOG_VERSION) {
			cout << "[Error] Unsupported WiiC log version." << endl;
			return false;
		}
		getline(infile,line); // Date (we should load this as well...)
		
		// For each training
		int counter = 0;
		while(!infile.eof()) {
			getline(infile,line);
			istringstream iline(line);
			string cmd; 
			unsigned long ts;
			iline >> cmd >> ts;
			if(cmd == "START") {
				// Each training is inserted in the training vector	
				trainings.push_back(new Training());
				trainings[counter]->setTimestampFromMidnight(ts);
				if(!(trainings[counter++]->loadTraining(infile))) {
					cout << "[Error] Unable to load a training in the dataset" << endl ;
					return false;
				}
			}
		}
	}
	infile.close();
	loaded = true;
	
	return loaded;
}

/**
 * Delete all trainings and clear the buffer. This method will take 
 * care of freeing the memory of each training in the dataset, 
 * hence you don't need to free them in your code.
 */
void Dataset::clear()
{
	for(unsigned int i = 0 ; i < trainings.size() ; i++) {
		if(trainings[i]) {
			delete trainings[i];
			trainings[i] = 0;
		}
	}
	trainings.clear();
	loaded = false;
}

/**
 * Save the dataset into a file for training and recognition.
 *
 * @param filename Pathname of the destination file
 * @param addr Wii device mac address
 */
bool Dataset::save(const char* file, const char* addr) const
{
	// Open file
	ofstream out(file, ios::trunc);
	if(!out.is_open()) { // File does not exist, hence I create it
		cout << "[Error] Unable to open " << file << endl;
		return false;
	}
	
	// Save header
	saveHeader(out, addr);
	
	// For each training
	for(int i = 0 ; i < size() ; i++)	{
		const Training* training = trainingAt(i);
		if(training)
			training->save(out);
	}
	
	out.close();

	return true;
}

/**
 * Save the dataset header into a file.
 *
 * @param out Output file stream
 * @param addr Wii device MAC address
 */	
void Dataset::saveHeader(ofstream& out, const char* addr) const
{
	// Log Version
	out << "WiiC " << WIIC_LOG_VERSION << endl;

	// Date
    time_t tim=time(NULL);
    char *s=ctime(&tim);
    s[strlen(s)-1]=0;        // remove \n
	out << s << endl;
	
	// Mac Address
	out << addr << endl;
}


