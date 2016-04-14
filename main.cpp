/*
 * main.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: Kyle
 */

#include <iostream>
#include <string>
#include <queue>
using namespace std;

struct realm{
	//the charm
	string charm;
	//number of magi
	int magi;
	//possible magi that can be used from the position of the magi
	int* possibleMagi;
	//the maximum number of magi that can be used
	int maxMagi;
	//the values of each magi's power
	int* powers;
	//the optimized powers based on the max number of magi
	int* optimumPowers;
	//number of incantations needed to reach the other realms
	int* incantationsNeeded;
	//whether a realm has been visited
	bool visited;
//	//whether the realm is capable of reaching the next realm
//	bool feasible;
};

struct state{
	int currentRealm;
	int nextRealm;
	int gemCount;
	int incantationCount;
};

//finds the optimized powers based on the max number of magi
void backtrace(realm* r){
	int k=0;
	int lastPower=0;
	int currentPower;
	int currentPossible=0;
	int i=0;
	r->optimumPowers = new int[r->maxMagi];

	//scan all magi with the same possibility value and pick smallest feasible power
	while(i<r->magi-1){
		currentPower=r->powers[i];
		currentPossible=r->possibleMagi[i];
		i++;
		while(i<r->magi-1&&r->possibleMagi[i]==currentPossible){
			if(r->powers[i]<currentPower&&r->powers[i]>lastPower){
				currentPower=r->powers[i];
			}
			i++;
		}
		r->optimumPowers[k]=currentPower;
		lastPower=r->optimumPowers[k];
		k++;
	}
	//adds last index if that magi was used
	if(k<r->maxMagi){
		r->optimumPowers[k]=r->powers[r->magi-1];
	}
}

//analyzes the powers of the magi and finds 1) the max number of magi that can be used 2) the optimized powers based on the max number of magi
void analyzeMagi(realm* r){
	//sets possible magi for the last position equal to 1 (base case)
	r->possibleMagi[r->magi-1]=1;

	//calculates maximum possible magi for a realm using dynamic programming
	for(int i=r->magi-2; i>=0; i--){
		//checks values greater than i for previously computed value
		for(int j=i+1; j<r->magi; j++){
			if(r->powers[i]<r->powers[j]){
				r->possibleMagi[i]=r->possibleMagi[j]+1;
				break;
			}
		}
		//if computed value for possible magi is less than the next index's, then set equal to next index's (max)
		if(r->possibleMagi[i]<r->possibleMagi[i+1]){
			r->possibleMagi[i]=r->possibleMagi[i+1];
		}
	}

	//maxMagi will be the first index of possibleMagi
	r->maxMagi=r->possibleMagi[0];
	backtrace(r);
}

//finds the distances between strings
int stringCompare(string string1,string string2){
    int size1;
    int size2;
    size1=string1.size()+1;
    size2=string2.size()+1;

    int** myarray=new int* [size1];
    for (int i=0; i< size1;i++){
        myarray[i]=new int[size2];
    }
    for(int i=0; i< size1; i++){
        myarray[i][0]=i;
    }
    for(int i=0; i < size2; i++){
        myarray[0][i]=i;
    }

    int lowest;
	for(int i=0; i< size1-1;i++){
		for(int j=0; j< size2-1; j++){
			lowest=myarray[i][j];
			if (string1.at(i)!=string2.at(j)){
				lowest++;
			}
			if(lowest>myarray[i][j+1]+1){
				lowest=myarray[i][j+1]+1;
			}
			if(lowest>myarray[i+1][j]+1){
				lowest=myarray[i+1][j]+1;
			}
			myarray[i+1][j+1]=lowest;
		}
	}
	int answer=myarray[size1-1][size2-1];

	//clears up dynamically allocated memory
	for(int i=0; i<size1; i++){
		delete myarray[i];
	}
	delete myarray;

    return answer;
}


//void greedyAlgorithm(realm* realms, int startRealm, int endRealm, int numRealms){
//	int currentRealm=startRealm;
//	int nextRealm=startRealm;
//	int gemCount=0;
//	int incantationCount=0;
//	bool impossible=false;
//	stack<state> stack;
//	state s;
//
//	//declares every realm unvisited
//	for(int i=0; i<numRealms; i++){
//		realms[i].visited=false;
//	}
//
//	while(nextRealm!=endRealm&&!impossible){
//		//determines which realms can be reached from the current unvisited realm
//		if(!realms[currentRealm].visited){
//			for(int i=0; i<numRealms; i++){
//				if(realms[currentRealm].incantationsNeeded[i]<=realms[currentRealm].maxMagi){
//					realms[i].feasible=true;
//				}
//				else{
//					realms[i].feasible=false;
//				}
//			}
//			realms[currentRealm].visited=true;
//		}
//
//		//if you can reach the end realm from the current realm
//		if(realms[endRealm].feasible){
//			nextRealm=endRealm;
//		}
//		//if you have to try to pass through other realms
//		else{
//			for(int i=0; i<numRealms; i++){
//				if(realms[i].feasible&&realms[i].incantationsNeeded[endRealm]<=realms[nextRealm].incantationsNeeded[endRealm]){
//					nextRealm=i;
//				}
//			}
//		}
//
//		//if unable to find a feasible nextRealm
//		if(currentRealm==nextRealm){
//			impossible=true;
//		}
//
//		if(!impossible){
//			s.currentRealm=currentRealm;
//			s.nextRealm=nextRealm;
//			s.incantationCount=incantationCount;
//			s.gemCount=gemCount;
//			stack.push(s);
//			//gets the number of gems and incantations needed to move to the next realm
//			for(int i=0; i<realms[currentRealm].incantationsNeeded[nextRealm]; i++){
//				gemCount+=realms[currentRealm].optimumPowers[i];
//				incantationCount++;
//			}
//			//the current realm is now the next realm
//			currentRealm=nextRealm;
//		}
//		else if(!stack.empty()){
//			s=stack.top();
//			stack.pop();
//			currentRealm=s.currentRealm;
//			nextRealm=currentRealm;
//			gemCount=s.gemCount;
//			incantationCount=s.incantationCount;
//			realms[s.nextRealm].feasible=false;
//			impossible=false;
//		}
//	}
//
//	//prints the number of incantations and gems if possible
//	if(impossible){
//		cout << "IMPOSSIBLE\n";
//	}
//	else{
//		cout << incantationCount << " " << gemCount << endl;
//	}
//}

