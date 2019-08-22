#ifndef STARBUCKS_H_
#define STARBUCKS_H_

#include <vector>
#include <string>
#include <list>
#include <memory>

#include "location.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::vector;

namespace mtm {
namespace pokemongo {


//=============================================================================
//								COMMUN FUNCTIONS
//=============================================================================
class Gym : public Location {

	typedef enum {ADD,SUB} Action;

		Trainer* leader;
		Team team;

		static int score_red;
		static int score_blue;
		static int score_yellow;

		// return the number of teams int the gym exluding leader
		const int numOfTeams();

		//return a pointer to the strongest trainer in team
		//return NULL if there is no trainer with the given color
		// or if color == NO_COLOR
		Trainer* findMaxTrainerFromTeam(const Team team);

		// assuming there are two diffrent team that are not leader.team
		//the function will make a battle and update the leader of the gym
		void leaderByStrongestLeft();

	  // add or subtract num (positive number) from team score
	  static void affectScore(Team team,int num,Action action);

	public:

		// create a new Gym location
		Gym();
		Gym(const Gym& gym) = delete;
		Gym& operator=(const Gym& gym) = delete;

		// do as same as Location::Arrive but also try to conquer the gym
		void Arrive(Trainer& trainer) override;

//	 	do as same as Location::Leave but also find a new leader to the gym
		virtual void Leave(Trainer& trainer) override;

		// return a const pointer to the leader
		const Trainer* const getLeader() const;

	  // Returns the score of the team
	  static const int GetTeamScore(const Team team);

	  // initiallize all teams score to 0
	  static void InitiallizeTeamsScore();
};


//-----------------------------------------------------------------------------
class Pokestop : public Location {

		vector<Item> items;

	public:

		// create a new Pokestop location
		//
		// @param data is an input vector<string> as followed:
		//"<item_type1> <item_level1> <item_type2> <item_level2> ..."
		explicit Pokestop(const string input_str);
		Pokestop(const Pokestop& pokestop) = delete;
		Pokestop& operator=(const Pokestop& pokestop) = delete;

		// do as same as Location::Arrive but also take the first item he can
		//if he can't - he do nothing
		void Arrive(Trainer& trainer) override;

		// return the number of itemss in pokestop
		int getItemsNum() const;
};

//-----------------------------------------------------------------------------
class Starbucks : public Location {

		vector<Pokemon> pokemons;

	public:

		// create a new Starrbucks location
		//
		// @param data is an input vector<string> as followed:
		//"<pokemon_species1> <cp1> <level1> <pokemon_species2> <cp2> <level2> ..."
		explicit Starbucks(const string input_str);
		Starbucks(const Starbucks& starbucks) = delete;
		Starbucks& operator=(const Starbucks& starbucks) = delete;

		// do as same as Location::Arrive but also try to catch the first pokemon in
		//place
		void Arrive(Trainer& trainer) override;

		// return the number of pokemons in starbcuks
		int getPokemonsNum() const;

};
//-----------------------------------------------------------------------------




}  // pokemongo
}  // mtm

#endif /* STARBUCKS_H_ */
