
#include "pokemon.h"
#include "print_utils.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//=============================================================================
//								MACROS
//=============================================================================

#define MAX_HP 100
#define MIN_HP 0
#define MIN_LEGAL_ATTRIBUTE 1
#define ILLEGAL_VALUE -1

//=============================================================================
//								STRUCT
//=============================================================================


struct pokemon_t{
	char* species;
	int id, cp , candy_cp, level;
	double hp;
};


//=============================================================================
//							  STATIC FUNCTION
//=============================================================================


static Pokemon pokemonAllocation(const char* species){

	Pokemon pokemon = malloc(sizeof(*pokemon));
	if(!pokemon) return NULL;
	pokemon->species = malloc(strlen(species) + 1);
	if(!pokemon->species){
		free(pokemon);
		return NULL;
	}
	return pokemon;
}


//=============================================================================
//					  		HEADER FUNCTION
//=============================================================================


Pokemon pokemonCreate(const char* species, int cp, int id,
		PokemonResult* result){

	if(!result) return NULL;   //will never happen.
	if(!species){

		*result = POKEMON_NULL_ARGUMENT;
		return NULL;
	}
	if(id < MIN_LEGAL_ATTRIBUTE || cp < MIN_LEGAL_ATTRIBUTE){
		*result = POKEMON_INVALID_ARGUMENT;
		return NULL;
	}
	Pokemon pokemon = pokemonAllocation(species);
	if(!pokemon){
		*result = POKEMON_OUT_OF_MEMORY;
		return NULL;
	}
	strcpy(pokemon->species, species);
	pokemon->candy_cp = 0;
	pokemon->cp = cp;
	pokemon->hp = MAX_HP;
	pokemon->id = id;
	pokemon->level = MIN_LEGAL_ATTRIBUTE;
	*result = POKEMON_SUCCESS;
	return pokemon;
}



void pokemonDestroy(Pokemon pokemon){

	if (!pokemon) return;
	free(pokemon->species);
	free(pokemon);
}



PokemonResult pokemonGetParameters(Pokemon pokemon,
		char** pokemon_species, int* pokemon_cp, double* pokemon_hp,
		int* pokemon_level){

	if(!pokemon || !pokemon_species || !pokemon_cp || !pokemon_hp ||
			!pokemon_level) return POKEMON_NULL_ARGUMENT;

	*pokemon_species = pokemon->species;
	*pokemon_cp = pokemon->cp;
	*pokemon_hp = pokemon->hp;
	*pokemon_level = pokemon->level;
	return POKEMON_SUCCESS;
}



double pokemonGetHp(Pokemon pokemon){

	if(!pokemon) return ILLEGAL_VALUE;
	return pokemon->hp;
}



PokemonResult pokemonSetParameters(Pokemon pokemon, double new_hp,
		int new_level){

	if(!pokemon) return POKEMON_NULL_ARGUMENT;
	if(new_level < MIN_LEGAL_ATTRIBUTE || new_hp < MIN_HP ||
			new_hp > MAX_HP) return POKEMON_INVALID_ARGUMENT;

	pokemon->hp = new_hp;
	pokemon->level = new_level;
	return POKEMON_SUCCESS;
}



bool isPokemonAlive(Pokemon pokemon){

	if (!pokemon || pokemon->hp == MIN_HP) return false;
	return true;
}



PokemonResult pokemonEvolve(Pokemon pokemon, const char* new_species,
		int evolve_level, int new_cp){

	if(!pokemon || !new_species) return POKEMON_NULL_ARGUMENT;
	if(evolve_level < MIN_LEGAL_ATTRIBUTE || new_cp < MIN_LEGAL_ATTRIBUTE)
		return POKEMON_INVALID_ARGUMENT;
	if(pokemon->level >= evolve_level){
		char* evolved_species = malloc(strlen(new_species)+1);
		if(!evolved_species) return POKEMON_OUT_OF_MEMORY;
		strcpy(evolved_species, new_species);
		free(pokemon->species);
		pokemon->species = evolved_species;
		pokemon->cp = pokemon->candy_cp + new_cp;
	}
	return POKEMON_SUCCESS;
}



PokemonResult pokemonHeal(Pokemon pokemon, int value){

	if(!pokemon) return POKEMON_NULL_ARGUMENT;
	if(value < MIN_LEGAL_ATTRIBUTE)return POKEMON_INVALID_ARGUMENT;
	if(pokemon->hp == MAX_HP) return POKEMON_HP_IS_AT_MAX;
	pokemon->hp += value;
	if(pokemon->hp > MAX_HP){
		pokemon->hp = MAX_HP;
	}
	return POKEMON_SUCCESS;
}



bool isPokemonTrained(Pokemon pokemon, int candy_value){

	if(!pokemon || candy_value < MIN_LEGAL_ATTRIBUTE) return false;
	pokemon->cp += candy_value;
	pokemon->candy_cp += candy_value;
	return true;
}



Pokemon pokemonCopy(Pokemon pokemon){
	if(!pokemon) {
		return NULL;
	}
	Pokemon copy = pokemonAllocation(pokemon->species);
	if(!copy) return NULL;
	strcpy(copy->species, pokemon->species);
	copy->id = pokemon->id;
	copy->hp = pokemon->hp;
	copy->cp = pokemon->cp;
	copy->candy_cp = pokemon->candy_cp;
	copy->level = pokemon->level;
	return copy;
}



PokemonResult pokemonShowInfo(FILE* output_file, Pokemon pokemon){

	if(!output_file || !pokemon) return POKEMON_NULL_ARGUMENT;
	mtmPrintPokemon(output_file, pokemon->id, pokemon->species, pokemon->hp,
			pokemon->cp, pokemon->level);
	return POKEMON_SUCCESS;
}


