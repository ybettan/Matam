
#ifndef POKEMON_GO_H_
#define POKEMON_GO_H_
#include <stdlib.h>


// a pointer to pokemonGo_t struct.

typedef struct PokemonGo_t* PokemonGo;


/* pokemon types declaration
 * ()   : BUG,GROUND,NORMAL,GROUND,GHOST,PSYCHIC
 * (*)  : ROCK,ELECTRIC,WATER,FAIRY,ICE
 * (**) : FIRE,FLYING,POISON
 */


/* possible errors.
 * in each function , error should be returned by the
 *   first error in the given order:
 */

typedef enum {
	POKEMONGO_OUT_OF_MEMORY,
	POKEMONGO_NULL_ARGUMENT,
	POKEMONGO_INVALID_ARGUMENT,
	POKEMONGO_TRAINER_NAME_ALREADY_EXISTS,
	POKEMONGO_TRAINER_DOES_NOT_EXIST,
	POKEMONGO_LOCATION_DOES_NOT_EXIST,
	POKEMONGO_POKEMON_DOES_NOT_EXIST,
	POKEMONGO_ITEM_OUT_OF_STOCK,
	POKEMONGO_BUDGET_IS_NOT_SUFFICIENT,
	POKEMONGO_HP_IS_AT_MAX,
	POKEMONGO_NO_AVAILABLE_ITEM_FOUND,
	POKEMONGO_TRAINER_ALREADY_IN_LOCATION,
	POKEMONGO_LOCATION_IS_NOT_REACHABLE,
	POKEMONGO_SUCCESS
} PokemonGoResult;



/* create a new PokemonGo world and initialize it according to the given files.
 * ----Return-----
 * NULL for any memory allocation failure or if any argument given is NULL.
 * a pointer to PokemonGo otherwise.
 */

PokemonGo pokemonGoCreate(FILE* pokedex,FILE* evolutions,FILE* locations,
		FILE* output_file);


/* releases all the memory allocated for pokemon_go.
 */

void pokemonGoDestroy(PokemonGo pg);


/* adds a new trainer to the game. the location of trainer is start-point given.
 * the trainer will try to hunt the first pokemon in the list (if exists) at
 * start point. if caught, the pokemon is given an index = 1.
 * function uses print_utils.h to print catch result.
 * ----Return-----
 * POKEMONGO_OUT_OF_MEMORY - if any allocation failed.
 * POKEMONGO_NULL_ARGUMENT - if any pointer of arguments is NULL.
 * POKEMONGO_INVALID_ARGUMENT - if budget is not an integer >= 0.
 * POKEMONGO_TRAINER_ALREADY_EXISTS - if trainer with given name already exist.
 * POKEMONGO_LOCATION_DOES_NOT_EXIST - if location as given dont exist in the
 * game.
 * POKEMONGO_SUCCESS - if a new trainer successfully added, and if in location
 * was a species to catch and hunt was successful.
 */

PokemonGoResult pokemonGoAddTrainer(PokemonGo pg, char* name, int budget,
		char* start_point);


/* function try making a given trainer move on the map to a given destination.
 * a hunt will be tried and result will be printed by print_utils.h.
 * if hunt made, the hunted pokemon gets index = the number of catches made by
 * his new master.
 * -----Return------
 * POKEMONGO_OUT_OF_MEMORY - if any allocation failed.
 * POKEMONGO_NULL_ARGUMENT - if any pointer given is NULL.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - if the given trainer does not exist.
 * POKEMONGO_LOCATION_DOES_NOT_EXIST - if no location as given in the game.
 * POKEMONGO_LOCATION_IS_NOT_REACHABLE - if given destination is not reachable
 * from current trainer's location.
 * POKEMONGO_TRAINER_ALREADY_IN_LOCATION - if the trainer is already in
 * location.
 * POKEMONGO_SUCCESS - if move was successful and if in new location was a
 * species to catch and hunt was successful.
 */

PokemonGoResult pokemonGoTrainerMoveAndHunt(PokemonGo pg, char* name,
		char* destination);


/* adds an item of type "potion" or "candy" to the store. item value and
 * quantity are specified in argument.
 * -----Return-------
 *  POKEMONGO_OUT_OF_MEMORY - if allocation faile.
 *  POKEMONGO_NULL_ARGUMENT - if received any NULL argument.
 *  POKEMONGO_INVALID_ARGUMENT	- if type isn't "potion" or "candy" or
 *  if quantity/value <= 0
 *  POKEMONGO_SUCCESS - if item with given type quantity and value was
 *  successfully added to the game store.
 */

PokemonGoResult pokemonGoStoreAddItem(PokemonGo pg , char* type, int value,
		int quantity);


