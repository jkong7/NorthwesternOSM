//
// buildings.h
// Header file for the Buildings class, which allows storage and management
// of Building objects 
// Provides functions to both read and store building data, as well as returning 
// the number of buildings 
// 

#pragma once 

#include <vector>
#include "building.h"
#include "tinyxml2.h"

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
}; 