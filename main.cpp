/*
 * main.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: Kyle
 */

#include <iostream>
#include <string>
using namespace std;

struct realm{
	string charm;
	int magi;
	int usableMagi;
	int* optimumPowers;
	int* powers;
};

void magiCount(realm* r){

}

void messenger(){
	//gets the number of realms and creates an array to store them in
	int numRealms;
	cin >> numRealms;
	realm* realms = new realm[numRealms];

	//gets the characteristics of each realm
	for(int i=0; i<numRealms; i++){
		//charm
		cin >> realms[i].charm;

		//number of magi
		cin >> realms[i].magi;

		//powers of each realm's magi
		realms[i].powers = new int[realms[i].magi];
		for(int j=0; j<realms[i].magi; j++){
			cin >> realms[i].powers[j];
		}
	}

	//gets current and target realm's charm
	string startCharm;
	string endCharm;
	cin >> startCharm;
	cin >> endCharm;

	//gets the maximum number of usable magi for each realm
	for(int i=0; i<numRealms; i++){
		magiCount(&realms[i]);
	}

	//deletes dynamically allocated memory
	for(int i=0; i<numRealms; i++){
		delete realms[i].powers;
	}
	delete realms;
}

int main(){
	messenger();
	return 0;
}
