/*
 * IStatisticsTracker.h
 *
 *  Created on: 1 Sep 2014
 *      Author: rusty
 */

#ifndef ISTATISTICSTRACKER_H_
#define ISTATISTICSTRACKER_H_

#include <vector>
using namespace std;

class IStatisticsTracker{

public:
	virtual ~IStatisticsTracker(){}

	virtual bool track(vector<Patch>& patches) = 0;
	virtual bool save() = 0;
};

class AggregatePopulationTracker : public IStatisticsTracker{

private:
	int numberOfPlants, numberOfAnimals;

	vector<int> animalPopulation;
	vector<int> plantPopulation;

public:
	~AggregatePopulationTracker(){}
	AggregatePopulationTracker(int numberOfPlants, int numberOfAnimals):numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals){}

	bool track(vector<Patch>& patches){

		int N_p = 0;
		int N_a = 0;

		for (unsigned int p=0; p<patches.size(); p++){
			for (int i=0; i<numberOfPlants; i++){
				if (patches.at(p).isPlantPresent(i)){N_p ++;}
			}
			for (int j=0; j<numberOfPlants; j++){
				if (patches.at(p).isAnimalPresent(j)){N_a ++;}
			}
		}
		plantPopulation.push_back(N_p);
		animalPopulation.push_back(N_a);

		return true;
	}

	bool save(){
		ofstream ofile;
		ofile.open("aggregate_population.test");
		if (!ofile){return false;}

		for (unsigned int t =0; t<plantPopulation.size(); t++){
			ofile << t << ", " << plantPopulation.at(t) << ", " << animalPopulation.at(t) << endl;
		}

		ofile.close();

		return true;
	}
};


#endif /* ISTATISTICSTRACKER_H_ */
