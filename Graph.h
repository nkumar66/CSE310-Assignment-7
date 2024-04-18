//********************************************************************
// ASU CSE310 Spring 2024 Assignment #7
// Name of Author: Nilay Kumar
// ASU ID: 
// Description: This is the header file that defines a weighted directed graph
//********************************************************************

//include necessary header file here
//----
#include "MinPriorityQueue.h"
#include <cfloat>

using namespace std;

class Graph
{
   private:
   int numOfNode;
   MinPriorityQueue* departureHeap;     //adjacency list of the graph, it is a min-heap
                                        //of Departure objects based on d value
   public:
   Graph(int numOfNode, MinPriorityQueue* departureHeap);
   ~Graph();
   MinPriorityQueue* getDepartureHeap();
   void printGraph();
   void initialize_single_source(string sourceDepAddress);
   void relax(Departure u, Departure v);
   int findOneDeparture(string aDepAddress);
   void dijkstra(string sourceDepAddress);
   void printDijkstraPath(string sourceDepAddress);

   //add any auxiliary functions here in case you need them
   //----
};

//*******************************************************************
//Constructor
Graph::Graph(int numOfNode, MinPriorityQueue* departureHeap)
{
   //----
   //----
   //set up the private variables of the class
   this->numOfNode = numOfNode;
   this->departureHeap = departureHeap;

}

//*******************************************************************
//Destructor Graph::~Graph()
Graph::~Graph()
{
   //----
   //----
   //we just need to call delete to delete the heap
   delete departureHeap;
}

//*******************************************************************
void Graph::printGraph() {
//This function prints the graph. It traverse through the vertex list,
//then for each vertex, it print its adjacent list from head to tail.

   //for this function, we can just looop through each vertex in the departure heap
   for (int i = 0; i < numOfNode; i++) {

      //gets the current departure location, which is basically the first one in the node bceause i starts at 0
      Departure current = departureHeap->getDepartureArr()[i];

      //print the information about it
      cout << "Departure Address: " << current.depAddress << endl;

      //call function we already defined back in arrival list to print the arrival list
      current.arrList->printArrivalList();

   }

}

//According to above class definition, define all other functions accordingly
//----
//----

void Graph::initialize_single_source(string sourceDepAddress) {

   //we need to find the index to source the departure address from
   int source = findOneDeparture(sourceDepAddress);

   if (source == -1) {     //this will mean that it didn't find the correct value and therefore doesn't exist
      cout << "DOES NOT EXIST" << endl;
      return;
   }

   //lets set the shortest path distance to 0 
   departureHeap->getDepartureArr()[source].d = 0;
   
   //do the same for the parent (pi), except set it to nullptr because we don't know
   departureHeap->getDepartureArr()[source].pi = nullptr;

}

void Graph::relax(Departure u, Departure v) {

   //calculate the weight of the edge from u to v
   double weight = v.d;

   //now lets check if the shortpest path can be improved by going through u, and if it does, we should update the properties of v
   if (v.d > u.d+ weight) {
      v.d = u.d + weight;     //update the shortest path for v
      v.pi = &u;     //update the predecessor of v
   }

   //that is all the pseudocode that was given for this method
}

int Graph::findOneDeparture(string aDepAddress) {

   //we should iterate through each node in the departures, and find if the departure address matches
   //if it does match, then we should return the index, because that's what the method description states

   for (int i = 0; i < numOfNode; i++) {
      
      Departure current = departureHeap->getDepartureArr()[i];       //set current to loop through

      if (current.depAddress == aDepAddress) {     //check if they match
         return i;   //return the index
      }

   }
   //return -1 if the index isn't found
   return -1;
}


void Graph::dijkstra(string sourceDepAddress) {

   //lets start by initializing a single source with given sourceDepAddress
   initialize_single_source(sourceDepAddress);

   //create a min priority queue to store vertices
   MinPriorityQueue minPQ(numOfNode);
   
   //now lets add all of the vertices to the queue
   for (int i =0; i < numOfNode; i++) {
      minPQ.insert(departureHeap->getDepartureArr()[i]);
   }

   //now we have to process all the vertices until the minPriorityqueue is empty
   while (true) {
      if (minPQ.getSize() == 0) {      //lets make sure it's not empty, and if it is, then we will stop 
         break;
      }

      Departure u = minPQ.getHeapMin();      //get the root, which is the smallest value

      //now let's process the information from, and get it's adjacent vertices
      Arrival* currentArrival = u.arrList->getHead();
      //now lets loop 
      while (currentArrival != nullptr) {    //loop through each adjacent vertex

         //Find the vertex corresponding to current arrival address and get the pointer to it
         Departure* v = &(departureHeap->getDepartureArr()[findOneDeparture(currentArrival->arrAddress)]);

         //call relax as per pseudocode
         relax(u, *v);
         
         //now lets check if the vertex v is still in the priority queue
         if (minPQ.isFound(v->depAddress)) {

            //if yes, find the index for it
            int index = findOneDeparture(v->depAddress);

            //then decrease the key of v
            minPQ.decreaseKey(index, *v);
         }
         //keep iterating through
         currentArrival = currentArrival->next;
      }

      minPQ.extractHeapMin();
   }
}

void Graph::printDijkstraPath(string sourceDepAddress) {

   //use the format to make columns to print out the information headers
   cout << "Departure address: " << sourceDepAddress << endl << endl;
   cout << left << setw(15) << "Arrival" << setw(20) << "Shortest Time (h)" << "Shortest Path" << endl;


   //now lets loop through the vertices in the graph
   for (int i = 0; i < numOfNode; i++) {
      //let's get the departure array for each departure, loop through using current
      Departure current = departureHeap->getDepartureArr()[i];

      cout << setw(15) << current.depAddress;

      //now print the shortest time and path for current vertex
      double max = DBL_MAX;
      if (current.d == max) {      //DBL_MAX is an extremely large number default included, it will represent Infinity in this case
         cout << setw(20) << "No Path";      //since the number is infinity, it has not been visited yet or has nothing leading to the node
         cout << "No Path" << endl;
      }
      else {
         cout << setw(20) << fixed << setprecision(2) << current.d;

         //print the shortest path now
         Departure* predecessor = current.pi;

         string shortestPath = current.depAddress;    //depAddress is going to be shortestPath

         //while the predecessor exists, we will keep adding to the shortest path, and then finally print it out
         while (predecessor != nullptr) {

            shortestPath = predecessor->depAddress + " -> " + shortestPath; //keep padding to the path
            predecessor = predecessor->pi;         //keep traversing
         }

         cout << shortestPath << endl;    //print final result;
      }
   }
}

   




   