void BFS(realm* realms, int startRealm, int endRealm, int numRealms){
	int currentRealm=startRealm;
	int gemCount=0;
	int incantationCount=0;
	int tempGems;
	int tempIncantations;
	int minIncantations=0;
	int minGems=0;
	bool impossible=true;
	queue<state> q;
	state s;

	//pushes intital values onto queue
	s.currentRealm=currentRealm;
	s.gemCount=gemCount;
	s.incantationCount=incantationCount;
	q.push(s);

	//declares every realm unvisited
	for(int i=0; i<numRealms; i++){
		realms[i].visited=false;
	}

	while(!q.empty()){
		//gets the front of the queue
		s=q.front();
		q.pop();
		currentRealm=s.currentRealm;
		gemCount=s.gemCount;
		incantationCount=s.incantationCount;
		realms[currentRealm].visited=true;

		//if the currentRealm is the endRealm
		if(currentRealm==endRealm&&(minIncantations==0||incantationCount<minIncantations)){
			impossible=false;
			minGems=gemCount;
			minIncantations=incantationCount;
			continue;
		}

		//determines which realms can be reached from the current realm
		for(int i=0; i<numRealms; i++){
			if(!realms[i].visited&&realms[currentRealm].incantationsNeeded[i]<=realms[currentRealm].maxMagi){
				//resets gems and incantations back to current realms values
				tempGems=gemCount;
				tempIncantations=incantationCount;
				//gets the number of gems and incantations needed to move to the next realm
				for(int j=0; j<realms[currentRealm].incantationsNeeded[i]; j++){
					tempGems+=realms[currentRealm].optimumPowers[j];
					tempIncantations++;
				}
				//pushes new realms' values into queue
				s.gemCount=tempGems;
				s.incantationCount=tempIncantations;
				s.currentRealm=i;
				q.push(s);
			}
		}
	}

	//prints the number of incantations and gems if possible
	if(impossible){
		cout << "IMPOSSIBLE\n";
	}
	else{
		cout << minIncantations << " " << minGems << endl;
	}
}

void messenger(){
	//gets the number of realms and creates an array to store them in
	int numRealms;
	cin >> numRealms;
	realm* realms = new realm[numRealms];

	//gets the characteristics of each realm
	for(int i=0; i<numRealms; i++){
		//gets charm
		cin >> realms[i].charm;

		//gets number of magi
		cin >> realms[i].magi;

		//gets powers of each realm's magi/initializes possible magi
		realms[i].powers = new int[realms[i].magi];
		realms[i].possibleMagi = new int[realms[i].magi];
		for(int j=0; j<realms[i].magi; j++){
			cin >> realms[i].powers[j];
			realms[i].possibleMagi[i]=1;
		}
	}

	//gets starting and ending realms' charms
	string startCharm;
	string endCharm;
	cin >> startCharm;
	cin >> endCharm;

	//determines which realms are the starting and ending realms
	int startRealm;
	int endRealm;
	for(int i=0; i<numRealms; i++){
		if(realms[i].charm==startCharm){
			startRealm=i;
		}
		if(realms[i].charm==endCharm){
			endRealm=i;
		}
	}

	//gets the maximum number of usable magi and their powers for each realm
	for(int i=0; i<numRealms; i++){
		analyzeMagi(&realms[i]);
	}

	//declares the array needed to hold the incantations needed to reach each realm
	for(int i=0; i<numRealms; i++){
		realms[i].incantationsNeeded = new int[numRealms];
	}

	//finds each charms closeness to other charms
	for(int i=0; i<numRealms; i++){
		for(int j=i; j<numRealms; j++){
			realms[i].incantationsNeeded[j]=stringCompare(realms[i].charm, realms[j].charm);
			realms[j].incantationsNeeded[i]=realms[i].incantationsNeeded[j];
		}
	}

	//finds incantations and gems to reach end realm
	BFS(realms, startRealm, endRealm, numRealms);

	//finds incantations and gems to return to start realm
	BFS(realms, endRealm, startRealm, numRealms);

	//deletes dynamically allocated memory
	for(int i=0; i<numRealms; i++){
		delete[] realms[i].powers;
		delete[] realms[i].possibleMagi;
		delete[] realms[i].optimumPowers;
		delete[] realms[i].incantationsNeeded;
	}
	delete[] realms;
}

int main(){
	messenger();
	return 0;
}
