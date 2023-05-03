//********************************************************************
// ASU CSE310 Fall 2022 Assignment #7
// Name of Author:  Race Musgrave
// ASU ID:          1215151093
// Description: this file handles all functions that update a minPriorityQueue
// which is implemented as a minHeap. We can create a min heap from an array,
// search the minHeap for a given node, add to it, and remove its minimum value.
// All other functions are used to aid in these.
//********************************************************************

#include "ArrivalCityList.h"

using namespace std;

//represent each node of the graph which is a departure City
struct DepCity
{
    string name;
    int d;
    struct DepCity* pi;
    ArrivalCityList* arrCityList;
};

// class MinPriorityQueue represents a Min-heap that contains DepCity objects.
// The key of each DepCity is its d value.
class MinPriorityQueue
{
    private:
        struct DepCity* depCityArr;	        //a DepCity array
        int capacity, size;

	public:
        MinPriorityQueue(int capacity);
        ~MinPriorityQueue();

        DepCity* getDepCityArr();
        int getSize();
        int getCapacity();
        int isFound(string oneDepCityName);
        bool decreaseKey(int index, DepCity oneDepCityWithNewDValue);
        bool insert(DepCity oneDepCity);
        void heapify(int index);
        DepCity getHeapMin();
        void extractHeapMin();
        int leftChild(int parentIndex);
        int rightChild(int parentIndex);
        int parent(int childIndex);
        void printHeap();
        void buildMinHeap();
};


//constructor
MinPriorityQueue::MinPriorityQueue(int capacity){
    depCityArr =  new DepCity[capacity];
    this->capacity = capacity;
    size = 0;
}

//destructor
MinPriorityQueue::~MinPriorityQueue(){
    delete[] depCityArr;
    depCityArr = NULL;
}

//prints each departure city with their respective arrival cities
void MinPriorityQueue::printHeap()
{
		cout << "\nHeap size = " << getSize() << "\n" << endl;

		cout << left;
		cout << setw(15) << "Dep. City"
             << setw(12) << "d Value"
             << setw(15) << "PI"
             << "Arrival City List" << endl;
        for(int i = 0; i < getSize(); i++){
            cout << left;
		    cout << setw(15) << getDepCityArr()[i].name
                 << setw(12) << getDepCityArr()[i].d
                 << setw(15) << "No Parent";
                 getDepCityArr()[i].arrCityList->printArrCityList();
            cout <<  endl;
        }
}

//returns a pointer to Departure city array
DepCity* MinPriorityQueue::getDepCityArr(){
    return depCityArr;
}

//returns the current size of the min heap
int MinPriorityQueue::getSize(){
    return size;
}

//returns the capacity of the min heap
int MinPriorityQueue::getCapacity(){
    return capacity;
}

//searches for a city, if found returns the index, if not, -1
int MinPriorityQueue::isFound(string oneDepCityName){
    for(int i = 0; i < getSize(); i++){
        if(depCityArr[i].name == oneDepCityName){
            return i;
        }
    }
    return -1;
}

//decreases the distance of a given node and then moves it to the correct place
bool MinPriorityQueue::decreaseKey(int index, DepCity oneDepCityWithNewDValue){
    if (depCityArr[index].d < oneDepCityWithNewDValue.d){
        return false;
    }

    depCityArr[index].d = oneDepCityWithNewDValue.d ;                                   //the city at index's d = to the d in the new city
    depCityArr[index].pi = oneDepCityWithNewDValue.pi;

    while(index > 0 && depCityArr[parent(index)].d > depCityArr[index].d){              //this while loop swaps the child node into the parent node
        DepCity temp = depCityArr[index];                                               //until the child's d is smaller than its parent's d.
        depCityArr[index] = depCityArr[parent(index)];
        depCityArr[parent(index)] = temp;
        index = parent(index);
    }
    return true;
}

//a new departure city object is created and then added
bool MinPriorityQueue::insert(DepCity oneDepCity){
    DepCity dHolder;
    dHolder = oneDepCity;
    oneDepCity.d = 30000;
    size++;
    depCityArr[getSize() - 1] = oneDepCity;
    bool success = decreaseKey(getSize() - 1, dHolder);
    if(success) return true;
    return false;
}

//from a given index, we create a min heap down the tree
void MinPriorityQueue::heapify(int index){
    if(getSize() > 0) {
        int smallest;
        int lChild = leftChild(index);
        int rChild = rightChild(index);
        //if the left child is smaller than its parent, we set it to the smallest
        if (lChild < getSize() && depCityArr[lChild].d < depCityArr[index].d) {
            smallest = lChild;
        } else smallest = index;
        //if the right child is smaller than the smallest, we set it to the smallest
        if (lChild < getSize() && depCityArr[rChild].d < depCityArr[smallest].d) {
            smallest = rChild;
        }
        //if the parent is greater than either child, recursively call to fix it
        if (smallest != index) {
            DepCity temp = depCityArr[index];
            depCityArr[index] = depCityArr[smallest];
            depCityArr[smallest] = temp;
            heapify(smallest);
        }
    }
}

//returns the node with the shortest distance
DepCity MinPriorityQueue::getHeapMin(){
    return depCityArr[0];
}

//removes the smallest element by replacing the first 
//element with the last and then fixes its position
void MinPriorityQueue::extractHeapMin(){
    depCityArr[0] = depCityArr[getSize()-1];
    size--;
    heapify(0);
}

//returns the index of a parent's left child
int MinPriorityQueue::leftChild(int parentIndex){
    if(parentIndex == 0){
        return 1;
    }
    return parentIndex * 2 + 1;
}

//returns the index of a parent's right child
int MinPriorityQueue::rightChild(int parentIndex){
    if(parentIndex == 0){
        return 2;
    }
    return parentIndex * 2 + 2;
}

//returns the index of a child's parent
int MinPriorityQueue::parent(int childIndex){
    if(childIndex % 2 == 1){
        childIndex++;
    }
    return childIndex / 2 - 1;
}

//builds the depCityArr into a minheap
void MinPriorityQueue::buildMinHeap(){
    for(int i = getSize()/2; i > 0; i--){
        heapify(i);
    }
}