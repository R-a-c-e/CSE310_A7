//********************************************************************
// ASU CSE310 Fall 2022 Assignment #7
// Name of Author:  Race Musgrave
// ASU ID:          1215151093
// Description: The driver file for this program. It gets from the user
// the number of departure cities and their information. It then asks
// the suer which city will be the source. It does this until the user
// return 'no' at which point the program terminates.
//********************************************************************

#include "Graph.h"

using namespace std;

void getCityInfo(string oneLine, string& depCityName, ArrivalCityList* arrCityList);
void getArrCityInfo(string oneArrCityInfo, string& arrCityName, int& distance);     //**optional

int main()
{
    int count;       //a variable for number of nodes
    string oneLine, sourceCityName;
    string answer = "yes";       //a variable to decide whether to run Dijkstra multiple times

    //cout << "Enter number of cities: ";
    cin >> count;
    cin.ignore(20, '\n');

    //For each line of the file, we will create an ArrivalCityList(which is a linked list)
    //for a specific departure city. It represents all its reachable arrival cities with distance info.
    //Here we use an array of DepCity to store them
    DepCity* depCityArr = new DepCity[count];
    MinPriorityQueue* queue = new MinPriorityQueue(count);


    //initialize each arrival city list
    for(int i = 0; i< count; i++){
        depCityArr[i].d = 10000 + i;
        depCityArr[i].arrCityList = new ArrivalCityList();
    }

    //get input line by line and create the depCityArr
    cout << endl;
    for(int i = 0; i< count; i++){
        getline(cin, oneLine);
      
        getCityInfo(oneLine,depCityArr[i].name,depCityArr[i].arrCityList);
    }
    for(int i = 0; i < count; i++){
        queue->insert(depCityArr[i]);
    }

    //create a graph object
    Graph* graph = new Graph(count, queue);

    //print the graph adjacency list before running Dijkstra algorithm
    cout << "\nPrint the graph adjacency list before running Dijkstra algorithm" << endl;
    graph->printGraph();

    // iterates until the user enters 'no'. While 'yes' it gets a new source
    // city form the user and prints all the paths.
    while(answer == "yes"){
        cout << "\nEnter source city name: \n";
        getline(cin, sourceCityName);
        if(queue->isFound(sourceCityName) == -1){
            cout << sourceCityName <<" does NOT exist\n";
        }
        else graph->printDijkstraPath(sourceCityName);
        cout << "\nFind shortest path for another departure city(yes or no): \n";
        getline(cin, answer);
    }
    cout << "Program terminate" << endl;
}

//************************************************************************
//Give one line in input file, this function extract tokens and get departure city
//name and its arrival cities info.
void getCityInfo(string line, string& depCityName, ArrivalCityList* arrCityList){

    //set the depCity name
    string delim = ",";
    int idx = line.find(delim);
    depCityName = line.substr(0,idx);
    line.erase(0, 1);

    //checking if the Departure city has no arrival cities
    if(line == ",,"){
        arrCityList = nullptr;
    }

    //loop until all the arrival cities are added
    else{
        line.erase(0, 1);
        while(line != "\n" && !line.empty() && line != "?"){
            idx = line.find(delim);
            string arrCity;
            int distance;

            getArrCityInfo(line.substr(0,idx), arrCity, distance);
            arrCityList->addArrCity(arrCity,distance);
            line.erase(0, idx+delim.length());
        }
    }
}

//****************************************************************************
//Given one arrival city info. format of, such as Z(60), this function
//extract arrival city name 'Z' and distance 60 from it.
//this function is optional. Feel free to extract tokens using your own way
void getArrCityInfo(string info, string& arrCityName, int& distance){
    //set the arrCity name
    string delim = "(";
    int idx = info.find(delim);
    arrCityName = info.substr(0,idx);
    info.erase(0, idx+delim.length());

    //set the distance value
    delim = ")";
    idx = info.find(delim);
    string strdis = info.substr(0,idx);
    distance = stoi(info.substr(0,idx));
}