/* function makes a given trainer purchase an item  with type "potion" or
 * "candy" from the store.
 * if purchase is successful an item will be added to trainers stock,
 * and trainers pokeCoins reduced bt the price of item added.
 * if added "potion"- price = value.
 * id added "candy" = price = value*2.
 * ----Return------
 * POKEMONGO_OUT_OF_MEMORY - if any allocation failed.
 * POKEMONGO_NULL_ARGUMENT - if received any NULL argument.
 * POKEMONGO_INVALID_ARGUMENT	- if item isn't of type "potion" or "candy",
 * or if value <= 0.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - if the given trainer does not exist.
 * POKEMONGO_ITEM_OUT_OF_STOCK - if the item with type and value given
 * is not in store's stock.
 * POKEMONGO_BUDGET_IS_NOT_SUFFICIENT - if trainer doesn't have enough
 * pokeCoins to seal the deal.
 * POKEMONGO_SUCCESS - if store successfully sold the item and trainers.
 * successfully added the item to his stock.
 */

PokemonGoResult pokemonGoTrainerPurchase(PokemonGo pg, char* name ,char* type,
		int value);


/* function makes a battle between 2 pokemons of 2 trainers.
 * two pokemons picked are those related to the indexes given as an argument.
 * for each trainer/pokemon the next values will be updated after battle
 * according to described in exercise:
 *  	-pokemon level.
 *  	-pokemon HP.
 *  	-trainer XP.
 *  uses print_utils.h to print the battle result.
 *  at the end of battle:
 *  	- if some battled pokemon HP = 0 he is removed from the game.
 *  	- battled pokemon evolves if reached a certain level.
 *  -----Return------
 * POKEMONGO_NULL_ARGUMENT - if received any NULL argument.
 * POKEMONGO_INVALID_ARGUMENT - if trainer1 == trainer2.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - if one of the trainer does't exist in
 * game.
 * POKEMONGO_POKEMON_DOES_NOT_EXIST - if one of the trainers don't have
 * a pokemon related to index given.
 * POKEMONGO_SUCCESS - if battle was complete and in case of evolve/die all was
 * successful.
 */

PokemonGoResult pokemonGoBattle(PokemonGo pg, char* name1, char* name2,
		int index1 ,int index2);


/* function makes a trainer with a given name heal one of his
 * pokemons (determined by index given) by using a potion from his stock.
 * the potion has to be optimal, therefore the trainer will use the minimal
 * potion that brings pokemon's HP to 100.
 * if such potion does not exist in trainers stock then the maximal potion in
 * trainer's stock will be picked.
 * ----Return-----
 * POKEMONGO_NULL_ARGUMENT - if one of pointers given is NULL.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - if trainer does't exist in the game.
 * POKEMONGO_POKEMON_DOES_NOT_EXIST - if trainer doesn't have any pokemon
 * related to the given index.
 * POKEMONGO_HP_IS_AT_MAX - if the pokemon to heal already have HP = 100.
 * POKEMONGO_NO_AVAILABLE_ITEM_FOUND - if trainer doesn't have any potion in
 * stock.
 * POKEMONGO_SUCCESS - if the pokemon is successfully healed.
 */

PokemonGoResult pokemonGoPokemonHeal(PokemonGo pg, char* name, int index);


/* the function makes a trainer with a given name uses his maximal valued candy
 * to increase the CP of the pokemon related to index given.
 * ----Return-----
 * POKEMONGO_NULL_ARGUMENT - if any pointer given is NULL.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - trainer does't exist in the game
 * POKEMONGO_POKEMON_DOES_NOT_EXIST - if trainer doesn't have any pokemon
 * related to the given index.
 * POKEMONGO_NO_AVAILABLE_ITEM_FOUND - if trainer doesn't have any candy in
 * stock.
 * POKEMONGO_SUCCESS - if the pokemon is successfully healed.
 */

PokemonGoResult pokemonGoPokemonTrain(PokemonGo pg, char* name, int index);


/* the function prints trainer's info by using print_utils.h.
 * printing will occur as follows:
 * first prints all trainer's candies : by value in increasing order.
 * secondly prints all trainer's potion : by value in increasing order.
 * finally prints all trainer's pokemons : by id in increasing order.
 * -----Return------
 * POKEMONGO_NULL_ARGUMENT - if any pointer given is NULL.
 * POKEMONGO_TRAINER_DOES_NOT_EXIST - if trainer doesn't exist
 * POKEMONGO_SUCCESS - otherwise.
 */

PokemonGoResult pokemonGoShowTrainerInfo(PokemonGo pg, char* name);


/* the function prints map info in increasing lexicographic order, using
 * print_utils.h :
 * in each location the first pokemon in location's list of pokemons
 *  will be printed.
 * -----Return------
 * POKEMONGO_NULL_ARGUMENT - if got NULL as an argument.
 * POKEMONGO_SUCCESS - otherwise.
 */

PokemonGoResult pokemonGoShowMapInfo(PokemonGo pg);


/* function prints store's info  increasing order using print_utils.h:
 * first prints all store's candies : by value in increasing order.
 * secondly prints all store's potion : by value in increasing order.
 * -----Return------
 * POKEMONGO_NULL_ARGUMENT - if got NULL as an argument.
 * POKEMONGO_SUCCESS - otherwise.
 */

PokemonGoResult pokemonGoShowStoreInfo(PokemonGo pg);



#endif /* POKEMON_GO_H_ */
