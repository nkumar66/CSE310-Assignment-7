//********************************************************************
// ASU CSE310 Spring 2024 Assignment #7
// Name: Nilay Kumar
// ASU ID:
// Description: //---- is where you should add your own code
//********************************************************************

//include necessary header file here
//----
#include "Graph.h"
#include <sstream>

using namespace std;

void getDepartureInfo(string oneLine, string& depAddress, ArrivalList* arrList);
void getArrivalInfo(string oneArrivalInfo, string& arrAddress, double& distance, string& roadCategory); //**optional

int main()
{
   int count;       //a variable for number of nodes
   string oneLine, sourceDepAddress;
   string answer = "yes";       //a variable to decide whether to run Dijkstra multiple times or not

   cout << "Enter number of departure address: ";
   cin >> count;
   cin.ignore(20, '\n');

   //Create an array called 'departureArr' that holds 'count' number of Departure objects
   //----
   Departure* departureArr = new Departure[count];    //created with size count

   //Initialize departureArr. Note: you will need to initialize each of the instance variable
   //including arrival list
   for(int i = 0; i < count; i++)
   {
      departureArr[i].d = 10000.0 + i;  //this is to make sure each node has different d value
      //----
      //----
      departureArr[i].depAddress = "";    //this is to initialize the address, we can fill it in later
      departureArr[i].d = 10000 + i;      //to make sure each node has a different value we are going to add the iteration number to it to make it unique
      departureArr[i].pi = nullptr;       //initialize the predecessor pointer
      departureArr[i].arrList = new ArrivalList();       //make sure to make new arrival List, this is why I was getting seg fault.
   }

   //get input line by line and create the departureArr
   for(int i = 0; i< count; i++)
   {
      getline(cin, oneLine);
      //----
      //----

      string depAddress;
      //creates a departure array to keep track
      ArrivalList* arrList = departureArr[i].arrList;
      getDepartureInfo(oneLine,depAddress,arrList);
      departureArr[i].depAddress = depAddress;
   }

  //create a Graph object
  //----
  MinPriorityQueue* minPQ = new MinPriorityQueue(count); //create a new min priority queue object to create the graph object
  Graph graph(count, minPQ);     //create the graph object

   //print the graph adjacency list before running Dijkstra algorithm
   cout << "\nPrint the graph adjacency list before running Dijkstra algorithm" << endl;
   //----
   graph.printGraph();

   //Next you need to design a sentinel-value controlled while loop base on 'answer'
   //If user answer is 'yes', get departure address, run Dijkstra algorithm, print
   //shortest traveling time and path, then ask the user whether he want to continue or not...

   //Look back in previous assignments and use similar pattern to take yes or no from the user

   while (answer == "yes") {
      cout << "\nEnter departure address";
      cin >> sourceDepAddress;

      //Check if the source departure address exists
      int sourceIndex = -1;
      for (int i =0; i < count; i++) {
         if (departureArr[i].depAddress == sourceDepAddress) {
            sourceIndex = i;
            break;
         }
      }

      if (sourceIndex != -1) {
         graph.dijkstra(sourceDepAddress);
         graph.printDijkstraPath(sourceDepAddress);
      } else {
         cout << sourceDepAddress << "Does not Exist" << endl;
      }

      //determine whether they want to run again, and then take the answer and loop again if need to
      cout << "\nFind shortest path for another departure address (yes or no): ";
      cin >> answer;

   }

   
   delete[] departureArr;
   
   delete minPQ;

   return 0;

}

//********************************************************************************
//Give one line in input file, this function extract tokens and get departure address
//and all arrival info.
void getDepartureInfo(string oneLine, string& depAddress, ArrivalList* arrList)
{
   //----
   //----
   //extract the departure address
   int pos = oneLine.find(',');
   depAddress = oneLine.substr(0, pos);

   //extract arrival info for each arrival address 
   string arrivalInfo = oneLine.substr(pos + 1);
   while (!arrivalInfo.empty()) {

      pos = arrivalInfo.find(',');     //find the delimiter
      string oneArrivalInfo = arrivalInfo.substr(0, pos); //extracts the next section of input

      string arrAddress;      //these 3 variables are for calling the getArrivalInfo
      double distance;
      string roadCategory;
      getArrivalInfo(oneArrivalInfo, arrAddress, distance, roadCategory);  //call getArrivalInfo
      arrList->addArrival(arrAddress, distance, roadCategory);   //add the arrival to the array list
      arrivalInfo = arrivalInfo.substr(pos + 1);         //get the arrivalInfo
   }

}

//******************************************************************************************
//Given such as Y(1803.2/I), this function extracts arrival Address "Y", distance 1803.2
//and roadCategory "I" info. out
void getArrivalInfo(string oneArrivalInfo, string& arrAddress, double& distance, string& roadCategory)
{
   //----
   //----

   //extract the arrival address first
   arrAddress = oneArrivalInfo.substr(0, oneArrivalInfo.find('('));     //the parenthesis means an address is coming

   //now lets extract the distance and road category
   int start = oneArrivalInfo.find('(') + 1;    //find the parenthesis
   int end = oneArrivalInfo.find('/');          //find the slash

   //get the distance, and put it into the distance variable
   string distanceString = oneArrivalInfo.substr(start, end - start);
   
   //now lets convert to double using sstream
   istringstream iss(distanceString);
   iss >> distance;
   
   roadCategory = oneArrivalInfo.substr(end+ 1, oneArrivalInfo.size() - end - 2);   //this excludes the ( and the )


}
