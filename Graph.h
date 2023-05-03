//********************************************************************
// ASU CSE310 Fall 2022 Assignment #7
// Name of Author:  Race Musgrave
// ASU ID:          1215151093
// Description: This is the header file that defines a weighted directed graph
//********************************************************************

#include "MinPriorityQueue.h"

using namespace std;

class Graph
{
    private:
    int numOfNode;                 //number of nodes in the graph
    MinPriorityQueue* cityHeap;    //adjacency list of the graph, it is a min-heap of departure city objects

    public:
    MinPriorityQueue* S;
    string* nameArr;
    Graph(int numOfNode, MinPriorityQueue* cityHeap);
    ~Graph();
    void printGraph();
    void initializeSingleSource(string sourceCityName);
    void relax(DepCity u, DepCity v);
    int findOneCity(string aCityName);
    int getDuplicates(int idx);
    void dijkstra(string sourceCityName);
    void printDijkstraPath(string sourceCityName);
    void printPath(string sourceCityName, DepCity* aCity);
    void printMid(DepCity toPrint);
    DepCity* arrToDep(ArrCity arr);

};

//Constructor
Graph::Graph(int num, MinPriorityQueue* cityHeap){
   this->numOfNode = num;
   this->cityHeap = cityHeap;
   nameArr = new string[num];
   for (int i = 0; i < numOfNode; i++) {
       nameArr[i] = cityHeap->getDepCityArr()[i].name;
   }
}

//Destructor
Graph::~Graph(){
   delete [] cityHeap;
   cityHeap = nullptr;
   numOfNode = 0;
}

//This function prints the graph. It traverses through the vertex list,
//then for each vertex, it prints its adjacent list from head to tail.
void Graph::printGraph(){
   cityHeap->printHeap();
}

//Given a source departure city's name, this function initializes the graph
void Graph::initializeSingleSource(string sourceCityName){
   int source = cityHeap->isFound(sourceCityName);
   DepCity sourceCity;
   if(source >= 0){
      for(int i = 0; i < numOfNode; i++){
         cityHeap->getDepCityArr()[i].d = 30000;
         cityHeap->getDepCityArr()[i].pi = nullptr;
      }
      sourceCity.d = 0;
      cityHeap->decreaseKey(source, sourceCity);
   }
   else{
      cout << "initialize single source could not perform, the city was not found\n";
   }
}

//used to fix the distances between cities and set the pi structure
void Graph::relax(DepCity u, DepCity v){
    DepCity newCityWithNewDValue;
    ArrCity* W = u.arrCityList->findArrCity(v.name);
    int idxV = cityHeap->isFound(v.name);
    int idxU = cityHeap->isFound(u.name);
    if(idxU != -1 && idxV != -1 && W != nullptr && v.d > (u.d + W->distance)){
        newCityWithNewDValue.d = u.d + W->distance;
        cityHeap->decreaseKey(idxV,newCityWithNewDValue);
        if (S->isFound(v.name) != -1) {
            S->decreaseKey(S->isFound(v.name),newCityWithNewDValue);
        }
        cityHeap->getDepCityArr()[cityHeap->isFound(v.name)].pi = &cityHeap->getDepCityArr()[cityHeap->isFound(u.name)];
    }
}

//Given a departure city's name, this function returns its index 
//inside the cityHeap, it will return -1 if it's not inside.
int Graph::findOneCity(string aCityName){
   return cityHeap->isFound(aCityName);
}

//Given a source city's name, this function performs Dijkstra's shortest
//path algorithm on the graph(based on the departure city's d value)
void Graph::dijkstra(string sourceCityName){
    //set the initial values given a source city
    initializeSingleSource(sourceCityName);

    //set up S to be a minPriority Queue that won't change the values in cityHeap
    DepCity* setS = cityHeap->getDepCityArr();
    S = new MinPriorityQueue(cityHeap->getCapacity());
    for(int i = 0; i < numOfNode; i++){
        S->insert(setS[i]);
    }

    //S is set to empty every time we run the alg
    //S = new MinPriorityQueue(cityHeap->getCapacity());

    DepCity U;
    DepCity Temp;
    DepCity* V;
    ArrCity* curr;
    while(S->getSize() > 0){
        Temp = S->getHeapMin();
        U = setS[cityHeap->isFound(Temp.name)];
        //S->insert(U);
        curr = U.arrCityList->getHead();
        while(curr != nullptr){
            V = arrToDep(*curr);
            relax(U,*V);
            curr = curr->next;
        }
        S->extractHeapMin();
    }
}

