/*
 * trainer.h
 *
 *  Created on: 17 בדצמ× 2016
 *      Author: user
 */

#ifndef TRAINER_TRAINER_H_
#define TRAINER_TRAINER_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct trainer_t* Trainer;

typedef enum{
	TRAINER_OUT_OF_MEMORY,
	TRAINER_NULL_ARGUMENT,
	TRAINER_INVALID_ARGUMENT,
	TRAINER_POKEMON_DOES_NOT_EXIST,
	TRAINER_BUDGET_IS_NOT_SUFFICIENT,
	TRAINER_HP_IS_AT_MAX,
	TRAINER_NO_AVAILABLE_ITEM_FOUND,
	TRAINER_TRAINER_ALREADY_IN_LOCATION,
	TRAINER_SUCCESS
}TrainerResult;


/* the function creates a new trainer. it copies given name and location_name.
 * trainer starts with number of pokeCoins specified in argument.
 * initial trainer's xp=1.
 * ----return-----
 * return NULL in following cases:
 * if given name or location_name are NULL, or if any memory allocation failed,
 * or if budget < 0.
 * if returns NULL the reason will be written in result argument.
 * result will contain: TRAINER_OUT_OF_MEMORY if any allocation failed,
 * TRAINER_NULL_ARGUMENT if received NULL as arg andTRAINER_INVALID_ARGUMENT -
 * if argument are invalid.
 * otherwise returns trainer.
 */

Trainer trainerCreate(const char* name, const char* location, int budget,
		TrainerResult* result);


/* free all memory associated with the trainer, and free the trainer itself.
 * if given NULL does nothing.
 */

void trainerDestroy(Trainer trainer);


/* function changes trainer location.
 * it copies given new_location and free previous location.
 * ----return-----
 * TRAINER_OUT_OF_MEMORY - if allocation for new_location or pokemon failed.
 * TRAINER_NULL_ARGUMENT - if given trainer or new_location are NULL.
 * TRAINER_TRAINER_ALREADY_IN_LOCATION - if given location == trainer's
 * location.
 * TRAINER_SUCCESS- if move was successful
 */

TrainerResult trainerMove(Trainer trainer,const char* new_location);  //calls static:


/* function makes trainer hunt a pokemon with given species and cp.
 * the pokemon will be added to trainers list of pokemons with id = number of
 * pokemon caught by trainer. as hunt is always successful, trainer gains an
 * amount of pokeCoins specified in hunt_bonus argument.
 * ----return-----
 * TRAINER_OUT_OF_MEMORY - if any allocation failed while creating a pokemon.
 * TRAINER_NULL_ARGUMENT - if any pointer is NULL.
 * TRAINER_INVALID_ARGUMENT - if pokemon cp <= 0, or
 * if hunt_bonus < 0.
 * TRAINER_SUCCESS - if the trainer hunted a pokemon which added successfully
 * to trainer's list of pokemons.
 */

TrainerResult trainerHunt(Trainer trainer, const char* pokemon_species,
		int pokemon_cp, int hunt_bonus);


/* the function adds item of type "candy" or "potion" with a given value to
 * the trainer. if item was successfully added then trainer's pokeCoins are
 *  reduced by the price of the item( potion-price=value, candy-price=value*2).
 * ----return-----
 * TRAINER_OUT_OF_MEMORY - if allocation for new item failed.
 * TRAINER_NULL_ARGUMENT - if trainer or type are NULL.
 * TRAINER_INVALID_ARGUMENT - if type isn't "candy" or "potion",
 * or if value <= 0.
 * TRAINER_BUDGET_IS_NOT_SUFFICIENT - if trainer doesn't have the amount of
 * pokeCoins needed for the purchase.
 * TRAINER_SUCCESS - if adding item was successful.
 */

TrainerResult trainerAddItem(Trainer trainer,const char* type, int value);


/* the function returns the given trainer's location.
 *	----return-----
 *	return NULL if trainer is NULL.
 *	otherwise returns trainer's location.
 */

char* trainerGetLocation(Trainer trainer);


/* function returns true if trainer given has a pokemon related to the given
 * index. otherwise returns false.
 */

bool isTrainerContainsPokemon(Trainer trainer, int index);


/* the function writes to the given pointers the info of the trainer and its
 * pokemon's attributes that his id = index.
 * after function finish (if successful) the given pointers will contain:
 * -trainer's xp.
 * -the pokemon related to given index species, cp, hp and level.
 * ----return-----
 * TRAINER_NULL_ARGUMENT - if any pointer is NULL.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if trainer has no pokemon
 *  with id = index given.
 * TRAINER_SUCCESS - if got all the info successfully.
 */

