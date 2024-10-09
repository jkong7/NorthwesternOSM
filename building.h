/*building.h*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#pragma once

#include <string>
#include <vector>
#include <iostream> 

#include "node.h"
#include "nodes.h"
#include "busstops.h"
#include "curl_util.h"

using namespace std;


//
// Building
//
// Defines a campus building with a name (e.g. "Mudd"), a street
// address (e.g. "2233 Tech Dr"), and the IDs of the nodes that
// define the position / outline of the building.
// 
// NOTE: the Name could be empty "", the HouseNumber could be
// empty, and the Street could be empty. Imperfect data.
//
class Building
{
public:
  long long ID;
  string Name;
  string StreetAddress;
  vector<long long> NodeIDs;

  //
  // constructor
  //
  Building(long long id, string name, string streetAddr);

  //
  // adds the given nodeid to the end of the vector.
  //
  void add(long long nodeid);
  
  //
  // print: Outputs building details, closest bus stop information, and bus arrival predictions.
  // Parameters: nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
  //             busStops - the BusStops object that stores the bus stop information.
  //             curl - CURL* pointer used for making API requests to fetch bus predictions.
  // Returns: void
  //
  void print(const Nodes& nodes, const BusStops& BusStops, CURL* curl) const; 

  //
  // getLocation: calculates the average latitude and longitude of the building's perimeter nodes.
  // Parameters: nodes - the Nodes object that stores the lat/lon values of all nodes in the map.
  // Returns: pair containing the average latitude and longitude for building 
  //
  pair <double, double> getLocation(const Nodes& nodes) const; 

};
