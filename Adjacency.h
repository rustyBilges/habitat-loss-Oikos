/*
 * Adjacency.h
 *
 *  Created on: 8 Aug 2014
 *      Author: rusty
 *
 *      Adjacency is strictly binary (presence/absence)
 */

#ifndef ADJACENCY_H_
#define ADJACENCY_H_

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Adjacency{

private:
	int numberOfPlants, numberOfAnimals;
	vector<bool> connections;
public:
	~Adjacency(){}
	Adjacency(int numberOfPlants, int numberOfAnimals):numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals){
		connections.resize(numberOfAnimals*numberOfPlants, 0);
	}

	bool readFromFile(string fname){
		ifstream ifile;
		ifile.open(fname.c_str());
		if(!ifile){
			cerr << "Could not open file containing empirical network" << endl;
			return false;
		}

		int count = 0;
		string line;
		while (getline(ifile, line, '\n')){
			istringstream iss(line);

			string s;
			while (getline(iss, s, ',')){

				if (s!="\n"){
					stringstream ss(s);
					int n;
					ss >> n;
					if (n!=0){connections.at(count) = 1;}
					count++;
				}
			}
		}

		ifile.close();
		return true;
	}

	void print(ostream& os){
		int plantCount = 0;
		for (unsigned int i=0; i<connections.size(); i++){

			os << connections.at(i) << ", ";
			plantCount++;
			if (plantCount==numberOfPlants){
				cout << endl;
				plantCount=0;
			}

		}
	}

	// is plant i connected to animal j (are they partners)?
	bool connected(int i, int j){

		return connections.at(numberOfPlants*j + i);
//		return true;
	}


};


#endif /* ADJACENCY_H_ */
