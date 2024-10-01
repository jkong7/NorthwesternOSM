//
// main.cpp
// Main program that loads a user-inputted OSM XML file--extracts node 
// and building data from that file. It displays the number of nodes and 
// buildings in the file to the user, alows searching for buildings by name, 
// and shows details such as building name, address, ID, and geographic coordinates
// of associated nodes. 
//

#include <iostream>
#include <string>

#include "building.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "buildings.h"

using namespace std;
using namespace tinyxml2;

//
// main
// Operation: Loads and parses a user-inputted OSM XML file and provides an interactive
// search loop for the user to query building details by name
// Returns: int - 0 for success 
//
int main() {
    // Prompt user to input the name of the OSM XML file and attempt to load it.
    string OSMfilename; 
    cout << "Input the name of your file: "; 
    getline(cin, OSMfilename);

    XMLDocument docObj; 
    bool loaded = osmLoadMapFile(OSMfilename, docObj); 
    if (!loaded) {
        cerr << "Error: Couldn't load OSM file" << endl; 
        return 0; 
    }

    // Parse, store, and output information (# of) about the nodes and buildings. 
    Nodes nodes; 
    nodes.readMapNodes(docObj); 
    int numNodes = nodes.getNumMapNodes();

    Buildings buildings; 
    buildings.readMapBuildings(docObj); 
    int numBuildings = buildings.getNumMapBuildings();
    
    cout << "Number of nodes: " << numNodes << endl; 
    cout << "Number of buildings: " << numBuildings << endl; 

    string user_input; 

    // Interactive loop that allows user to search for buildings by name or list all buildings 
    while (user_input != "$") {
        cout << "Enter building name (partial or complete), or * to list, or $ to end: "; 
        getline(cin, user_input); 

        // If user enters "*", list all buildings along with their ID, name, and address.
        if (user_input == "*") {
            for (const Building& building : buildings.MapBuildings) {
                cout << building.ID << ": " << building.Name << ", " << building.StreetAddress << endl;
            }
        } else {
            // Otherwise, user searches for a building--iterate through the list of buildings to find matches with find(). 
            for (const Building& building : buildings.MapBuildings) {
                if (building.Name.find(user_input) != string::npos) {
                    // Display building details if building match(es) found
                    cout << building.Name << endl;
                    cout << "Address: " << building.StreetAddress << endl;
                    cout << "Building ID: " << building.ID << endl;
                    cout << "Nodes: " << endl;

                    // For each node associated with the building, display its coordinates (lat, lon). 
                    for (long long nodeID : building.NodeIDs) {
                        double lat, lon;
                        bool isEntrance;
                        if (nodes.find(nodeID, lat, lon, isEntrance)) {
                            cout << " " << nodeID << ": (" << lat << ", " << lon << ")" << endl; 
                        } else {
                            cout << " " << nodeID << " **NOT FOUND**" << endl;
                        }
                    }
                }
            }
        }
    }
    
    // Summary of statistics after user exits.
    cout << "** Done **" << endl; 
    cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl; 
    cout << "# of Nodes created: " << Node::getCreated() << endl; 
    cout << "# of Nodes copied: " << Node::getCopied() << endl; 

    return 0; 
}