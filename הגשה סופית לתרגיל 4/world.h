#ifndef WORLD_H
#define WORLD_H

#include <iostream>

#include "k_graph.h"
#include "location_extentions.h"


using std::string;
using std::istream;

using namespace mtm;

namespace mtm {
namespace pokemongo {

typedef int Direction;

static const int NORTH = 0;
static const int SOUTH = 1;
static const int EAST = 2;
static const int WEST = 3;

class World : public KGraph<string,Location*,4> {

	vector<Location*> locations;

 public:
  // Constructs a new empty world.
  World();

  // A destructor.
  ~World();

  // assert - current location is a valid key of KGraph
  // update destination_name to have the destination name
  //update destination_obj to have the pointer to desination itself
  //throw - throw WorldReachedEnd if destination is a dead end
  void GetDestination(string current_location,Direction direction,string& destiantion_name,Location*& destination_obj);

  //return a const vector of all the locations that exist in world
  const vector<Location*>& GetLocations() const;

  // Input iterator. Scans a single line from the input stream. The line can be
  // one of the following three options:
  //
  // (1) "GYM <gym_name>"
  //     e.g. "GYM taub"
  //     Adds a gym with the given name to the world.
  // (2) "POKESTOP <pokestop_name> <item_type1> <item_level1> <item_type2> <item_level2> ..."
  //     e.g. "POKESTOP mikhlol POTION 10 CANDY 20 CANDY 13"
  //     Creates a Pokestop with the given name that contains the given items by
  //     order, and adds it to the world.
  // (3) "STARBUCKS <starbucks_name> <pokemon_species1> <cp1> <level1> <pokemon_species2> <cp2> <level2> ..."
  //     e.g. "STARBUCKS shani pikachu 2.5 3 pikachu 1 2 charmander 3.45 4"
  //     Creates a starbucks with the given name that contains Pokemons with the
  //     given parameters by order, and adds it to the world. The Pokemons will
  //     have the default types.
  // You can assume that none of the pieces of information (location name,
  // Pokemon species, etc.) contains a space.
  //
  // @param input the input stream.
  // @param world the world to which to add the locations.
  // @return the input stream.
  // @throw WorldInvalidInputLineException if the input line is not one of the
  //        three options, or one of the parameters is invalid (for example,
  //        negative CP value, etc.).
  // @throw WorldLocationNameAlreadyUsed if there already exists a location with
  //        the given name in the world.
  friend istream& operator>>(istream& input, World& world);

  // Disable copy constructor and operator.
  World(const World& world) = delete;
  World& operator=(const World& world) = delete;

  // Inserts a new node with the given data to the world.
  //
  // @param key the key to be assigned to the new node.
  // @param value the value to be assigned to the new node.
  // @throw WorldLocationNameAlreadyUsed when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(string const& key,Location* const& value = new Gym());

  /* Remove function don't need to be overriden because the vector<Location*>
   * is onley in use to free memory and dont have and logical purpose. memory will
   * be free and world-end-of-scope
   */

};

istream& operator>>(istream& input, World& world);

}  // namespace pokemongo
}  // namespace mtm

#endif  // WORLD_H
