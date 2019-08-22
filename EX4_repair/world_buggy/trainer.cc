#include <iostream>
#include <set>
#include <string>
#include <cassert>

#include "pokemon.h"
#include "trainer.h"
#include "exceptions.h"

//=============================================================================
//									USING
//=============================================================================

using std::ostream;
using std::endl;
using std::string;
using std::list;

namespace mtm {
namespace pokemongo {

#define TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(num)	num = num/2 + num % 2

//=============================================================================
//									CONSTs
//=============================================================================
const double INITIAL_LVL = 1;
const double TEN_PERCENT = 0.1;

//=============================================================================
//								PRIVATE METHODS
//=============================================================================

const string Trainer::team2str(const Team team) {

	switch (team) {

	case BLUE:		return "BLUE";
	case YELLOW:	return "YELLOW";

	default:		return "RED";
	}
}
//------------------------------------------------------------------------------
bool Trainer::winByTeam(const Trainer& victim) const {

	if (this->team == BLUE && victim.team == RED)		return true;
	if (this->team == RED && victim.team == YELLOW)		return true;
	if (this->team == YELLOW && victim.team == BLUE)	return true;

	return false;
}
//------------------------------------------------------------------------------
void Trainer::useItem(const Item& item,Pokemon& pokemon) {

	const double boost = 1 + TEN_PERCENT * pokemon.Level();
	item.getType() == POTION ? pokemon.Heal() : pokemon.Train(boost);
	vector<Item>::iterator it;

	for (it = items.begin() ; it != items.end() ; ++it) {
		if (*it == item)	break;
	}
	if (it != items.end()) {
		items.erase(it);
	}
}
//------------------------------------------------------------------------------
void Trainer::prepareForBattel(Trainer& trainer1,Trainer& trainer2) {

	Pokemon trainer1_strongest = trainer1.GetStrongestPokemon();
	Pokemon trainer2_strongest = trainer2.GetStrongestPokemon();
	if (trainer1.items.size() > 0) {
		trainer1.useItem(trainer1.items[0],trainer1_strongest);
	}
	if (trainer2.items.size() > 0) {
		trainer2.useItem(trainer2.items[0],trainer2_strongest);
	}
}
//------------------------------------------------------------------------------
// trainer1 strongest pokemon attack trainer2 strongest pokemon and kill them if
// needed
void Trainer::fight(Trainer& trainer1,Trainer& trainer2) {

	Pokemon trainer1_strongest = trainer1.GetStrongestPokemon();
	Pokemon trainer2_strongest = trainer2.GetStrongestPokemon();

	bool trainer1_strongest_died = false;
	bool trainer2_strongest_died = false;

	if (trainer1_strongest.Hit(trainer2_strongest)) {
		trainer2_strongest_died = true;
	}
	if (trainer2_strongest.Hit(trainer1_strongest)) {
		trainer1_strongest_died = true;
	}
	if (trainer1_strongest_died) {
		trainer1.KillStrongestPokemon();
	}
	if (trainer2_strongest_died) {
		trainer2.KillStrongestPokemon();
	}
}
//=============================================================================
//								PUBLIC METHODS
//=============================================================================
Trainer::Trainer(const string& name, const Team& team) :

