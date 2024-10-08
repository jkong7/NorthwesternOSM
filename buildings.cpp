//
// buildings.cpp
// Implements the Buildings class, with the two methods readMapBuildings and getNumMapBuildings 
// readMapBuildings parses OSM XML document information and passes Building objects into 
// the Buildings class' MapBuildings vector 
// getNumMapBuildings retreives the number of buildings by accessing the size of the MapBuildings vector 
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#include "building.h"
#include "osm.h"
#include "tinyxml2.h"
#include "buildings.h"
#include "curl_util.h"

using namespace std;
using namespace tinyxml2;

//
// readMapBuildings
// Operation: Parses an XML document and iterates through the OSM file 
// to extract information about buildings. It stores the buildings with 
// their unique ID, name, street address, and associated node IDs 
// in the MapBuildings vector.
// Parameters: xmldoc: the XML document containing OSM map data.
// Returns: void
//

void Buildings::readMapBuildings(XMLDocument& xmldoc) {
    // Retrive the root osm element froom the file 
    XMLElement* osm = xmldoc.FirstChildElement("osm"); 
    assert(osm!=nullptr);

    // Loop through all way elements in the XML document.
    // A way element represents a physical structure, such as a building. 
    // For each way, the function checks if it represents a building,
    // and if so, it extracts its details--name, ID, and address, and stores it in the MapBuildings vector.
    XMLElement* way = osm->FirstChildElement("way"); 
    while (way!=nullptr) {
        const XMLAttribute* wayid = way->FindAttribute("id");
        long long ID = wayid->Int64Value(); 

        string name=""; 
        string streetAddr=""; 

        if (osmContainsKeyValue(way, "building", "university")) {
            name = osmGetKeyValue(way, "name"); 
            streetAddr = osmGetKeyValue(way, "addr:housenumber") + " " + osmGetKeyValue(way, "addr:street");

            this->MapBuildings.emplace_back(ID, name, streetAddr); 

            // Loop through all nd elements within the current way element.
            // Each nd element contains a reference to a node, representing a point
            // that is part of the building's outline. 
            // Collects all node references and pushes to the NodeIDs vector within the 
            // corresponding Building object inside the greater MapBuildings vector 
            XMLElement* nd = way->FirstChildElement("nd"); 
            while (nd!=nullptr) {
                const XMLAttribute* ndref = nd->FindAttribute("ref"); 
                assert(ndref!=nullptr); 
                long long id = ndref->Int64Value(); 
                this->MapBuildings.back().add(id); 
                nd=nd->NextSiblingElement("nd"); 
            }
        } 
        way = way->NextSiblingElement("way"); 
    }
};

//
// getNumMapBuildings
// Operation: Returns the number of buildings stored in the MapBuildings vector.
// Parameters: None
// Returns: int
//
int Buildings::getNumMapBuildings() const {
    return (int) this->MapBuildings.size(); 
};

// print
// Operation: Prints outs building information for all buildings 
// Parameters: None
// Returns: void 
void Buildings::print() {
    for (const Building& building : MapBuildings) {
        cout << building.ID << ": " << building.Name << ", " << building.StreetAddress << endl;
    }; 
}; 

// findAndPrint 
// Operation: Loops through all buildings and if there are match(es), will provide parameters to another print function
// to output further information about the matched buildings like closest stop and arrival predictions 
// Parameters: 
//             name - user_inputted string name to associate with buildings. 
//             nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
//             busStops - the BusStops object that stores the bus stop information.
//             curl - CURL* pointer used for making API requests to fetch bus predictions.
// Returns: void
void Buildings::findAndPrint(const string& name, const Nodes& nodes, const BusStops& BusStops, CURL* curl) const {
    bool building_found_flag = false; 
    for (const Building& building : MapBuildings) {
        if (building.Name.find(name) != string::npos) {
            building_found_flag = true; 
            building.print(nodes, BusStops, curl); 
        }
    } 
    // Boolean logic - false to inform user the name matched no buildings 
    if (!building_found_flag && name!="$") {
        cout << "No such building" << endl; 
    }
}
