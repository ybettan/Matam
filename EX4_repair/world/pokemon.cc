#include <iostream>
#include <set>
#include <string>

#include "pokemon.h"

//=============================================================================
//									USING
//=============================================================================

using std::string;
using std::set;
using mtm::pokemongo::Pokemon;
using namespace mtm::pokemongo;





//=============================================================================
//									CONSTs
//=============================================================================

const double INITIAL_HP = 100.0;





//=============================================================================
//								PRIVATE METHODS
//=============================================================================
void Pokemon::strenghtParams(	const Pokemon& pokemon1,
		  	  	  	  	  	  	const Pokemon& pokemon2,
		  	  	  	  	  	  	double& pokemon1_hit_power,
								double& pokemon2_hit_power,
								int& pokemon1_types_sum,
								int& pokemon2_types_sum			) {

	typedef std::set<PokemonType>::iterator Iterator;

	pokemon1_hit_power = pokemon1.cp * pokemon1.level;
	pokemon2_hit_power = pokemon2.cp * pokemon2.level;
	pokemon1_types_sum = 0;
	pokemon2_types_sum = 0;

	for (Iterator it=pokemon1.types.begin() ; it!=pokemon1.types.end() ; it++) {
		pokemon1_types_sum = pokemon1_types_sum + *it;
	}
	for (Iterator it=pokemon2.types.begin() ; it!=pokemon2.types.end() ; it++) {
		pokemon2_types_sum = pokemon2_types_sum + *it;
	}

}
//------------------------------------------------------------------------------
string pokemonType2str(PokemonType type) {

	switch (type) {

	case ROCK:		return "ROCK";
	case BUG:		return "BUG";
	case FAIRY:		return "FAIRY";
	case GROUND:	return "GROUND";
	case GRASS:		return "GRASS";
	case WATER:		return "WATER";
	case ICE:		return "ICE";
	case GHOST:		return "GHOST";
	case POISON:	return "POISON";
	case ELECTRIC:	return "ELECTRIC";
	case FIRE:		return "FIRE";
	case FLYING:	return "FLYING";
	case PSYCHIC:	return "PSYCHIC";

	default:		return "NORMAL";
	}
}


//=============================================================================
//								PUBLIC METHODS
//=============================================================================

Pokemon::Pokemon(	const std::string& species,
					const std::set<PokemonType>& types,
					const double& cp,
					const int& level		) :

	species(string(species)),level(level),hp(INITIAL_HP),cp(cp),types(types) {

	if (cp <= 0 ||level <= 0 || species=="")throw PokemonInvalidArgsException();
}
//------------------------------------------------------------------------------
Pokemon::Pokemon(	const std::string& species,
					const double& cp,
					const int& level			) :

	species(string(species)),level(level),hp(INITIAL_HP),cp(cp),
	types(GetDefaultTypes(species)) {

	if (cp <= 0 ||level <= 0 || species=="")throw PokemonInvalidArgsException();
}
//------------------------------------------------------------------------------
bool Pokemon::operator==(const Pokemon& rhs) const {

	double this_hit_power , rhs_hit_power;
	int this_types_sum , rhs_types_sum;

	strenghtParams(*this,rhs,this_hit_power,rhs_hit_power,this_types_sum,
															rhs_types_sum);

	if ((this_hit_power != rhs_hit_power)||(this_types_sum != rhs_types_sum)) {
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------
bool Pokemon::operator!=(const Pokemon& rhs) const {

	return !(*this == rhs);
}
//------------------------------------------------------------------------------
bool Pokemon::operator<(const Pokemon& rhs) const {

	double this_hit_power , rhs_hit_power;
	int this_types_sum , rhs_types_sum;

	strenghtParams(*this,rhs,this_hit_power,rhs_hit_power,this_types_sum,
															rhs_types_sum);

	if (this_hit_power < rhs_hit_power)		return true;
	if (this_hit_power > rhs_hit_power)		return false;

	return this_types_sum < rhs_types_sum ? true : false;

}
//------------------------------------------------------------------------------
bool Pokemon::operator>(const Pokemon& rhs) const {

	return rhs.operator<(*this);
}
//------------------------------------------------------------------------------
bool Pokemon::operator<=(const Pokemon& rhs) const {

	return !(*this > rhs);
}
//------------------------------------------------------------------------------
bool Pokemon::operator>=(const Pokemon& rhs) const {

	return !(*this < rhs);
}
//------------------------------------------------------------------------------
std::ostream& mtm::pokemongo::operator<<(std::ostream& output,
													const Pokemon& pokemon) {

	output << pokemon.species << "(" << pokemon.level << "/" << pokemon.cp <<
													"/" << pokemon.hp << ")";
	if (pokemon.types.size() != 0) {
		output << " ";
	}

	typedef std::set<PokemonType>::iterator Iterator;
	unsigned int counter = 1;

	for (Iterator it=pokemon.types.begin() ; it!=pokemon.types.end() ;
															it++,counter++) {

		/* the std::set is alredy ordered nad since PokemonType is interpreted
		   as integers by the compiller than we dont need to overload the
		   compare function*/
		output << pokemonType2str(*it);
		if (counter != pokemon.types.size()) {
			output << " ";
		}
	}

	return output;
}
//------------------------------------------------------------------------------
int Pokemon::Level() const {

	return level;
}
//------------------------------------------------------------------------------
bool Pokemon::Hit(Pokemon& victim) {

	double hit_power = level * cp;
	victim.hp -= hit_power;

	if (victim.hp < 0) {
		victim.hp = 0;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------
void Pokemon::Heal() {

	hp = INITIAL_HP;
}
//------------------------------------------------------------------------------
void Pokemon::Train(const double& boost) {

	if (boost <= 1)	throw PokemonInvalidArgsException();

	cp *= boost;
}
//------------------------------------------------------------------------------