TrainerResult trainerGetParameters(Trainer trainer, double* trainers_xp,
		int index, char** pokemon_species, int* pokemon_cp, double* pokemon_hp,
		int* pokemon_level);


/* the function removes a pokemon from the game if its hp=0.
 * the pokemon removed is the pokemon related to the given index.
 * ----return-----
 * TRAINER_INVALID_ARGUMENT - if trainer is NULL.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if index <= 0 , or if trainer doesn't have
 * a pokemon with id = index.
 * TRAINER_SUCCESS - otherwise. returned success also if pokemon is not removed.
 */

TrainerResult trainerRemovePokemonIfDead(Trainer trainer, int index);


/* the function checks pokemon related to the given index
 * level is >= evolve_level. if so, changes pokemon's species to
 * the evolved species of the pokemon (pokemon previous species is destroyed).
 * updates pokemon's cp by adding cp which pokemon gained by candies to the new
 * cp given.
 * TRAINER_OUT_OF_MEMORY - if allocations for new_species failed.
 * TRAINER_NULL_ARGUMENT - if new_species or trainer are NULL.
 * TRAINER_INVALID_ARGUMENT - if evolve_level <= 0, or new_cp <= 0.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if index <= 0 , or if trainer doesn't have
 * a pokemon with id = index.
 * TRAINER_SUCCESS - otherwise. success returned whether or not evolve made.
 */

TrainerResult trainerEvolvePokemon(Trainer trainer, int index,
		int evolve_level, const char* new_species, int new_cp);


/* sets trainer's xp and the index related pokemon's hp and level.
 * ----return-----
 * TRAINER_NULL_ARGUMENT - if trainer is NULL.
 * TRAINER_INVALID_ARGUMENT - if trainer is NULL, or xp is <=0,
 * or if new hp is not between 0 and 100, or if level <= 0.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if trainer has no pokemon
 *  with id = index given.
 * TRAINER_SUCCESS - if set all the parameters successfully.
 */

TrainerResult trainerSetParameters(Trainer trainer, double new_xp, int index,
		double new_hp, int new_level);


/* the function checks if trainer's index related pokemon can be healed.
 * if so finds the most appropriate potion among the given trainer's items.
 * the appropriate potion is decided as follows:
 * if trainer got a potion with the exact value to bring his pokemon to 100 hp,
 * this potion will be picked. if no such potion, the best potion of the
 * trainer is picked.
 * ----return-----
 * TRAINER_NULL_ARGUMENT - if trainer is NULL.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if trainer has no pokemon
 *  with id = index given.
 * TRAINER_HP_IS_AT_MAX - if given pokemon's hp is 100 before healing.
 * TRAINER_NO_AVAILABLE_ITEM_FOUND - if trainer has no potions.
 * TRAINER_SUCCESS - if the pokemon related to the given index was healed.
 */

TrainerResult trainerPokemonHeal(Trainer trainer, int index);


/* the function train the trainer's pokemon related to the given index.
 * in order to train the pokemon well, the highest value candy in trainer's
 * items will be picked.
 * ----return-----
 * TRAINER_NULL_ARGUMENT - if trainer is NULL.
 * TRAINER_POKEMON_DOES_NOT_EXIST - if trainer has no pokemon
 *  with id = index given.
 * TRAINER_NO_AVAILABLE_ITEM_FOUND - if trainer has no potions.
 * TRAINER_SUCCESS - if the pokemon related to the given index was trained.
 */

TrainerResult trainerPokemonTrain(Trainer trainer, int index);


/* the function copies a given trainer and return the copy.
 * returns NULL if allocation for copy failed or if trainer given as
 * an argument is NULL.
 * otherwise returns copy.
 */

Trainer trainerCopy(Trainer trainer);


/* prints the info of the Trainer:
 * his candies in increasing order,then all his potions in increasing order.
 * then prints trainer's pokemons in increasing chronological catch order.
 * prints using utils_print.h to a given open file.
 * ----return-----
 * TRAINER_NULL_ARGUMENT - if any NULL given.
 * TRAINER_SUCCESS - if printing was successful.
 */

TrainerResult trainerShowInfo(FILE* output_file, Trainer trainer);


#endif /* TRAINER_TRAINER_H_ */
