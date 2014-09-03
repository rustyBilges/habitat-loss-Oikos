/*
 * InteractionTracker.h
 *
 *  Created on: 3 Sep 2014
 *      Author: rusty
 *
 *      Class to check for regional extinction of interactions.
 *      ?? ALSO NEED TO CHECK FOR EXTINCTION OF SPECIES?? (no because thsi would lead to extiction of interaction, unless species is non-interacting)
 */

#ifndef EXTINCTIONTRACKER_H_
#define EXTINCTIONTRACKER_H_

#include <vector>
using namespace std;

class ExtinctionTracker{

private:
	int numberOfInteractions;
	vector<vector<int> > interactionIJ;  // the location of the interactions in the C_plant vector

	vector<vector<double> >& C_plant;
	vector<vector<double> >& C_animal;

//	vector<int> interactionPresence;   // to store global presence/absence of each interaction

public:
	~ExtinctionTracker(){}
	ExtinctionTracker(vector<vector<double> >& C_plant, vector<vector<double> >& C_animal):C_plant(C_plant), C_animal(C_animal){

		int tempInteractionCount = 0;
		for (unsigned int i=0; i<C_plant.size(); i++){
			for (unsigned int j=0; j<C_plant.at(0).size(); j++){
				if (C_plant.at(i).at(j)!=0){
					tempInteractionCount++;
					vector<int> tempIJ;
					tempIJ.push_back(i);
					tempIJ.push_back(j);
					interactionIJ.push_back(tempIJ);
				}
			}
		}
		numberOfInteractions = tempInteractionCount;
//		interactionPresence.resize(numberOfInteractions, 1);  // initially all interactions are present (as all species exist in each patch)
	}

	// check for regional extinction of interactions, returns true if all interactions are present in at least one patch
	bool checkExtinctions(vector<Patch>& patches){

		if (!checkForInteractionExtinctions(patches)){return false;}
//		if (!checkForSpeciesExtinctions(patches)){return false;}      // weaker conditions than the above
		return true;
	}

private:

	bool checkForInteractionExtinctions(vector<Patch>& patches){     // returns false if an interaction has gone extinct

		for (int i=0; i<numberOfInteractions; i++){
			if (!checkForIthInteraction(i, patches)){return false;}  // this interaction is extinct
		}

		return true;
	}

	bool checkForSpeciesExtinctions(vector<Patch>& patches){         // returns false if at least one species is regionally extinct

		for (unsigned int i=0; i<C_plant.size(); i++){
			if(!checkForIthPlant(i,patches)){return false;}
		}
		for (unsigned int i=0; i<C_animal.size(); i++){
			if(!checkForJthAnimal(i,patches)){return false;}
		}
		return true;
	}
	bool checkForIthInteraction(int I, vector<Patch>& patches){

		for (unsigned int p=0; p<patches.size(); p++){
			if (patches.at(p).isPlantPresent(interactionIJ.at(I).at(0)) && patches.at(p).isAnimalPresent(interactionIJ.at(I).at(1))){
				return true;
			}
		}
		return false;
	}

	bool checkForIthPlant(int I, vector<Patch>& patches){

		for (unsigned int p=0; p<patches.size(); p++){
			if (patches.at(p).isPlantPresent(I)){
				return true;
			}
		}
		return false;
	}
	bool checkForJthAnimal(int J, vector<Patch>& patches){

		for (unsigned int p=0; p<patches.size(); p++){
			if (patches.at(p).isAnimalPresent(J)){
				return true;
			}
		}
		return false;
	}
};


#endif /* EXTINCTIONTRACKER_H_ */
