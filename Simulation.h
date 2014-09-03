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
#include "Patch.h"
#include "IStatisticsTracker.h"
#include "ExtinctionTracker.h"
using namespace std;

class Simulation{

private:

	const int iterationCount;
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

	const int patchCount;
	vector<Patch> patches;

public:
	~Simulation(){}
	Simulation(MTRand_closed& rng, int iterationCount, int numberOfPlants, int numberOfAnimals, int landscapeWidth, int landscapeHeight, Adjacency& masterNetwork):iterationCount(iterationCount),numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals), landscapeWidth(landscapeWidth), landscapeHeight(landscapeHeight), masterNetwork(masterNetwork), patchCount(landscapeWidth*landscapeHeight){

		drawExtinctionProbabilities(rng);
		drawColonisationProbabilities(rng);
		createPatches();

	}

	Simulation(Simulation& sim_to_copy):iterationCount(sim_to_copy.iterationCount), numberOfPlants(sim_to_copy.numberOfPlants), numberOfAnimals(sim_to_copy.numberOfAnimals), landscapeWidth(sim_to_copy.landscapeWidth), landscapeHeight(sim_to_copy.landscapeHeight), masterNetwork(sim_to_copy.masterNetwork), patchCount(sim_to_copy.patchCount){


		this->E_a = sim_to_copy.E_a;
		this->E_p = sim_to_copy.E_p;
		this->C_plant = sim_to_copy.C_plant;
		this->C_animal= sim_to_copy.C_animal;

		createPatches();

	}

	bool run(MTRand_closed& rng, IStatisticsTracker* tracker){

		for (int t=0; t<iterationCount; t++){
			// simultaneity issue!
			colonisations(rng);
			tracker->track(patches);
		}
		tracker->save();
		return true;
	}

	bool stabilityCheck(MTRand_closed& rng){

		ExtinctionTracker tracker(C_plant, C_animal);

		for (int t=0; t<iterationCount; t++){
			// simultaneity issue!
			colonisations(rng);
			if(!tracker.checkExtinctions(patches)){
				cerr << "at least one extinct interaction" << endl;
				return false;
			}
		}
		return true;
	}

	bool patchState(){
		for (int p=0; p<patchCount; p++){
			patches.at(p).printPatchInfo(cout);
		}

		return true;
	}

	bool saveParameters(){
		ofstream ofile;
		ofile.open("stable_parameters.test");
		if (!ofile){return false;}

		for (int i=0; i<numberOfPlants; i++){
			ofile << E_p.at(i) << ", ";
		}
		ofile << endl;
		for (int i=0; i<numberOfAnimals; i++){
			ofile << E_a.at(i) << ", ";
		}
		ofile << endl << endl;

		for (int i=0; i<numberOfPlants; i++){
			for (int j=0; j<numberOfAnimals; j++){
				ofile << C_plant.at(i).at(j) << ", ";
			}
			ofile << endl;
		}
		ofile << endl << endl;
		for (int i=0; i<numberOfAnimals; i++){
			for (int j=0; j<numberOfPlants; j++){
				ofile << C_animal.at(i).at(j) << ", ";
			}
			ofile << endl;
		}
		ofile.close();
		return true;
	}

private:
	bool drawExtinctionProbabilities(MTRand_closed& rng){
		E_p.resize(numberOfPlants, 0);
		E_a.resize(numberOfAnimals, 0);

		for (int p=0; p<numberOfPlants; p++){
			E_p.at(p) = rng()*0.02 + 0.14;
			cout << E_p.at(p) << ", ";
		}
		cout << endl;
		for (int p=0; p<numberOfAnimals; p++){
			E_a.at(p) = rng()*0.02 + 0.14;
		}

		return true;
	}
	bool drawColonisationProbabilities(MTRand_closed& rng){
		C_plant.resize(numberOfPlants, vector<double>(numberOfAnimals, 0));
		C_animal.resize(numberOfAnimals, vector<double>(numberOfPlants, 0));

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
	bool createPatches(){

		int patchID = 0;
		for (int y=0;y<landscapeHeight; y++){
			for (int x=0; x< landscapeWidth; x++){

				patches.push_back(Patch(patchID, x, y, landscapeWidth, landscapeHeight, numberOfPlants, numberOfAnimals));
//				patches.at(patchID).printPatchInfo(cout);
				patchID++;
			}
		}
		if (patchCount!= (int)patches.size()){
			cerr << "wrong number of patches!" << endl;
			return false;
		}
		return true;
	}

	bool extinctions(MTRand_closed& rng){

		for (int p=0; p< patchCount; p++){
			for (int i=0; i<numberOfPlants; i++){

				if (patches.at(p).isPlantPresent(i) && rng()<=E_p.at(i)){
					patches.at(p).changePlantPresence(i);
				}

			}

			for (int j=0; j<numberOfAnimals; j++){

				if (patches.at(p).isAnimalPresent(j) && rng()<=E_a.at(j)){
					patches.at(p).changeAnimalPresence(j);
				}

			}
		}
		return true;
	}
	bool colonisations(MTRand_closed& rng){

		for (int p=0; p< patchCount; p++){
			patches.at(p).calculateColonisationProbabilities(C_plant, C_animal, patches);
		}
		extinctions(rng);
		for (int p=0; p< patchCount; p++){
			patches.at(p).performColonisations(rng);
		}
		return true;
	}

};


#endif /* SIMULATION_H_ */
