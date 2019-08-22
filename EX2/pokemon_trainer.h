#ifndef POKEMON_TRAINER_H_
#define POKEMON_TRAINER_H_

#include "pokemon.h"

typedef enum {
	POKEMON_TRAINER_SUCCESS,
	POKEMON_TRAINER_NULL_ARG,
	POKEMON_TRAINER_OUT_OF_MEM,
	POKEMON_TRAINER_INVALID_INDEX,
	POKEMON_TRAINER_PARTY_FULL,
	POKEMON_TRAINER_REMOVE_LAST,
	POKEMON_TRAINER_DEPOSIT_LAST,
	POKEMON_TRAINER_DEPOSIT_FULL
} PokemonTrainerResult;

/*
* The pokemon_trainer structure.
* in a trainer:
* local pokemons have indexes 0--> n-1 (and not 1--> n as described in ex2)
* local pokemons have indexes 0--> m-1 (and not 1--> m as described in ex2)
*/
typedef struct pokemon_trainer_t {
	char* name;
	Pokemon* trainer_pokemons_array;
	Pokemon* proffessor_pokemons_array;
	int max_pokemons_local;
	int max_pokemons_remote;
	int local_counter;
	int remote_counter;

} *PokemonTrainer;


//the function creates a new pokemon trainer with a single pokemon
//the function make a copy of pokemon to be stored in trainer
//max capacitate of pokemon to hold and max capacitaed of pokemon to be stored
//															at the professor
//-----Errors:------
//return NULL in the followed cases:
// - name is NULL or an empty string
// - initial_pokemon is NULL
// - max_num_local <= 0 or max_num_remote <= 0
// - malloc has failed
PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
	int max_num_local, int max_num_remote);

//release the memory allocated for trainer
//if trainer is NULL then the function do nothing
void pokemonTrainerDestroy(PokemonTrainer trainer);

//the function create a new trainer and copy trainer into new_trainer
//the function return a pointer to new_trainer
//-----Errors:-----
//return NULL if trainer is NULL or malloc faild
PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer);

//the function add pokemon to trainer pokemons list
//the number of the new pokemon will be n if the trainer have alredy n-1
//pokemons
//-----Errors:------
//return POKEMON_TRAINER_NULL_ARG if trainer or pokemon is NULL
//return POKEMON_TRAINER_PARTY_FULL if trainer had reached his maximum pokemon
//																	capacity
//return POKEMON_TRAINER_SUCCESS otherwise
PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
	Pokemon pokemon);

//the function returnes a pokemon of his trainer by his index
//it return the pokemon itself and not a copy
//-----Errors:------
//return NULL if trainer is NULL
//return NULL if pokemon_index is invalid (smaller than 1 or greater than
//trainers number of pokemons)
//IMPORTANT NOTE : in the implementation pokemons have indexes 0--> n-1
Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index);

//the function removes pokemon with pokemon_index from trainer
//pokemon index of each pokemon decrease by 1 if pokemon index is greater the
//																"pokemon_index"
//-----Errors:------
//return  POKEMON_TRAINER_NULL_ARG if trainer is NULL
//return POKEMON_TRAINER_INVALID_INDEX if pokemon_index is invalid
//				(smaller than 1 or greater than trainers number of pokemons)
//return POKEMON_TRAINER_REMOVE_LAST if the trainer has onley one pokemon left
//return POKEMON_TRAINER_OUT_OF_MEM if any memory allocation failed in function
//return POKEMON_TRAINER_SUCCESS otherwise
//IMPORTANT NOTE : in the implementation pokemons have indexes 0--> n-1
PokemonTrainerResult pokemonTrainerRemovePokemon(
	PokemonTrainer trainer, int pokemon_index);

//the function transfer the pokemon that corespond to pokemon_index to the
//																proffessor
//the new pokemon index will be m if this trainer had m-1 deposited  at the
//																proffessor
//each pokemon index greater than pokemon_index will decrease by 1
//															(for the trainer)
//------Errors:-------
//return POKEMON_TRAINER_NULL_ARG if trainer is NULL
//return POKEMON_TRAINER_INVALID_INDEX if pokemon_index is invalid
//				(smaller than 1 or greater than trainers number of pokemons)
//return POKEMON_TRAINER_DEPOSIT_LAST if the trainer has onley one pokemon left
//return POKEMON_TRAINER_DEPOSIT_FULL if the trainer has alredy deposidet
//																max_num_remote
//return POKEMON_TRAINER_OUT_OF_MEM if any memory allocation failed
//return POKEMON_TRAINER_SUCCESS otherwise
PokemonTrainerResult pokemonTrainerDepositPokemon(
	PokemonTrainer trainer, int pokemon_index);

//trainer "pull" a pokemon that corespond pokemon_index from the proffessor
//the new pokemon index will be n if this trainer alredy have n-1 pokemons
//each pokemon index greater than pokemon_index will decrease by 1
//														(for the proffessor)
//-----Errors:------
//return POKEMON_TRAINER_NULL_ARG if trainer is NULL
//return POKEMON_TRAINER_INVALID_INDEX if pokemon_index is invalid
//(smaller than 1 or greater than proffessor's number of pokemons)
//return   POKEMON_TRAINER_PARTY_FULL if the trainer had alredy reached his
//																max_num_local
//return POKEMON_TRAINER_OUT_OF_MEM if any memory allocation failed
//return POKEMON_TRAINER_SUCCESS otherwise
PokemonTrainerResult pokemonTrainerWithdrawPokemon(
	PokemonTrainer trainer, int pokemon_index);

//the function return the total number of pokemons of trainer
//								(local pokemons + kept at proffessor pokemon)
//-----Errors:-----
//only assert possible error (assert check that trainer != NULL)
int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer);

//return the pokemon (not a copy) with the highest rank
//							(includes all pokemons - trainer and proffessor)
//trainer pokemons have priority on proffessors pokemon in case of smae ranks
//in each {trainer,pokemon} priority is given to the pokemon with the smallest
//													index in case of same ranks
//-----Errors:-----
//return NULL if trainer is NULL
Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer);

//the function sort the trainer pokemons_list (trainer + proffessor) according
//																to followed:
// 1. their rank
// 2. for pokemons with same rank - trainer's pokemons have priority on
//														proffessors's pokemons
// 3. for pokemons with same ranke and same holder (trainer or proffessor)
//					- priority will be given to the one with the smaller index
//the function will replace the pokemons on their trainer according to this
//																		order
//all the remaining pokemons who didn't had place will be deposit to the
//												proffessor with the same order
//------Errors:------
//return POKEMON_TRAINER_NULL_ARG if trainer is NULL
//return POKEMON_TRAINER_OUT_OF_MEM if any memory allocation failed
//return POKEMON_TRAINER_SUCCESS otherwise
PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer);

//the function check that all trainer_pokemons are with him in index order
//for each pokemon in the the trainer, trainer call him by his name and the
//										pokemon respond according to his voice
//the output will be writen to file
//-----Errors:-----
//return POKEMON_TRAINER_NULL_ARG if trainer or file are NULL
//return POKEMON_TRAINER_OUT_OF_MEM for any memory allocation failure
//retrun POKEMON_TRAINER_SUCCESS othewise
PokemonTrainerResult pokemonTrainerPrintEnumeration(
	PokemonTrainer trainer, FILE* file);

#endif // POKEMON_TRAINER_H_