	name(string(name)),lvl(INITIAL_LVL),team(team),pokemons(vector<Pokemon>()),
															items(vector<Item>()) {

	if (name == "")	throw TrainerInvalidArgsException();
}


//------------------------------------------------------------------------------
Pokemon& Trainer::GetStrongestPokemon() {

	if (pokemons.size() == 0)	throw TrainerNoPokemonsFoundException();

	typedef vector<Pokemon>::iterator Iterator;
	Iterator max_pokemon = pokemons.begin();

	for (Iterator it = pokemons.begin() ; it != pokemons.end() ; it++) {
		if (*it > *max_pokemon) {
			max_pokemon = it;
		}
	}
	// now max_pokemon points to a pokemon with the maximal hit points
	for (Iterator it = pokemons.begin() ; it != pokemons.end() ; it++) {
		if (*it == *max_pokemon) {
			max_pokemon = it;
			break;
		}
	}
	return *max_pokemon;
}


const Pokemon& Trainer::GetStrongestPokemon() const {

	if (pokemons.size() == 0)	throw TrainerNoPokemonsFoundException();

	typedef vector<Pokemon>::const_iterator Const_Iterator;
	Const_Iterator max_pokemon = pokemons.begin();

	for (Const_Iterator it = pokemons.begin() ; it != pokemons.end() ; it++) {
		if (*it > *max_pokemon) {
			max_pokemon = it;
		}
	}
	// now max_pokemon points to a pokemon with the maximal hit points
	for (Const_Iterator it = pokemons.begin() ; it != pokemons.end() ; it++) {
		if (*it == *max_pokemon) {
			max_pokemon = it;
			break;
		}
	}
	return *max_pokemon;
}
//------------------------------------------------------------------------------
void Trainer::KillStrongestPokemon() {

	if (pokemons.size() == 0)	throw TrainerNoPokemonsFoundException();

	Pokemon strongest_pokemon = GetStrongestPokemon();

	typedef vector<Pokemon>::iterator Iterator;
	Iterator it;

	for (it = pokemons.begin() ; it != pokemons.end() ; it++) {
		if (*it == strongest_pokemon) {
			break;
		}
	}
	pokemons.erase(it);
}
//------------------------------------------------------------------------------
const int Trainer::GetPokemonsNum() const {
	return pokemons.size();
}
//------------------------------------------------------------------------------
const int Trainer::GetItemsNum() const {
	return items.size();
}
//------------------------------------------------------------------------------
bool Trainer::operator==(const Trainer& rhs) const {

	if (pokemons.size() == 0  &&  rhs.pokemons.size() == 0)		return true;
	if (pokemons.size() == 0  &&  rhs.pokemons.size() != 0)		return false;
	if (pokemons.size() != 0  &&  rhs.pokemons.size() == 0)		return false;

	return GetStrongestPokemon() == rhs.GetStrongestPokemon();
}
//------------------------------------------------------------------------------
bool Trainer::operator!=(const Trainer& rhs) const {

	return !(*this == rhs);
}
//------------------------------------------------------------------------------
bool Trainer::operator<(const Trainer& rhs) const {

	if (pokemons.size() == 0  &&  rhs.pokemons.size() == 0)		return false;
	if (pokemons.size() == 0  &&  rhs.pokemons.size() != 0)		return true;
	if (pokemons.size() != 0  &&  rhs.pokemons.size() == 0)		return false;

	return GetStrongestPokemon() < rhs.GetStrongestPokemon();
}
//------------------------------------------------------------------------------
bool Trainer::operator>(const Trainer& rhs) const {

	return rhs < *this;
}
//------------------------------------------------------------------------------
bool Trainer::operator<=(const Trainer& rhs) const {

	return !(operator>(rhs));
}
//------------------------------------------------------------------------------
bool Trainer::operator>=(const Trainer& rhs) const {

	return !(operator<(rhs));
}
//------------------------------------------------------------------------------
bool Trainer::IsAlly(const Trainer& trainer) const {

	return GetTeam() == trainer.GetTeam();
}
//------------------------------------------------------------------------------
Team Trainer::GetTeam() const {

	return team;
}
//------------------------------------------------------------------------------
bool Trainer::attack(Trainer& victim) {

	assert(this->GetTeam() != victim.GetTeam());
	bool this_win;

	if (*this == victim)		return this->winByTeam(victim);

	if (*this > victim) {
		this->lvl += TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(victim.lvl);
		this_win = true;
	}else{
		victim.lvl += TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(this->lvl);
		this_win = false;
	}

	if(this->pokemons.size() && victim.pokemons.size()) {
		prepareForBattel(*this,victim);
		fight(*this,victim);
	}

	return this_win;
}
//------------------------------------------------------------------------------
bool Trainer::TryToCatch(const Pokemon& pokemon) {

	if (pokemon.Level() > lvl)	return false;

	pokemons.push_back(pokemon);
	return true;
}
//------------------------------------------------------------------------------
bool Trainer::TryToTake(const Item& item) {

	if (item.getLvl() > lvl)	return false;

	items.push_back(item);
	return true;
}
//------------------------------------------------------------------------------
ostream& operator<<(ostream& output, const Trainer& trainer) {

	output << trainer.name << " (" << trainer.lvl << ") " <<
											Trainer::team2str(trainer.team) << endl;

	typedef vector<Pokemon>::const_iterator Const_Iterator;

	for (Const_Iterator it = trainer.pokemons.begin() ;
										it != trainer.pokemons.end() ; it++) {

		output << *it << endl;
	}

	return output;
}
//------------------------------------------------------------------------------


}  // namespace mtm
}  // namespace pokemongo







