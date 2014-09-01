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

public:
	~Patch(){}
	Patch(int m, int m_i, int m_j, int landscapeWidth, int landscapeHeight, int numberOfPlants, int numberOfAnimals):m(m), m_i(m_i), m_j(m_j), landscapeWidth(landscapeWidth), landscapeHeight(landscapeHeight){

		// initially all species are present in each patch
		plantPresence.resize(numberOfPlants, 1);
		animalPresence.resize(numberOfAnimals, 1);

		// Moore Neighbourhood -> 8 neighbours
		neighbours.resize(8, 0);
		meetTheNeighbours();      // method determines the neighbourhood of this patch
	}

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
					if(i>99){i=i-2;}
					if (j<0){j=j+2;}
					if(j>99){j=j-2;}

					cout << i << ","  << j << endl;
					neighbours.at(neighbourID) = convertToM(i, j);
					neighbourID++;
				}
			}
		}

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
		os << "Number of animals extinct =  " << ep  << endl;
		for (int n=0; n<8; n++){
			os<< neighbours.at(n) << ", ";
		}
		os << endl;
	}

};

class PatchHistory{

private:

public:
	~PatchHistory(){}
	PatchHistory(){}
};

#endif /* PATCH_H_ */
