#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "world.h"

//=============================================================================
//									USING
//=============================================================================

using std::string;
using std::istringstream;
using std::cout;
using std::endl;



namespace mtm {
namespace pokemongo {


//=============================================================================
//								PUBLIC METHODS
//=============================================================================
World::World() : KGraph(NULL) {

	Gym::InitiallizeTeamsScore();
}
//-----------------------------------------------------------------------------
istream& operator>>(istream& input, World& world) {
	string whole_input , location_type , location_name;
	std::getline(input,whole_input);
	istringstream iss(whole_input);
	Location* location_ptr = NULL;
	iss >> location_type;
	iss >> location_name;
	try {
		if (location_type == "GYM") {
			location_ptr = new Gym();
			world.locations.push_back(location_ptr);
		}else if (location_type == "POKESTOP") {
			location_ptr = new Pokestop(whole_input);
			world.locations.push_back(location_ptr);
		}else if (location_type == "STARBUCKS") {
			location_ptr = new Starbucks(whole_input);
			world.locations.push_back(location_ptr);
		}else {
			throw WorldInvalidInputLineException();
		}
	}
	catch (...) {
		throw WorldInvalidInputLineException();
	}
	try {
		world.KGraph::Insert(location_name,location_ptr);
	} catch (KGraphKeyAlreadyExistsExpection&) {
		throw WorldLocationNameAlreadyUsed();
	}
	return input;
}
//-----------------------------------------------------------------------------
World::~World() {

	vector<Location*>::iterator it;

	for (it = locations.begin() ; it != locations.end() ; ++it) {
		delete (*it);
	}
}
//-----------------------------------------------------------------------------
void World::GetDestination(string current_location, Direction direction,
						string& destination_name,Location*& destination_obj) {

	assert(Contains(current_location));

	try{
		destination_name =  *((BeginAt(current_location)).Move(direction));
	}
	catch(KGraphIteratorReachedEnd&) {
		throw WorldReachedEnd();
	}
	destination_obj = (*this)[destination_name];
}
//-----------------------------------------------------------------------------
const vector<Location*>& World::GetLocations() const {
	return locations;
}
//-----------------------------------------------------------------------------
void World::Insert(string const& key,Location* const& value) {

	locations.push_back(value);

	try {
		KGraph::Insert(key,value);
	} catch (KGraphKeyAlreadyExistsExpection&) {
		throw WorldLocationNameAlreadyUsed();
	}
}
//-----------------------------------------------------------------------------


}  // pokemongo
}  // mtm



