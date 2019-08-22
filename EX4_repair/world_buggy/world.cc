#include <iostream>
#include <sstream>
#include <string>

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
World::World() : KGraph(NULL) {}
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
		world.Insert(location_name,location_ptr);
	} catch (KGraphKeyAlreadyExistsExpection&) {
		throw WorldLocationNameAlreadyUsed();
	}
	return input;
}
//-----------------------------------------------------------------------------
World::~World() {

	vector<Location*>::iterator it;

	for (it = locations.begin() ; it != locations.end() ; ++it) {
		delete *it;
	}
}
//-----------------------------------------------------------------------------


}  // pokemongo
}  // mtm


using namespace mtm;
using namespace mtm::pokemongo;

int main() {

	KGraph<string,Location*,4> graph(NULL);

}



