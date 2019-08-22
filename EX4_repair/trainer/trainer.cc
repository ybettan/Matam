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

#define TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(num)	num = num/2 + num % 2;

//=============================================================================
//									VARIABLESs
//=============================================================================
const double INITIAL_LVL = 1;
const double TEN_PERCENT = 0.1;

int Trainer::score_red = 0;
int Trainer::score_blue = 0;
int Trainer::score_yellow = 0;
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
void Trainer::fight(Trainer& trainer1,Trainer& trainer2) {

	Pokemon& trainer1_strongest = trainer1.GetStrongestPokemon();
	Pokemon& trainer2_strongest = trainer2.GetStrongestPokemon();

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
//------------------------------------------------------------------------------
void Trainer::affectScore(Team team,int num,Action action) {

	assert(num > 0);

	if (action == SUB) {
		num = -num;
	}

	switch (team) {
	case RED :
		score_red += num;
		break;
	case BLUE :
		score_blue += num;
		break;
	default :
		score_yellow += num;
	}
}
//=============================================================================
//								PUBLIC METHODS
//=============================================================================
Trainer::Trainer(const string& name, const Team& team) :

	name(string(name)),lvl(INITIAL_LVL),team(team),pokemons(vector<Pokemon>()),
															items(vector<Item>()) {
	if (name == "")	throw TrainerInvalidArgsException();

	affectScore(team,1,ADD);
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
const string Trainer::GetName() const {
	return name;
}
//------------------------------------------------------------------------------
const int Trainer::GetTeamScore(const Team team) {

	if (team == RED)	return score_red;
	if (team == BLUE)	return score_blue;

	return score_yellow;
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
		affectScore(this->team,this->lvl,SUB);
		this->lvl += TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(victim.lvl);
		affectScore(this->team,this->lvl,ADD);
		this_win = true;
	}else{
		affectScore(victim.team,victim.lvl,SUB);
		victim.lvl += TO_HIGHER_BOUNT_WHEN_DIVIDED_BY_TWO(this->lvl);
		affectScore(victim.team,victim.lvl,ADD);
		this_win = false;
	}

	if(this->pokemons.size() && victim.pokemons.size()) {
		prepareForBattel(*this,victim);
		fight(*this,victim);
	}
	Team winner_team = this_win ? this->team : victim.team;
	Team looser_team = this_win ? victim.team : this->team;

	affectScore(winner_team,2,ADD);
	affectScore(looser_team,1,SUB);

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
void Trainer::InitiallizeTeamsScore() {

	score_red = 0;
	score_blue = 0;
	score_yellow = 0;
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







