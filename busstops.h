// busstops.h 
// Manages a vector of BusStop objects, the constructor parses busstop file and populates this vector 
// with BusStop objects. Provides methods to access the number of bus stops, print the bus stop information, 
// and find the closest southbound and northbound bus stops relative to a given location.

#pragma once
#include <vector>
#include <string>
#include "busstop.h"

using namespace std;

class BusStops {
private:
    vector<BusStop> MapStops;

public:

    // Constructor 
    // Parses busstop file and constructs BusStop object inside the MapStops vector
    BusStops(const string& filename);

    // getNumMapStops
    // Operation: Outputs number of Busstops 
    // Parameters: None
    // Returns: void 
    int getNumMapStops() const;

    // print
    // Operation: Lists out full busstop information for every busstop 
    // Parameters: None
    // Returns: Void
    void print() const; 


    // getClosestStops
    // Operation: Finds the closest southbound and northbound busstop for an inputted building location 
    // Parameters: double lat, double lon - location of input building 
    // Returns: pair <BusStop, BusStop> closest southbound and northbound BusStop objects   
    pair <BusStop, BusStop> getClosestStops (double lat, double lon) const; 
};
