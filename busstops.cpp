// busstops.cpp 
// Implements the BusStops class -  constructor, getNumMapStops, print, and getClosestStops

#include "busstops.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> 
#include <utility> 
#include "dist.h"


// Constructor 
// Parses busstop file and constructs BusStop object inside the MapStops vector 
BusStops::BusStops(const string& filename) {
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    // Parse file information and create BusStop object in the vector <BusStop> MapStops vector 
    string line;
    while (getline(infile, line)) {
        if (infile.fail()) break;

        stringstream parser(line);
        string id_str, route_str, stopname, direction, location, lat_str, lon_str;

        getline(parser, id_str, ',');
        getline(parser, route_str, ',');
        getline(parser, stopname, ',');
        getline(parser, direction, ',');
        getline(parser, location, ',');
        getline(parser, lat_str, ',');
        getline(parser, lon_str, ',');

        int stopID = stoi(id_str);
        int route = stoi(route_str);
        double lat = stod(lat_str);
        double lon = stod(lon_str);

        MapStops.emplace_back(stopID, route, stopname, direction, location, lat, lon);
    }
}

// getNumMapStops
// Operation: Outputs number of Busstops 
// Parameters: None
// Returns: void
int BusStops::getNumMapStops() const {
  return (int) this->MapStops.size(); 
}


// print
// Operation: Lists out full busstop information for every busstop listed in order of 
// busstop id
// Parameters: None
// Returns: Void
void BusStops::print() const {

    // Create a copy of MapStops, sort the copy, and then print full busstop information 
    // for every busstop 
    vector <BusStop> sortedMapStops = MapStops; 

    sort(sortedMapStops.begin(), sortedMapStops.end(), 
    [](BusStop& stop1, BusStop& stop2) { return stop1.stopID<stop2.stopID; }); 

    for (const BusStop& BusStop : sortedMapStops) {
        BusStop.print(); 
    }
}

// getClosestStops
// Operation: Finds the closest southbound and northbound busstop for an inputted building location 
// Parameters: double lat, double lon - location of input building 
// Returns: pair <BusStop, BusStop> closest southbound and northbound BusStop objects 
pair <BusStop, BusStop> BusStops::getClosestStops (double lat, double lon) const {
    // Create vectors that hold southbound and northbound BusStop objects 
    vector <BusStop> southbound, northbound; 
    for (const BusStop& BusStop : MapStops) {
        if (BusStop.direction == "Southbound") {
            southbound.push_back(BusStop);
        } else if (BusStop.direction == "Northbound") {
            northbound.push_back(BusStop); 
        }
    }
    // Sort both vectors in ascending order of distance from inputted lat and lon using the provided distBetween2Points function 
    sort(southbound.begin(), southbound.end(),
    [lat,lon](BusStop& stop1, BusStop& stop2) {return distBetween2Points(lat,lon,stop1.lat, stop1.lon) < distBetween2Points(lat, lon, stop2.lat, stop2.lon);});
    sort(northbound.begin(), northbound.end(),
    [lat,lon](BusStop& stop1, BusStop& stop2) {return distBetween2Points(lat,lon,stop1.lat, stop1.lon) < distBetween2Points(lat,lon,stop2.lat,stop2.lon);});

    //The first entry in both vectors now contain the closest stops 
    return make_pair(southbound[0], northbound[0]); 
}
