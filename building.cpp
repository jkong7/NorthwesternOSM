/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include "building.h"
#include "busstops.h"
#include "dist.h"
#include "curl_util.h"
#include <iostream> 
#include <utility> 
#include <stdexcept> 


#include "json.hpp" 
using json = nlohmann::json;


using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//
// print: Outputs building details, closest bus stop information, and bus arrival predictions.
// Parameters: nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
//             busStops - the BusStops object that stores the bus stop information.
//             curl - CURL* pointer used for making API requests to fetch bus predictions.
// Returns: void
//
void Building::print(const Nodes& nodes, const BusStops& busStops, CURL* curl) const {

    // Get location of building using getLocation 
    double firstLocation = getLocation(nodes).first; 
    double secondLocation = getLocation(nodes).second; 
    
    // Find the closest northbound and southbound stops using getClosestStops
    pair <BusStop, BusStop> closestStops = busStops.getClosestStops(firstLocation, secondLocation); 
    BusStop southboundStop = closestStops.first; 
    BusStop northboundStop = closestStops.second; 

    // These two shortest distances need to be printed, so hold them in two variables 
    double southboundDistance=distBetween2Points(firstLocation, secondLocation, southboundStop.lat, southboundStop.lon);
    double northboundDistance=distBetween2Points(firstLocation, secondLocation, northboundStop.lat, northboundStop.lon); 

    // Construct URL with API key, stop route, stop ID, and JSON format - do for both southbound and northbound 
    string myAPIKey = "vbEtpCtxZuuwE2uNBCSdT4J3L";  
    string southboundURL = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=" + myAPIKey + "&rt=" + to_string(southboundStop.route) + "&stpid=" + to_string(southboundStop.stopID) + "&format=json";
    string northboundURL = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=" + myAPIKey + "&rt=" + to_string(northboundStop.route) + "&stpid=" + to_string(northboundStop.stopID) + "&format=json";
    if (!curl) {
        cerr << "Error: CURL not initialized" << endl;
        return;
    }

    // Request and retrieve JSON information from CTA using callWebServer, catach response with southboundResponse and northboundResponse variables 
    string southboundResponse;
    bool southboundSuccess = callWebServer(curl, southboundURL, southboundResponse);

    string northboundResponse;
    bool northboundSuccess = callWebServer(curl, northboundURL, northboundResponse);

    // Print out all building information, inluding location and closest southbound and northbound stops 
    cout << Name << endl;
    cout << "Address: " << StreetAddress << endl;
    cout << "Building ID: " << ID << endl;
    cout << "# perimeter nodes: " << NodeIDs.size() << endl;
    cout << "Location: (" << firstLocation << ", " << secondLocation << ')' <<endl; 
    cout << "Closest southbound bus stop:" <<endl; 
    cout << "  " << southboundStop.stopID << ": " << southboundStop.stopName << ", bus #" << southboundStop.route << ", " << southboundStop.location << ", " << southboundDistance << " miles" <<endl;

    // If callWebServer returned false, inform user
    if (!southboundSuccess) {
        cout << "  <<bus predictions unavailable, call failed>>" << endl;
    // Otherwise parse JSON response, retrieve and loop through the predictions map to output 
    // necessary information 
    } else {
        auto jsondata = json::parse(southboundResponse);
        auto bus_response = jsondata["bustime-response"];
        auto predictions = bus_response["prd"];

        if (predictions.empty()) {
            cout << "  <<no predictions available>>" << endl;
        } else {
            for (auto& M : predictions) {
                // Wrap prediction output loop in a try-catch block to catch errors and handle gracefully 
                try {
                    cout << "  vehicle #" << stoi(M["vid"].get_ref<string&>()) 
                        << " on route " << M["rt"].get_ref<string&>() 
                        << " travelling " << M["rtdir"].get_ref<string&>() 
                        << " to arrive in " << M["prdctdn"].get_ref<string&>() 
                        << " mins" << endl;
                } catch (const exception& e) {
                    cout << "  <<malformed CTA response, prediction unavailable>>" << endl;
                    cout << "  (error: " << e.what() << ")" << endl;
                }
            }
        }
    }

    // Do the exact same with the northbound JSON response 
    cout << "Closest northbound bus stop:" <<endl; 
    cout << "  " << northboundStop.stopID << ": " << northboundStop.stopName << ", bus #" << northboundStop.route << ", " << northboundStop.location << ", " << northboundDistance << " miles" <<endl;

    if (!northboundSuccess) {
        cout << "  <<bus predictions unavailable, call failed>>" << endl;
    } else {
        auto jsondata = json::parse(northboundResponse);
        auto bus_response = jsondata["bustime-response"];
        auto predictions = bus_response["prd"];

        if (predictions.empty()) {
            cout << "  <<no predictions available>>" << endl;
        } else {
            for (auto& M : predictions) {
                try {
                    cout << "  vehicle #" << stoi(M["vid"].get_ref<string&>()) 
                        << " on route " << M["rt"].get_ref<string&>() 
                        << " travelling " << M["rtdir"].get_ref<string&>() 
                        << " to arrive in " << M["prdctdn"].get_ref<string&>() 
                        << " mins" << endl;
                } catch (const exception& e) {
                    cout << "  <<malformed CTA response, prediction unavailable>>" << endl;
                    cout << "  (error: " << e.what() << ")" << endl;
                }
            }
        }
    }


    /*
    for (long long nodeID : NodeIDs) {
        double lat = 0.0, lon = 0.0;
        bool entrance = false;
        bool found = nodes.find(nodeID, lat, lon, entrance);

        cout << " " << nodeID << ": ";
        if (found) {
            cout << "(" << lat << ", " << lon << ")";
            if (entrance) cout << ", is entrance";
        } else {
            cout << "**NOT FOUND**";
        }
        cout << endl;
    }
    */
}


//
// getLocation: calculates the average latitude and longitude of the building's perimeter nodes.
// Parameters: nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
// Returns: pair containing the average latitude and longitude for building 
//
pair <double, double> Building::getLocation(const Nodes& nodes) const {
  double avgLat=0.0; 
  double avgLon=0.0; 
  int nodeCount = NodeIDs.size(); 

  //Average the lat and lon for all nodeIDs within the NodeIDs vector 
  for (long long nodeID : NodeIDs) {
    double lat, lon; 
    bool isEntrance; 
    if (nodes.find(nodeID, lat, lon, isEntrance)) {
      avgLat+=lat; 
      avgLon+=lon; 
    }
  }
  avgLat/=nodeCount; 
  avgLon/=nodeCount; 

  return make_pair(avgLat, avgLon); 
}


