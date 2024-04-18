//********************************************************************
// ASU CSE310 Spring 2024 Assignment #7
// Name of Author: Nilay Kumar
// ASU ID: 1225127891
// Description: //---- is where you should add your own code
//********************************************************************

//include necessary header file here
//----
#include "ArrivalList.h"

using namespace std;

//represent each node of the graph which is a departure location
//with all the relevant info.
struct Departure
{
    string depAddress;
    double d;
    struct Departure* pi;
    ArrivalList* arrList;
};

//class MinPriorityQueue represents a Min-heap that contains Departure objects.
class MinPriorityQueue
{
    private:
        struct Departure* departureArr;	//a Departure array
        int capacity, size;

	public:
        MinPriorityQueue(int capacity);
        ~MinPriorityQueue();

        Departure* getDepartureArr();
        int getSize();
        int getCapacity();
        int isFound(string oneDepAddress);
        bool decreaseKey(int index, Departure oneDepartureWithNewDValue);
        bool insert(Departure oneDeparture);
        void heapify(int index);
        Departure getHeapMin();
        void extractHeapMin();
        int leftChild(int parentIndex);
        int rightChild(int parentIndex);
        int parent(int childIndex);
        void printHeap();
        void build_min_heap();

        //add other function definitions here if 100% necessary
        //----
};


//*********************************************************
void MinPriorityQueue::printHeap()
{

		cout << "\nHeap size = " << getSize() << "\n" << endl;

		cout << left;
		cout << setw(15) << "Departure"
             << setw(12) << "d Value"
             << setw(15) << "PI"
             << "Arrival List" << endl;
        //----
        //----

        //we have to go through and print each element in the heap
        for (int i = 0; i < size; i++) {
            Departure current = departureArr[i];            //set current to know which node we are on
            cout << setw(15) << current.depAddress          //prints depAddress
                 << setw(15) << fixed << setprecision(2) << current.d;          //prints the shortest path

            //now we have to print the predecessor, or the pi
            if (current.pi != nullptr) {
                cout << setw(15) << current.pi->depAddress;
            }else {
                cout << setw(15) << "NULL";
            }

            current.arrList->printArrivalList();
        }
}

//************************************************************
//when you create the dummy node, initialize its d value to be
//50000 which represents infinite large
bool MinPriorityQueue::insert(Departure oneDeparture)
{
    //----
    //----
    //if the queue is full, lets increase the size to hold more items
    if (size == capacity) {
        int newCap = 2*capacity;

        Departure* newDepartureArr = new Departure[newCap];     //make a new departure array with the new capacity created
        //since we just created a new array, we have to copy the elements from the old array to the new one
        for (int i =0; i < size; i++) {
            newDepartureArr[i] = departureArr[i];           //copies the elements over
        }

        //now we don't need the old array, so we can delete it
        delete[] departureArr;

        //lets update the pointers for the new array
        departureArr = newDepartureArr;
        

        //and then update the capacity;
        capacity = newCap;
    }

    //now lets actually add the element to the end of the array
    departureArr[size] = oneDeparture;
    size++;         //increase the size because we just added an element

    //maintain the min-heap property by moving it to the correct position
    int index = size -1;
    while (index > 0 && departureArr[parent(index)].d > departureArr[index].d) {        //if the condition is met, then it needs to swap because it violates the min heap property
        swap(departureArr[index], departureArr[parent(index)]);     //swaps the elements
        index = parent(index);      //updates the index location
    }
    return true;        //we are finally finished with insertion

}

//****************************************************************************
//Given the original departureArr, the function build a min-heap by using d value
void MinPriorityQueue::build_min_heap()
{
    //----
    //----

    //lets start from the last node that isn't a leaf node and call heapify on all the subtrees
    for (int i = (size / 2) - 1; i >= 0; i++) {     //we used the formula to get that node to start from there
        heapify(i);
    }
}

//According to above class definition, define all other functions accordingly
//----
//----

//lets start by making the constructor
MinPriorityQueue::MinPriorityQueue(int initialCapacity) {

    capacity = initialCapacity;                     //we just have to set all the private variables to default values
    size = 0;                                       //set to default values 0f 0
    departureArr = new Departure[initialCapacity];          //set departureArr in a new "node"
}

//next lets make the destructor
MinPriorityQueue::~MinPriorityQueue() {

    delete[] departureArr;          //just delete for now, come back later if it doesn't work, or if i need a count of everything it deleted
}


Departure* MinPriorityQueue::getDepartureArr() {
    return departureArr;                //just return the correct variable that it's asking for
}


int MinPriorityQueue::getSize() {
    return size;                //just return size
}

int MinPriorityQueue::getCapacity() {
    return capacity;            //just return capacity
}

int MinPriorityQueue::isFound(string oneDepAddress) {
    //lets loop through the list and compare values to see if we come across the correct value, and return true if that happens
    for (int i = 0; i < size; i++) {
        if (departureArr[i].depAddress == oneDepAddress) {
            return 1;        //means it found the address
        }
    }
    //if the code reaches this area, that means that the correct address wasn't in fact found so then it should return false
    return 0;
}


bool MinPriorityQueue::decreaseKey(int index, Departure oneDepartureWithNewDValue) {


    //lets make sure the index isn't out of bounds
    if (index < 0 &&  index >= size) {
        cout << "Invalid Index" << endl;
        return false;
    }

    //update d which is the shortest path, and pi which is the predecessor for the departure address
    departureArr[index].d = oneDepartureWithNewDValue.d;
    departureArr[index].pi = oneDepartureWithNewDValue.pi;

    while (index > 0 && departureArr[parent(index)].d > departureArr[index].d) {
        //swap the departure address with it's parent
        swap(departureArr[index], departureArr[parent(index)]);     //use the swap function to make it easier
        index = parent(index);      //move up the index
    }


    return true;
}

void MinPriorityQueue::heapify(int index) {

    int smallest = index;       //initialize the smallest element as the root because that should be the property of the min priority queue
    int left = leftChild(index);
    int right = rightChild(index);

    //find the smallest element among the root, check left and right
    if (left < size && departureArr[left].d < departureArr[smallest].d) {       //check left and replcae
        smallest = left;
    }
    if (right < size && departureArr[right].d < departureArr[smallest].d) {     //check right, and replace
        smallest = right;
    }

    //if the smallest element is not the root, swap it with the root and recursively do the same thing

    if (smallest != index) {
        swap (departureArr[index], departureArr[smallest]);     //swap if they're the same
        heapify(smallest);              //recursively call the function again
    }

}

Departure MinPriorityQueue::getHeapMin() {

    
    return departureArr[0];
}

void MinPriorityQueue::extractHeapMin() {

    //if the heap is empty, then don't do anything, just return
    if (size == 0)
    {
        cout << "Priority queue is empty." << endl;
        return; 
    }

    // replace the root with last element
    departureArr[0] = departureArr[size - 1];

    // decrease size of the heap since we are taking away a node
    size--;

    // restore the min-heap property by calling heapify
    heapify(0);
}

int MinPriorityQueue::leftChild(int parentIndex) {
    //lets just plug in the formula for index of left child in the textbook
    return (2* parentIndex) +1;
}

int MinPriorityQueue::rightChild(int parentIndex) {
    //once again, we can just plug in the formula in the textbook
    return (2* parentIndex) +2;
}

int MinPriorityQueue::parent(int childIndex) {
    //can also use a formula here to get the parent index in the textbook
    return (childIndex -1) /2;
}





