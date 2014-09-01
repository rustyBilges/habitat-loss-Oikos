/*
 * main.cpp
 *
 *  Created on: 8 Aug 2014
 *      Author: rusty
 *
 *      Implementation of the cellular automata model from paper: "Habitat loss and the disassembly of mutalistic networks"
 *
 *      To Do: REFACTOR TO CREATE A SIMULATION CLASS! (THEN CAN FARM OUT SIMULATIONS USING OPENMP?)
 *
 *      For the grid (landscape), the index x refers to column i.e. x~[0,width]
 *                                          y refers to row    i.e. y~[0,height]
 *
 *      In the adjacency representation plants are columns (indexed by i) and animals are rows (indexed by j).
 *      This is in keeping with the notation used in the paper, and the network representations found in the literature.
 */

#include <iostream>
#include <vector>

#include "Simulation.h"
#include "Patch.h"
#include "Adjacency.h"
#include "mtrand.h"
using namespace std;

bool createPatches(vector<Patch>& patches, int nPlants, int nAnimals);
bool drawExtinctionProbabilities(int numberOfPlants, int numberOfAnimals, vector<double>& E_p, vector<double>& E_a);
bool drawColonisationProbabilities(int numberOfPlants, int numberOfAnimals, Adjacency& masterNetwork, 	vector<vector<double> >& C_plant, 	vector<vector<double> >& C_animal);

int main(){

	MTRand_closed rng(0);

	int numberOfPlants = 10;
	int numberOfAnimals = 12;

	// the master network defines all possible connections, it is taken form the literature (empirical network):
	Adjacency masterNetwork(numberOfPlants, numberOfAnimals);
	masterNetwork.readFromFile("empiricalNetwork_Oelsen2002_16.text");
//	masterNetwork.print(cout);

	Simulation testSimulation(rng, numberOfPlants, numberOfAnimals, 100, 100, masterNetwork);

//	if(masterNetwork.connected(0,10)){
//		cout << endl << endl << "yes!" << endl;
//	}

	cout << "set up successful" << endl;

	//////////////////////////////////////////////////////////////////////////////
//	int iterations = 100;
//	MTRand_closed rng(0);
//
//	for (int t=0; t<iterations; t++){
//
//	}

	return 0;
}

bool createPatches(vector<Patch>& patches, int nPlants, int nAnimals){

	if (patches.size()!=0){
		cerr << "cannot initialise patches, landscape already exists?" << endl;
		return false;
	}

	int width = 100;
	int height = 100;
	int patchID = 0;
	for (int y=0;y<height; y++){
		for (int x=0; x< width; x++){

			patches.push_back(Patch(patchID, x, y, width, height, nPlants, nAnimals));
			patches.at(patchID).printPatchInfo(cout);
			patchID++;
		}
	}

	return true;
}

bool drawExtinctionProbabilities(int numberOfPlants, int numberOfAnimals, vector<double>& E_p, vector<double>& E_a){

	MTRand_closed rng(0);

	for (int p=0; p<numberOfPlants; p++){
		E_p.at(p) = rng()*0.02 + 0.14;
	}
	for (int p=0; p<numberOfAnimals; p++){
		E_a.at(p) = rng()*0.02 + 0.14;
	}

	return true;
}

bool drawColonisationProbabilities(int numberOfPlants, int numberOfAnimals, Adjacency& masterNetwork, 	vector<vector<double> >& C_plant, 	vector<vector<double> >& C_animal){

	MTRand_closed rng(0);

	for (int i=0; i<numberOfPlants; i++){

		for (int j=0; j<numberOfAnimals; j++){

			if (masterNetwork.connected(i,j)){
				C_plant.at(i).at(j) = rng()*0.02 + 0.04;
				C_animal.at(j).at(i) = rng()*0.02 + 0.04;
			}
		}
	}


	return true;
}
