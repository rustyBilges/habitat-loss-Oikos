/*
 * Patch.h
 *
 *  Created on: 8 Aug 2014
 *      Author: rusty
 */

#ifndef PATCH_H_
#define PATCH_H_

#include <vector>
using namespace std;

class Patch {

private:
	int m, m_i, m_j;                // m: integer patch ID (1 to M), m_i: patch column ID, m_j: patch row ID

	int landscapeWidth, landscapeHeight;   // integer width and height of the 2D grid (landscape)

	vector<int> plantPresence;      // binary vector. 1->plant is present in this patch. 0->absent
	vector<int> animalPresence;    // binary vector. 1->animal is present in this patch. 0->absent

	vector<int> neighbours;        // vector that stores the IDs of the patches that are adjacent to this one (using Moore's neighbourhood with reflecting boudary conditions)

	vector<double> plantColonisationProbability;    // to store at each iteration the probability p(i,m) of plant i colonising this patch m
	vector<double> animalColonisationProbability;   // to store at each iteration the probability p(j,m) of animal j colonising this patch m

public:
	~Patch(){}
	Patch(int m, int m_i, int m_j, int landscapeWidth, int landscapeHeight, int numberOfPlants, int numberOfAnimals):m(m), m_i(m_i), m_j(m_j), landscapeWidth(landscapeWidth), landscapeHeight(landscapeHeight){

		// initially all species are present in each patch
		plantPresence.resize(numberOfPlants, 1);
		animalPresence.resize(numberOfAnimals, 1);

		// Moore Neighbourhood -> 8 neighbours
		neighbours.resize(8, 0);
		meetTheNeighbours();      // method determines the neighbourhood of this patch

		plantColonisationProbability.resize(numberOfPlants, 0);
		animalColonisationProbability.resize(numberOfAnimals, 0);
	}

	void printPatchInfo(ostream& os){

		int ep = 0;
		int ea = 0;

		for (unsigned int p=0; p<plantPresence.size(); p++){
			if (plantPresence.at(p)==0){ep++;}
		}
		for (unsigned int a=0; a<animalPresence.size(); a++){
			if (animalPresence.at(a)==0){ea++;}
		}

		os << "Patch " << m << ": " << endl;
		os << "Number of plants  extinct =  " << ep  << endl;
		os << "Number of animals extinct =  " << ea  << endl;
		os << "Neighbourhood =  ";
		for (int n=0; n<8; n++){
			os<< neighbours.at(n) << ", ";
		}
		os << endl;
	}

	// to check for presence/absence
	bool isPlantPresent(int i){
		return (plantPresence.at(i)==1);
	}
	bool isAnimalPresent(int j){
		return (animalPresence.at(j)==1);
	}

	// to toggle presence/absence
	void changePlantPresence(int i){
		plantPresence.at(i) = !plantPresence.at(i);
	}
	void changeAnimalPresence(int j){
		animalPresence.at(j) = !animalPresence.at(j);
	}

	bool calculateColonisationProbabilities(vector<vector<double> >& C_plant, vector<vector<double> >& C_animal, vector<Patch>& patches){
		// first calculate plant probabilities:
		for (unsigned int i=0; i<plantPresence.size(); i++){

			if (!plantPresence.at(i)){
				plantColonisationProbability.at(i) = pim(i, C_plant, patches);
			}
			else{
				plantColonisationProbability.at(i) = 0;
			}
		}
		// then calculate animal probabilities:
		for (unsigned int j=0; j<animalPresence.size(); j++){

			if (!animalPresence.at(j)){
				animalColonisationProbability.at(j) = pjm(j, C_animal, patches);
			}
			else{
				animalColonisationProbability.at(j) = 0;
			}
		}
		return true;
	}

	bool performColonisations(MTRand_closed& rng){

		for (unsigned int i=0; i<plantPresence.size(); i++){
			if (rng()<=plantColonisationProbability.at(i)){
				plantPresence.at(i) = 1;                      // successful colonisation!
			}
		}

		for (unsigned int j=0; j<animalPresence.size(); j++){
			if (rng()<=animalColonisationProbability.at(j)){
				animalPresence.at(j) = 1;                      // successful colonisation!
			}
		}


		return true;
	}

private:
	// method to convert x,y grid locations to integer patch ID
	int convertToM(int i, int j){
		return landscapeWidth*j + i;
	}

	void meetTheNeighbours(){

		int neighbourID = 0;

		for (int r=-1; r<2; r++){
			for (int c=-1; c<2; c++){

				int i = m_i + c;    // column number of this neighbour
				int j = m_j + r;    // row number of this neighbour

				if (i==m_i && j==m_j){}
				else {

					// reflective boundary conditions (CORRECT??)
					if (i<0){i=i+2;}
					if(i>=landscapeWidth){i=i-2;}
					if (j<0){j=j+2;}
					if(j>=landscapeHeight){j=j-2;}

//					cout << i << ","  << j << endl;
					neighbours.at(neighbourID) = convertToM(i, j);
					neighbourID++;
				}
			}
		}

	}

	double pim(int i, vector<vector<double> >& C_plant, vector<Patch>& patches){
		double p = 1;

		for (int n=0; n<8; n++){
			int bigJ = 0;
			if (patches.at(neighbours.at(n)).plantPresence.at(i)){           // plant can only colonise if present in neighbouring patch

				for (unsigned int j = 0; j< C_plant.at(i).size(); j++){
					if (patches.at(neighbours.at(n)).animalPresence.at(j)){  // if this plants animal partner is also present..(otherwise p remains unchanged)
						bigJ ++;                                             // increase the number of interacting partners in this neighbouring patch
						p = p * (1 - (C_plant.at(i).at(j)/(double)bigJ));
					}
				}
			}
		}
		return 1 - p;
	}

	double pjm(int j, vector<vector<double> >& C_animal, vector<Patch>& patches){

		double p = 1;

		for (int n=0; n<8; n++){
			int bigI = 0;
			if (patches.at(neighbours.at(n)).animalPresence.at(j)){        // animal j can only colonise from patches it is present in..

				for (unsigned int i = 0; i< C_animal.at(j).size(); i++){
					if (plantPresence.at(i)){                              // plant i must be present in patch to be colonised
						bigI ++;
						p = p * (1 - (C_animal.at(j).at(i)/(double)bigI));
					}
				}
			}
		}
		return 1 - p;

	}
};

class PatchHistory{

private:

public:
	~PatchHistory(){}
	PatchHistory(){}
};

#endif /* PATCH_H_ */
