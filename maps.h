/////////PRIMARY Maps/////save unique objects of a class with a unique key. Maps are in the .h file of the class that the map contains. This is the official record of all objects of the class. Map will be loaded at the start of program, new objects inserted into map will be saved to file.
/////////SECONDARY Multimaps/////////////save only keys to file, all the map functions like find and emplace only return iterators which are not useful to save. Lookups will be needed to use the contents of these maps
/////This file contains functions that can be used with maps

#pragma once
#include "Artist.h"
#include "Song.h"
#include "MapManagement.h"
#include <iostream>
#include <map>
using namespace std;



	