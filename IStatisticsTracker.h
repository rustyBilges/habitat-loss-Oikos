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
			for (int j=0; j<numberOfAnimals; j++){
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

class SpeciesPopulationTracker : public IStatisticsTracker{

private:
	int numberOfPlants, numberOfAnimals;

	vector<vector<int> >animalPopulation;
	vector<vector<int> >plantPopulation;

public:
	~SpeciesPopulationTracker(){}
	SpeciesPopulationTracker(int numberOfPlants, int numberOfAnimals):numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals){
		animalPopulation.resize(numberOfAnimals, vector<int>(0,0));
		plantPopulation.resize(numberOfPlants, vector<int>(0,0));
	}

	bool track(vector<Patch>& patches){

		for (int i=0; i<numberOfPlants; i++){
			int N_p = 0;
			for (unsigned int p=0; p<patches.size(); p++){
				if (patches.at(p).isPlantPresent(i)){N_p ++;}
			}
			plantPopulation.at(i).push_back(N_p);
		}
		for (int j=0; j<numberOfAnimals; j++){
			int N_a = 0;
			for (unsigned int p=0; p<patches.size(); p++){
				if (patches.at(p).isAnimalPresent(j)){N_a ++;}
			}
			animalPopulation.at(j).push_back(N_a);
		}

		return true;
	}

	bool save(){
		ofstream ofile;
		ofile.open("plant_populations.test");
		if (!ofile){return false;}

		for (unsigned int t =0; t<plantPopulation.at(0).size(); t++){
			ofile << t;
			for (int i= 0; i<numberOfPlants; i++){
				ofile << ", " << plantPopulation.at(i).at(t);
			}
			ofile << endl;
		}
		ofile.close();

		ofile.open("animal_populations.test");
		if (!ofile){return false;}

		for (unsigned int t =0; t<animalPopulation.at(0).size(); t++){
			ofile << t;
			for (int i= 0; i<numberOfAnimals; i++){
				ofile << ", " << animalPopulation.at(i).at(t);
			}
			ofile << endl;
		}
		ofile.close();

		return true;
	}
};

class BasicLandscapeTracker : public IStatisticsTracker{
// this class tracks how many plants and animals there are in each patch over the course of the simulation

private:
	int numberOfPlants, numberOfAnimals;

	int numberOfPathces, landscapeWidth, landscapeHeight;

	int saveEvery;

	vector<vector<int> >tempAggregateHeatmap;
	vector<vector<vector<int> > >fullHeatmapData;


public:
	~BasicLandscapeTracker(){}
	BasicLandscapeTracker(int numberOfPlants, int numberOfAnimals, int width, int height, int saveEvery):numberOfPlants(numberOfPlants), numberOfAnimals(numberOfAnimals), numberOfPathces(width*height), landscapeWidth(width), landscapeHeight(height), saveEvery(saveEvery){
		tempAggregateHeatmap.resize(numberOfPathces, vector<int>(4,0));
		//fullHeatmapData.resize();
	}

	bool track(vector<Patch>& patches){

		int I = 0;
		int j = 0;
		for (int p=0; p<numberOfPathces; p++){

			int N_p = 0;
			int N_a = 0;

			for (int i=0; i<numberOfPlants; i++){
				if (patches.at(p).isPlantPresent(i)){N_p++;}
			}
			for (int i=0; i<numberOfAnimals; i++){
				if (patches.at(p).isAnimalPresent(i)){N_a++;}
			}
			tempAggregateHeatmap.at(p).at(0) = j;
			tempAggregateHeatmap.at(p).at(1) = I;
			tempAggregateHeatmap.at(p).at(2) = N_p;
			tempAggregateHeatmap.at(p).at(3) = N_a;

			I++;
			if (I>=landscapeWidth){
				I=0;
				j++;
			}
		}
		fullHeatmapData.push_back(tempAggregateHeatmap);
		return true;
	}

	bool save(){

		ofstream ofile;

		for (unsigned int t = 0; t<fullHeatmapData.size(); t++){
			if (t%saveEvery==0){
				stringstream t_ss;
				t_ss << t;
				string fname_p = "aggregate_landscape_t"+t_ss.str()+".test";
				ofile.open(fname_p.c_str());

				if (!ofile){return false;}

				for (int p=0; p<numberOfPathces; p++){
					for (int c= 0; c< 4; c++){
						ofile << fullHeatmapData.at(t).at(p).at(c) << ",";
					}
					ofile << endl;
				}

				ofile.close();

			}
		}


		return true;
	}
};

#endif /* ISTATISTICSTRACKER_H_ */
