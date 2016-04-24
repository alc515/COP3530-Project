#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <climits>
using namespace std;

struct realm{
	//the charm
	string charm;
	//number of magi
	int magi;
	//array used to find maximum number of usable magi (LIS)
	int* S;
	//array used to find the optimum powers (LIS)
	int* parent;
	//the maximum number of magi that can be used
	int maxMagi;
	//the values of each magi's power
	int* powers;
	//the optimized powers based on the max number of magi
	int* optimumPowers;
	//number of incantations needed to reach the other realms (turned into edges)
	int* incantationsNeeded;
	//the feasible edges to other realms
	list<int> edges;
	//whether a realm has been visited
	bool visited;
	//holds the current tentativeDistance of a realm (solution incantations)
	int tentativeDistance;
	//holds the current tentativeGems of a realm (solution gems)
	int tentativeGems;
};

//finds the powers that maximize the number of feasible magi
void findOptimumPowers(realm* r){
    r->optimumPowers = new int[r->maxMagi];
    int i=r->maxMagi-1;
    int index = r->S[i];

    while(index!=-1){
        r->optimumPowers[i]=r->powers[index];
        index=r->parent[index];
        i--;
    }
}

//performs binary search to find location of a value
int binarySearch(realm* r, int low, int high, int val){
   int mid;

   //compares high and low value to find target index
   while(low<high){
       mid=(high+low)/2;

       //if mid value is less than the value
       if(r->powers[r->S[mid]]<val){
           low=mid+1;
       }
       //if mid value is greater than the value
       else if(r->powers[r->S[mid]]>val){
           high=mid;
       }
       //if the mid value is the value
       else{
           return mid;
       }
   }

   //if index isn't found in the loop then the low index is the target index
   return low;
}

//finds longest increasing subsequence of powers (maxMagi)
void analyzeMagi(realm* r){
    //allocates space for arrays
	r->S = new int[r->magi];
    r->parent = new int[r->magi];

    //sets initial values
    int index = 0;
    int newIndex;
    r->S[0]=0;
    r->parent[0]=-1;

    for(int i=1 ; i<r->magi ;i++){
        //if the value is smaller than the previous value - replaces the index of the smallest value greater than or equal to the new value
        if(r->powers[i]<=r->powers[r->S[index]]){
            newIndex=binarySearch(r,0,index,r->powers[i]);
            r->S[newIndex]=i;
            if(newIndex!=0)
            {
            	r->parent[i]=r->S[newIndex-1];
            }
            else{
            	r->parent[i]=-1;
            }
        }
        //if value is larger than the previous value - appends the new index to the end of the current list
        else{
            index=index+1;
            r->S[index]=i;
            r->parent[i]=r->S[index-1];
        }
    }
    r->maxMagi=index+1;

    //finds the powers for the maximum number of magi
    findOptimumPowers(r);
}

//finds the distances between strings
int stringCompare(string string1, string string2){
    int size1;
    int size2;
    size1=string1.size()+1;
    size2=string2.size()+1;

    //allocates memory for 2-D array to hold differences
    int** myarray = new int* [size1];
    for (int i=0; i< size1; i++){
        myarray[i] = new int[size2];
    }

    for(int i=0; i<size1; i++){
        myarray[i][0]=i;
    }
    for(int i=0; i < size2; i++){
        myarray[0][i]=i;
    }

    //compares the two strings
    int lowest;
	for(int i=0; i<size1-1; i++){
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
		delete[] myarray[i];
	}
	delete[] myarray;

    return answer;
}

//declares custom comparator to get smallest tentative distance in queue
typedef bool (*comp)(realm,realm);
bool compare(realm a, realm b)
{
   return (a.tentativeDistance>b.tentativeDistance);
}

//implements Dijkstra's algorithm to find incantations and gems to reach endRealm
void traverseRealms(realm* realms, int startRealm, int endRealm, int numRealms){
	int dist;
	int gems;
	realm r;
	int nextRealm;
	//initializes impossible to true
	bool impossible=true;

	//declares each realm unvisited and declares tentative distances as infinity and tentative gems as 0
	for(int i=0; i<numRealms; i++){
		realms[i].visited=false;
		realms[i].tentativeDistance=INT_MAX;
		realms[i].tentativeGems=0;
	}
	//declares tentative distance for first realm as 0
	realms[startRealm].tentativeDistance=0;

	//declares priority queue for Dijkstra's Algorithm
	priority_queue<realm,vector<realm>,comp> q(compare);
	q.push(realms[startRealm]);

	//loops and updates tentative distances until endRealm is found
	while(!q.empty()){
		//pop realm with smallest tentative distance and mark it visited
		r=q.top();
		q.pop();

		//check if realm was already visited
		if(r.visited){
			continue;
		}
		r.visited=true;

		//base case (realm popped is endRealm)
		if(r.charm==realms[endRealm].charm){
			impossible=false;
			cout << r.tentativeDistance << " " << r.tentativeGems << endl;
			break;
		}

		for(list<int>::iterator iter=r.edges.begin(); iter!=r.edges.end(); iter++){
			//gets the nextRealm from the edges list
			nextRealm=*iter;

			//gets the value of the distance to reach the next realm and compares it to the current tentative distance
			dist=r.tentativeDistance+r.incantationsNeeded[nextRealm];
			if(dist<realms[nextRealm].tentativeDistance){
				//updates tentative distance
				realms[nextRealm].tentativeDistance=dist;

				//updates tentative gems
				gems=r.tentativeGems;
				for(int j=0; j<r.incantationsNeeded[nextRealm]; j++){
					gems+=r.optimumPowers[j];
				}
				realms[nextRealm].tentativeGems=gems;

				//pushes nextRealm onto the queue
				q.push(realms[nextRealm]);
			}
		}
	}
	//if no path was found
	if(impossible){
		cout << "IMPOSSIBLE\n";
	}
}

//main program
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

		//gets powers of each realm's magi
		realms[i].powers = new int[realms[i].magi];
		for(int j=0; j<realms[i].magi; j++){
			cin >> realms[i].powers[j];
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

	//declares the arrays used to hold the incantationsNeeded to reach each realm
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

	//checks the incantationsNeeded against the maxMagi to determine if edge is possible
	for(int i=0; i<numRealms; i++){
		for(int j=0; j<numRealms; j++){
			if(i!=j&&realms[i].incantationsNeeded[j]<=realms[i].maxMagi){
				realms[i].edges.push_back(j);
			}
		}
	}

	//finds incantations and gems to reach end realm
	traverseRealms(realms, startRealm, endRealm, numRealms);

	//finds incantations and gems to return to start realm
	traverseRealms(realms, endRealm, startRealm, numRealms);

	//deletes dynamically allocated memory
	for(int i=0; i<numRealms; i++){
		delete[] realms[i].powers;
		delete[] realms[i].S;
		delete[] realms[i].parent;
		delete[] realms[i].optimumPowers;
		delete[] realms[i].incantationsNeeded;
	}
	delete[] realms;
}

int main(){
	messenger();
	return 0;
}
