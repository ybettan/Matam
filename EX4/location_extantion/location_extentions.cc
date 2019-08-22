#include "location_extentions.h"
#include "exceptions.h"

#include <cassert>
#include <sstream>

namespace mtm {
namespace pokemongo {

const int TWO_POSSIBLE_TEAMS = 2;
const int FIRST = 0;
const int SECOND = 1;





int Gym::score_red = 0;
int Gym::score_blue = 0;
int Gym::score_yellow = 0;

//=============================================================================
//								GYM METHODS
//=============================================================================
void Gym::affectScore(Team team,int num,Action action) {

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
//-----------------------------------------------------------------------------------
Trainer* Gym::findMaxTrainerFromTeam(const Team team) {

	if (team == NO_COLOR)	return NULL;
	Trainer* result = NULL;
	vector<Trainer*> vec = GetTrainers();

	for (unsigned int i = 0 ; i < vec.size() ; i++) {
		if( !result && vec[i]->GetTeam() == team && vec[i] != leader) {
				result = vec[i];
		}
		if (result && vec[i]->GetTeam()==team && vec[i]!=leader && *vec[i]>*result){
			result = vec[i];
		}
	}
	return result;
}
//-----------------------------------------------------------------------------------
void Gym::leaderByStrongestLeft() {

	Team teams_arr[TWO_POSSIBLE_TEAMS] = {NO_COLOR,NO_COLOR};
	int index = 0;
	for (int i = BLUE ; i < NO_COLOR ; i++) {
		if (i != team) {
			teams_arr[index] = (Team)i;
			index++;
		}
	}
	Trainer* first_replacment = findMaxTrainerFromTeam(teams_arr[FIRST]);
	Trainer* second_replacment = findMaxTrainerFromTeam(teams_arr[SECOND]);

	Trainer* winner = NULL;
	if (first_replacment && !second_replacment) { // only one team can replace
		winner = first_replacment;
	}else if ( !first_replacment && second_replacment) {
		winner = second_replacment;				// only one other team can replace
	}else if (first_replacment->attack(*second_replacment)){
		winner = first_replacment;				// 2 team will fight to replace
	}else{
		winner = second_replacment;
	}
	affectScore(leader->GetTeam(),10,SUB);
	leader = winner;
	team = winner->GetTeam();
	affectScore(team,10,ADD);
}
//-----------------------------------------------------------------------------------
Gym::Gym() : leader(NULL),team(NO_COLOR) {}
//-----------------------------------------------------------------------------------
void Gym::Arrive(Trainer& trainer) {

	Location::Arrive(trainer);

	if (leader == NULL) {
		leader = &trainer;
		team = trainer.GetTeam();
		affectScore(team,10,ADD);
		return;
	}
	if (trainer.IsAlly(*leader)) 	return;

	if (trainer.attack(*leader)) {

		affectScore(leader->GetTeam(),10,SUB);
		leader = &trainer;
		team = trainer.GetTeam();
		affectScore(team,10,ADD);
	}
	return;
}
//-----------------------------------------------------------------------------------
void Gym::Leave(Trainer& trainer) {

	if (&trainer != leader) {			// trainer is not the leder
		Location::Leave(trainer);
		return;
	}
	if (GetTrainers().size() == 1) {	// leader is the only trainer in gym
		affectScore(leader->GetTeam(),10,SUB);
		leader = NULL;
		team = NO_COLOR;
		Location::Leave(trainer);
		return;
	}
	Trainer* same_team_replacment = findMaxTrainerFromTeam(team);
	if (same_team_replacment) {// there is a replacment from the same team as leader
		leader = same_team_replacment;
		Location::Leave(trainer);
		return;
	}
	//leader by strongest in last team remains that is diffrent from old leader
	leaderByStrongestLeft();

	Location::Leave(trainer);
	return;
}
//-----------------------------------------------------------------------------------
const Trainer* const Gym::getLeader() const {
	return leader;
}
//------------------------------------------------------------------------------
const int Gym::GetTeamScore(const Team team) {

	if (team == RED)	return score_red;
	if (team == BLUE)	return score_blue;

	return score_yellow;
}
//------------------------------------------------------------------------------
void Gym::InitiallizeTeamsScore() {

	score_red = 0;
	score_blue = 0;
	score_yellow = 0;
}
//=============================================================================
//								POKESTOP METHODS
//=============================================================================
Pokestop::Pokestop(const string input_str) {

	ItemType item_type;
	int item_lvl;
	std::istringstream iss(input_str);
	string word;

	iss >> word;	// word = POKESTOP
	iss >> word;	// word = <pokestop_name>

	while(iss >> word) {
		item_type = Item::str2itemType(word);
		iss >> word;
		item_lvl = stoi(word);
		Item item(item_type,item_lvl);
		items.push_back(item);
	}
}
//-----------------------------------------------------------------------------------
void Pokestop::Arrive(Trainer& trainer) {

	Location::Arrive(trainer);

	if (items.size() == 0)	return;
	vector<Item>::iterator it;

	for (it = items.begin() ; it != items.end() ; ++it) {
		if (trainer.TryToTake(*it)) {
			items.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------------------
int Pokestop::getItemsNum() const {

		return items.size();
}
//=============================================================================
//								STARBUCKS METHODS
//=============================================================================
Starbucks::Starbucks(const string input_str) {

	string specie;
	double cp;
	int lvl;
	std::istringstream iss(input_str);
	string word;

	iss >> word;		// word = STARBUCKS
	iss >> word;		// WORD = <starbucks_name>

	while (iss >> word) {
		specie = word;
		iss >> word;
		cp = stod(word);
		iss >> word;
		lvl = stoi(word);
		Pokemon pokemon(specie,cp,lvl);
		pokemons.push_back(pokemon);
	}
}
//-----------------------------------------------------------------------------------
void Starbucks::Arrive(Trainer& trainer) {

	Location::Arrive(trainer);

	if (pokemons.size() == 0)	return;
	if (trainer.TryToCatch(*(pokemons.begin()))) {
		pokemons.erase(pokemons.begin());
	}
}
//-----------------------------------------------------------------------------------
int Starbucks::getPokemonsNum() const {

	return pokemons.size();
}
//-----------------------------------------------------------------------------------


}  // pokemongo
}  // mtm






