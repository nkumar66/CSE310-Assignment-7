//********************************************************************
// ASU CSE310 Spring 2024 Assignment #7
// Name of Author: Nilay Kumar
// ASU ID: 
// Description: //---- is where you should add your own code
//********************************************************************

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

//Arrival represents an arrival address with distance and road
//category info. from a specific departure address
struct Arrival
{
    string arrAddress;
    double distance;        //miles from the departure address to this arrival address
    string roadCategory;    //Interstates, Arterials, Collectors or Local streets
    struct Arrival* next;
};

//class ArrivalList contains a linked list of Arrival objects
class ArrivalList
{
    private:
        struct Arrival* head;

    public:
        ArrivalList();
        ~ArrivalList();
        Arrival* getHead();
        Arrival* findArrival(string oneAddress);
        bool addArrival(string oneAddress, double distance, string roadCategory);
        void printArrivalList();
};


//*****************************************************************************************************
//Destructor. Before termination, the destructor is called to free the associated memory occupied by the
//existing linked list. It deletes all the Arrivals including the head and finally prints the number of
//Arrival deleted by it.
//----
//----

ArrivalList::~ArrivalList() {

    Arrival* current = head;    //lets start by creating a variable that we can use to traverse through the linkedlist
    Arrival* next;      //to use during the while loop

    int addressCount = 0;          //this is for being able to print the number of Arrivals deleted by it

    while (current != nullptr) {
        next = current->next;       //save to use after deleting the current node
        delete current;         //delete current node
        current = next;         //move to next elemnt
        addressCount++;                //to increase the count so that we can print it out at the end
    }

    head = nullptr;     //after all the nodes are deleted, we need to make sure that the head is nullptr, because that means list is empty


    //lastly print out the information, in format that is given to us
    cout<< "The number of deleted arrival addresses are: " << addressCount <<"\n";

}


//According to above class definition, define all functions accordingly
//----
//----

ArrivalList::ArrivalList() {

    //for the constructor we just need to initialize the private variables
    head = nullptr;
}

Arrival* ArrivalList::getHead() {

    //just need to return the head of the linked list
    return head;
}

Arrival* ArrivalList::findArrival(string oneAddress) {

    Arrival* current = head;        //set a variable to be able to traverse through the linkedlist

    //lets check if the list is empty first
    if (head == nullptr) {
        return nullptr;         //return nullptr if so
    }

    while (current != nullptr) {
        //lets check if the address matches
        if (current->arrAddress == oneAddress) {
            return current;             //return if match
        }
        current = current->next;        //keep traversing otherwise
        
    }

    //if we reach this area of the code, that means it wasn't found, so lets return a nullptr
    return nullptr;
}


bool ArrivalList::addArrival(string oneAddress, double distance, string roadCategory) {

    //lets check to make sure it doesn't already exist
    if (findArrival(oneAddress) != nullptr) {
        return false;
    }

    //lets create a new node first
    Arrival* newNode = new Arrival;

    if (newNode == nullptr) {       //to prevent seg fault
        cout << "ERROR" << endl;
        return false;
    }


    newNode->arrAddress = oneAddress;           //set all the values of the newNode to the values that are passed in
    newNode->distance = distance;               //same as above
    newNode->roadCategory = roadCategory;       //same as above
    newNode->next = nullptr;                    //set to nullptr because we don't know what next is yet

    //lets check if the head is empty to see if it needs to be the head
    if (head == nullptr || oneAddress < head->arrAddress) {
        newNode->next = head;
        head = newNode;
        return true;            //replace head with newNode if the head is nullptr, then return because it's successful
    }


    //we check the case where the address actually needs to go before the head, so that means this address replaces the head
    if (oneAddress < head->arrAddress) {

        newNode->next = head;
        head = newNode;
        return true;            //added before the head
    }

    //set this variable to traverse to insert
    Arrival* current = head;

    while (current != nullptr && current->next != nullptr && oneAddress > current->next->arrAddress) {            //make the insertion keep traversing until it reaches insertion point
        current = current->next;        //it will stop once it reaches where it needs to insert
    }

    //update list by actually inserting the newNode and updating the other pointers

    newNode->next = current->next;
    current->next = newNode;

    return true;            //operation is finished

}


//Prints all the arrival address in the linked list starting from the head.
void ArrivalList::printArrivalList()
{
    //----
    //----

    Arrival* temp = head;       //temporary pointer to use to traverse the list, same as current, but temp is used in the format for cout given to us


    while (temp != nullptr)
    {
		cout << temp->arrAddress                //we can use this premade block that is given to us, just needed to add the loop 
             << fixed << setprecision(2) << "(" << temp->distance << "/" << temp->roadCategory << "),";
		temp = temp->next;
	}
	cout <<"\n";
}
