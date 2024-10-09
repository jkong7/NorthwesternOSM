// busstop.h
// Represents one bus stop with all the necessary information to identify it : stopID, route, stopName, 
// direction, location and lat/lon

#pragma once 
#include <string> 
using namespace std; 

class BusStop {
public:
    int stopID;
    int route;
    string stopName;
    string direction;
    string location;
    double lat;
    double lon;

    // Contructor
    BusStop(int stopID, int route, const string& stopName, const string& direction,
            const string& location, double lat, double lon);

    // print
    // Operation: Prints out all the bus stop information for one BusStop object
    // Parameters: None
    // Returns: void
    void print() const; 
};