//Given a source city, this function print the result of running
//dijkstra's algorithm on the graph
void Graph::printDijkstraPath(string sourceCityName){
    dijkstra(sourceCityName);
    DepCity toPrint;
    DepCity* setPull = new DepCity[numOfNode];
    DepCity* load = cityHeap->getDepCityArr();
    MinPriorityQueue pull(cityHeap->getCapacity());

    // loading in values from 'S'
    for(int i = 0; i < numOfNode; i++){
        setPull[i] = load[i];
        pull.insert(load[i]);
    }

    cout << "\n\nPrint the Dijkstra algorithm running result" << endl;
    cout << "\nSource City: " << sourceCityName << "\n\n";
    cout << setw(14) << "Arrival City"                              //column title
        << setw(15) << "Shortest Dist."                             //column title
        << "Shortest Path  " << endl;                                  //column title

    //printing!
    while(pull.getSize() > 0){
        toPrint = pull.getHeapMin();
        int duplicates = getDuplicates(cityHeap->isFound(toPrint.name));

        //case 1 & 2: there exist distances duplicates from this source city that are causing output errors
        if(duplicates != -1) {
            //case 1:   the path doesn't exist so the duplicate is the original infinite value set for all nodes
            if (toPrint.d >= 10000) {
                for (int i = 0; i < numOfNode; i++) {
                    if (load[cityHeap->isFound(nameArr[i])].d == toPrint.d) {
                        pull.extractHeapMin();                          //this must happen after all checks
                        cout << setw(15) << nameArr[i];
                        printMid(toPrint);
                        printPath(sourceCityName, &cityHeap->getDepCityArr()[cityHeap->isFound(nameArr[i])]);
                        cout << endl;
                    }
                }
            }
            //case 2:   the path does exist, but dijkstra calculated more than one path with the same distance
            else {
                for(int i = 0; i < numOfNode; i++){
                    if(load[i].d == toPrint.d) {
                        pull.extractHeapMin();                          //this must happen after all checks
                        cout << setw(15) << load[i].name;
                        printMid(load[i]);
                        printPath(sourceCityName, &cityHeap->getDepCityArr()[cityHeap->isFound(load[i].name)]);
                        cout << endl;
                    }
                }
            }
        }
        //case 3: there are no distance duplicates from this source city
        else {
            pull.extractHeapMin();                                      //this must happen after all checks
            cout << setw(15) << toPrint.name;
            printMid(toPrint);
            printPath(sourceCityName, &toPrint);
            cout << endl;
        }
    }
}

//this finds the pi path from the source to the destination and prints it
void Graph::printPath(string sourceCityName, DepCity* aCity){
    if(sourceCityName == aCity->name){
        cout << sourceCityName;
    }
    else if(aCity->pi == nullptr){
        cout << "None exist";
    }
    else{
        printPath(sourceCityName, aCity->pi);
        cout << "->" << aCity->name;
    }
}

//used to resolve redundancy in printDijkstras path
void Graph::printMid(DepCity toPrint){
    if (toPrint.d >= 10000) {                             //if the path does not exist
        cout << setw(15) << "None exist";
    } else {                                              //if the path does exist
        cout << setw(15) << toPrint.d;
    }
}

//checks if for a given node if other cities have the same distance
//it's used to fix duplication errors in output
int Graph::getDuplicates(int idx){
    for(int i = 0; i < numOfNode; i++){
        if(i != idx && (cityHeap->getDepCityArr()[i].d == cityHeap->getDepCityArr()[idx].d)){
            return i;
        }
    }
    return -1;
}

//becasue of pointer issues, this is used to find the corresponding
//departure city of an arrival city name
DepCity* Graph::arrToDep(ArrCity arr){
    DepCity* check = cityHeap->getDepCityArr();
    DepCity* returnDepCity;
    int idx = cityHeap->isFound(arr.name);
    if(idx == -1){
        returnDepCity = nullptr;
    }
    else{
        returnDepCity = &check[idx];
    }
    return returnDepCity;
}