//
// buildings.h
// Header file for the Buildings class, which allows storage and management
// of Building objects 
// Provides functions to both read and store building data, as well as returning 
// the number of buildings 
// 

#pragma once 

#include <iostream> 
#include <vector>
#include "building.h"
#include "tinyxml2.h"
#include "curl_util.h"

using namespace std; 
using namespace tinyxml2; 

class Buildings
{
    public: 
    vector<Building> MapBuildings;

    //
    // readMapBuildings
    // Operation: Parses an XML document and iterates through the OSM data 
    // to extract information about buildings. It stores the buildings with 
    // their unique ID, name, street address, and associated node IDs 
    // in the MapBuildings vector.
    // Parameters: xmldoc: the XML document containing OSM map data.
    // Returns: void
    //
    void readMapBuildings(XMLDocument& xmldoc); 


    //
    // getNumMapBuildings
    // Operation: Returns the number of buildings stored in the MapBuildings vector.
    // Parameters: None
    // Returns: int
    //
    int getNumMapBuildings() const; 


    // print
    // Operation: Prints outs building information for all buildings 
    // Parameters: None
    // Returns: void
    void print(); 

    // findAndPrint 
    // Operation: Loops through all buildings and if there are match(es), will provide parameters to another print function
    // to output further information about the matched buildings like closest stop and arrival predictions 
    // Parameters: 
    //             name - user_inputted string name to associate with buildings. 
    //             nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
    //             busStops - the BusStops object that stores the bus stop information.
    //             curl - CURL* pointer used for making API requests to fetch bus predictions.
    // Returns: void
    void findAndPrint (const string& name, const Nodes& nodes, const BusStops& BusStops, CURL* curl) const; 
}; 