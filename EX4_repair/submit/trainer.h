#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>

#include "pokemon.h"
#include "item.h"

using std::vector;
using std::string;

namespace mtm {
namespace pokemongo {

// Teams in game.
typedef enum {
  BLUE,
  YELLOW,
  RED,
  NO_COLOR
} Team;

class Trainer {

	string name;
	int lvl;						// lvl > 0
	Team team;
	vector<Pokemon> pokemons;
	vector<Item> items;

	static int score_red;
	static int score_blue;
	static int score_yellow;

	typedef enum {ADD,SUB} Action;


  // returns the equivalent string for a specific Team.
  //
  // @param team the enum value of Team.
  static const string team2str(const Team team);

  // returns true if *this is "stronger by team" than victim.
  //
  // @param team the enum value of Team.
  bool winByTeam(const Trainer& victim) const;

  // the trainer use an item on a pokemon
  void useItem(const Item& item,Pokemon& pokemon);

  // each trainer uses his oldest item on his oldest item
  // if a trainer dont have item or pokemon he do nothing
  static void prepareForBattel(Trainer& trainer1,Trainer& trainer2);

  // trainer1 strongest pokemon attack trainer2 strongest pokemon and kill them if
  // needed
  static void fight(Trainer& trainer1,Trainer& trainer2);

  // add or subtract num (positive number) from team score
  static void affectScore(Team team,int num,Action action);
  static void affectScores(const Trainer& winner,const Trainer& looser);


 public:



  // Constructs a new trainer with the given name and team.
  //	+1 for the team score
  //
  // @param name the name of the trainer.
  // @param team the team to which the trainer belongs.
  // @throw TrainerInvalidArgsException if name is an empty string.
  Trainer(const std::string& name, const Team& team);

  // Returns a reference to the strongest Pokemon the trainer owns. Strongest 
  // Pokemon is determined using the comparison operators provided by the class
  // Pokemon. If two Pokemons are of equal strength, the function returns the
  // one that was caught earlier by the trainer.
  //
  // @return the strongest pokemon.
  // @throw TrainerNoPokemonsException if trainer has no Pokemons.
  Pokemon& GetStrongestPokemon();
  const Pokemon& GetStrongestPokemon() const;

  // Returns the number of pokemons in trainer
  const int GetPokemonsNum() const;

  // Returns the number of items in trainer
  const int GetItemsNum() const;

  // Returns the name of trainer
  const string GetName() const;

  // Returns the score of the team
  static const int GetTeamScore(const Team team);

  // Kills the strongest Pokemon. Removes the Pokemon that is returned from
  // GetStrongestPokemon() from the collection of Pokemons owned by the trainer.
  //
  // @throw TrainerNoPokemonsException if trainer has no Pokemons.
  void KillStrongestPokemon();

  // Comparison operators for trainers. Trainers are compared as described in
  // the exercise sheet.
  //
  // @param rhs the right-hand side operand.
  bool operator==(const Trainer& rhs) const;
  bool operator!=(const Trainer& rhs) const;
  bool operator<(const Trainer& rhs) const;
  bool operator>(const Trainer& rhs) const;
  bool operator<=(const Trainer& rhs) const;
  bool operator>=(const Trainer& rhs) const;

  // Checks whether a given trainer is in the same team as this trainer.
  //
  // @param trainer
  // @return true iff trainer (argument) and this trainer belong to the same
  //         team.
  bool IsAlly(const Trainer& trainer) const;

  // Returns the team to which the trainer belongs.
  //
  // @return the team to which the trainer belongs.
  Team GetTeam() const;

  // trainer attack the victim.
  // @assume with assert that the trainers are from different teams
  // @return true if trainer won or false if victim won.
  bool attack(Trainer& victim);

  // Tries to catch a Pokemon.
  //
  // @param pokemon the Pokemon the trainer wishes to catch.
  // @return true if the attempt succeeded.
  bool TryToCatch(const Pokemon& pokemon);

  // Tries to take an item.
  //
  // @param item the item the trainer wishes to take.
  // @return true if the attempt succeeded.
  bool TryToTake(const Item& item);

  // initiallize all teams score to 0
  static void InitiallizeTeamsScore();

  // Prints the data of the trainer in the following format:
  //
  //     "<name> (<level>) <team>\n
  //      <pokemon1>\n
  //      <pokemon2>\n
  //      ..."
  //
  // where the Pokemons are ordered from least recently caught to most recently.
  // e.g.
  // 
  //     ash (4) YELLOW
  //     pikachu (3/5.5/87) NORMAL FIRE FLYING
  //     charmander (2/2.5/100) NORMAL BUG
  //     pikachu (7/6.5/56) FIRE
  // 
  //
  // @param output the output stream to which the data is printed.
  // @param pokemon the pokemon whose data is printed.
  // @return the output stream to which the data is printed.
  friend std::ostream& operator<<(std::ostream& output, const Trainer& trainer);
};

std::ostream& operator<<(std::ostream& output, const Trainer& trainer);

}  // namespace mtm
}  // namespace pokemongo

#endif  // TRAINER_H
