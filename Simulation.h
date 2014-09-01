/*
 * Simulation.h
 *
 *  Created on: 1 Sep 2014
 *      Author: rusty
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include <iostream>
#include "mtrand.h"
#include "Adjacency.h"
using namespace std;

class Simulation{

private:

	const int numberOfPlants, numberOfAnimals;
	const int landscapeWidth, landscapeHeight;

	// extinction probability vectors:
	vector<double> E_p;
	vector<double> E_a;

	// the master network defines all possible connections, it is taken form the literature (empirical network): (could be created form niche model?)
	Adjacency& masterNetwork;

	// matrix that defines probability of plant colonisation, mediated by animal partners:
	vector<vector<double> > C_plant;
	// matrix that defines probability of animal colonisation, mediated by plant partners:
	vector<vector<double> > C_animal;


public:
	~Simulation(){}
	Simulation(MTRand_closed& rng, int numberOfPlants, int numberOfAnimals, int landscapeWidth, int landscapeHeight, Adjacency& masterNetwork):numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals), landscapeWidth(landscapeWidth), landscapeHeight(landscapeHeight), masterNetwork(masterNetwork){

		E_p.resize(numberOfPlants, 0);
		E_a.resize(numberOfAnimals, 0);
		drawExtinctionProbabilities(rng);

		C_plant.resize(numberOfPlants, vector<double>(numberOfAnimals, 0));
		C_animal.resize(numberOfAnimals, vector<double>(numberOfPlants, 0));
		drawColonisationProbabilities(rng);

	}

private:
	bool drawExtinctionProbabilities(MTRand_closed& rng){


		for (int p=0; p<numberOfPlants; p++){
			E_p.at(p) = rng()*0.02 + 0.14;
		}
		for (int p=0; p<numberOfAnimals; p++){
			E_a.at(p) = rng()*0.02 + 0.14;
		}

		return true;
	}

	bool drawColonisationProbabilities(MTRand_closed& rng){

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

};


#endif /* SIMULATION_H_ */
