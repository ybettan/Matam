#include <iostream>


#include "pokemon_go.h"


namespace mtm {
namespace pokemongo {


//=============================================================================
//								PRIVATE METHODS
//=============================================================================
bool PokemonGo::IsTrainerNameAlredyExist(const string name) const {

	vector<Placment>::const_iterator it;
	for (it = placments.begin() ; it != placments.end() ; ++it) {
		if (	(*it).GetTrainer()->GetName() == name	)	return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
Trainer& PokemonGo::GetTrainer(const string name) const {

	vector<Placment>::const_iterator it;

	for (it = placments.begin() ; it != placments.end() ; ++it) {
		if ((*it).GetTrainer()->GetName() == name) {
			break;
		}
	}
	return *((*it).GetTrainer());
}
//-----------------------------------------------------------------------------
string& PokemonGo::GetCurrentLocationName(const string& trainer_name) {

	vector<Placment>::iterator it;

	for (it = placments.begin() ; it != placments.end() ; ++it) {
		if (	(*it).GetTrainer()->GetName() == trainer_name	) {
			break;
		}
	}
	return (*it).GetLocationName();
}
//=============================================================================
//								PUBLIC METHODS
//=============================================================================
PokemonGo::PokemonGo(World* world) : world(world) {

	Trainer::InitiallizeTeamsScore();
}
//-----------------------------------------------------------------------------
void PokemonGo::AddTrainer(const string& name,const Team& team,
															const string& location){

	if (name == "")		throw PokemonGoInvalidArgsException();
	if (IsTrainerNameAlredyExist(name))
								throw PokemonGoTrainerNameAlreadyUsedExcpetion();
	if (!world->Contains(location))
								throw PokemonGoLocationNotFoundException();

	Placment placment(location,new Trainer(name,team));
	placments.push_back(placment);

	Location* required_location = (*world)[location];
	required_location->Arrive(*placment.GetTrainer());
}
//-----------------------------------------------------------------------------
void PokemonGo::MoveTrainer(const string& trainer_name, const Direction& dir) {

	if ( !IsTrainerNameAlredyExist(trainer_name) )
										throw PokemonGoTrainerNotFoundExcpetion();
	string destination_name;
	Location* destination = NULL;
	try {
		world->GetDestination(WhereIs(trainer_name),dir,destination_name,destination);
	}
	catch(WorldReachedEnd&) {
		throw PokemonGoReachedDeadEndException();
	}
	Location* current_location = (*world)[WhereIs(trainer_name)];
	string& current_location_name = GetCurrentLocationName(trainer_name);
	current_location_name = destination_name;
	current_location->Leave(GetTrainer(trainer_name));
	destination->Arrive(GetTrainer(trainer_name));
}
//-----------------------------------------------------------------------------
PokemonGo::~PokemonGo() {

	vector<Placment>::iterator it;

	for (it = placments.begin() ; it != placments.end() ; ++it) {
		delete (*it).GetTrainer();
	}

	delete world;
}
//-----------------------------------------------------------------------------
string PokemonGo::WhereIs(const string& trainer_name) {

	vector<Placment>::iterator it;

	for (it = placments.begin() ; it != placments.end() ; ++it) {
		if ((*it).GetTrainer()->GetName() == trainer_name)
													return (*it).GetLocationName();
	}
	throw PokemonGoTrainerNotFoundExcpetion();
}
//-----------------------------------------------------------------------------
const vector<Trainer*>& PokemonGo::GetTrainersIn(const string& location) {

	try {
		KGraph<string,Location*,4>::iterator it = world->BeginAt(location);
	}
	catch (KGraphKeyNotFoundException&) {
		throw PokemonGoLocationNotFoundException();
	}
	Location* location_ptr = world->operator[](location);

	return location_ptr->GetTrainers();
}
//-----------------------------------------------------------------------------
int PokemonGo::GetScore(const Team& team) {

	return Trainer::GetTeamScore(team) + Gym::GetTeamScore(team);
}
//-----------------------------------------------------------------------------


}  // namespace pokemongo
}  // namespace mtm

