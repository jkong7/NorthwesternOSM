// busstop.cpp
// Constructor file for the BusStop class 

#include "busstop.h"
#include <iostream> 

// Contructor 
BusStop::BusStop(int stopID, int route, const string& stopName, const string& direction,
                 const string& location, double lat, double lon)
    : stopID(stopID), route(route), stopName(stopName), direction(direction),
      location(location), lat(lat), lon(lon) {
}


// print
// Operation: Prints out all the bus stop information for one BusStop object
// Parameters: None
// Returns: void
void BusStop::print() const {
    cout << stopID << ": bus " << route << ", " << stopName << ", " << direction << ", " << location << ", "
    << "location (" << lat << ", " << lon << ")" << endl; 
}
