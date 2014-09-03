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
#include "IStatisticsTracker.h"
//#include "Patch.h"
#include "Adjacency.h"
#include "mtrand.h"
using namespace std;


int main(){

	MTRand_closed rng(time(NULL));
	int numberOfPlants = 10;
	int numberOfAnimals = 12;
	int iterationCount = 10;
	int landscapeDimension = 100;

//	IStatisticsTracker* tracker = new AggregatePopulationTracker(numberOfPlants, numberOfAnimals);
	IStatisticsTracker* tracker = new SpeciesPopulationTracker(numberOfPlants, numberOfAnimals);
//	IStatisticsTracker* tracker = new BasicLandscapeTracker(numberOfPlants, numberOfAnimals, landscapeDimension, landscapeDimension, 10);

// the master network defines all possible connections, it is taken form the literature (empirical network):
	Adjacency masterNetwork(numberOfPlants, numberOfAnimals);
	masterNetwork.readFromFile("empiricalNetwork_Oelsen2002_16.text");
//	masterNetwork.testNetwork();
//	masterNetwork.print(cout);

	Simulation testSimulation(rng, iterationCount, numberOfPlants, numberOfAnimals, landscapeDimension, landscapeDimension, masterNetwork);

	cout << "set up successful" << endl;
	testSimulation.run(rng, tracker);
	cout << "run successful " << endl;
//	testSimulation.patchState();
	testSimulation.saveParameters();

	return 0;
}
