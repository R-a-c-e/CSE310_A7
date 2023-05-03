//********************************************************************
// ASU CSE310 Fall 2022 Assignment #7
// Name of Author:  Race Musgrave
// ASU ID:          1215151093
// Description: the ArrivalCityList.h file handles everything to do with
// the linked list of arrival cities that are linked to a given Departure
// city. We can search for and add cities, as well as print the cities
//********************************************************************

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

//struct ArrCity represents an arrival city information with
//distance info. from a specific departure city
struct ArrCity
{
    string name;
    int distance;
    struct ArrCity* next;
};

//class ArrivalCityList is a linked list of ArrCity
//class ArrivalCityList will contain a linked list of ArrCity
class ArrivalCityList
{
    private:
        struct ArrCity* head;

    public:
        ArrivalCityList();
        ~ArrivalCityList();
        ArrCity* getHead();
        ArrCity* findArrCity(string oneCity);
        bool addArrCity(string oneCity, int distance);
        void printArrCityList();
        void addFront(ArrCity* newCity);
        int getSize();
};

//*******************************************************************
//constructor
ArrivalCityList::ArrivalCityList(){
    head = nullptr;
}

//*******************************************************************
//Destructor
ArrivalCityList::~ArrivalCityList(){
    ArrCity * curr = getHead();
    while(curr != nullptr){
        head = head->next;
        delete curr;
        curr = head;
    }
    head = nullptr;
    curr = nullptr;
}

//*******************************************************************
//returns the head of the linked list ArrivalCityList with Nodes of type ArrCity
ArrCity* ArrivalCityList::getHead(){
    return head;
}

//*******************************************************************
//ArrCity* findArrCity(string oneCity);
//A function to find whether the parameterized city is inside the LinkedList or not.
//returns a pointer that points to the city if it exists, otherwise return NULL
ArrCity* ArrivalCityList::findArrCity(string oneCity){
    ArrCity* curr = getHead();
    while(curr != nullptr){
        if(curr->name == oneCity){
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}


//*******************************************************************
//bool addArrCity(string oneCity, int distance);
//Creates a new ArrCity and inserts it into the list at the right place.
//It also maintains an alphabetical ordering of the ArrCities while inserting
//it into the list. Note: you cannot insert duplicated ArrCity
//Return value: true if it is successfully inserted and false in case of failures.
bool ArrivalCityList::addArrCity(string oneCity, int distance){
    if(findArrCity(oneCity) != nullptr){
        cout << "city already exists in the list\n";
        return false;
    }

    //if the city does not yet exist we create the new city node
    ArrCity* prev = nullptr;
    ArrCity* curr = getHead();
    ArrCity* newCity = new ArrCity;
    newCity->name = oneCity;
    newCity->distance = distance;

    //if the list is empty, makes the newCity the head
    if(curr == nullptr){
        //cout << "\n" << newCity->name << " was added successfully to an empty ist" << endl;
        addFront(newCity);
        return true;
    }

    while(curr != nullptr){
        //adds in the front if theres only one element in the list
        if(newCity->name < curr->name && prev == nullptr){
            addFront(newCity);
            //cout << "\n" << newCity->name << " was added successfully added at the front, in front of " << curr->name << endl;
            return true;
        }

        //adds in the back if theres only one element in the list
        if(newCity->name < curr->name && prev == nullptr && curr->next == nullptr){
            //cout << "\n" << newCity->name << " was added successfully added at the back, behind " << curr->name << endl;
            curr->next = newCity;
            return true;
        }
        
        //adds in the middle
        else if(newCity->name < curr->name && newCity->name > prev->name){
            newCity->next = curr;
            prev->next = newCity;
            //cout << "\n" << newCity->name << " was added successfully added between" << prev->name << " and " << curr->name << endl;
            return true;
        }
        
        //adds at the end
        else if(newCity->name > curr->name && curr->next == nullptr){
            curr->next = newCity;
            //cout << "\n" << newCity->name << " was added successfully added at the back, behind " << curr->name << endl;
            return true;
        }
        
        //if the city wasnt added, we iterate and try again
        //if(newCity->name > curr->name && newCity->name > prev->name){
            prev = curr;
            curr = curr->next;
        //}
    }
    return false;
}

//reduces redundancy in add arrCity; adds to the front of a linked list
void ArrivalCityList::addFront(ArrCity* newCity){
    newCity->next = head;
    head = newCity;
}

//standard print of a linked list of arrival cities belonging to a departure city
void ArrivalCityList::printArrCityList(){
    ArrCity* curr = getHead();
    if(curr == nullptr){
        cout << "Arrival city list is empty";
    }
    while(curr != nullptr){
        cout << curr->name << "(" << curr->distance << "),";
        curr = curr->next;
    }
}

//iterates through the linked list and returns the number of noted
int ArrivalCityList::getSize(){
    ArrCity* curr = getHead();
    int count = 0;
    while(curr != nullptr){
        count++;
        curr = curr->next;
    }
    return count;
}