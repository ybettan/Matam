

#ifndef POKEMON_POKEMON_H_
#define POKEMON_POKEMON_H_
#include <stdbool.h>
#include <stdio.h>

typedef struct pokemon_t* Pokemon;

typedef enum{
	POKEMON_OUT_OF_MEMORY,
	POKEMON_NULL_ARGUMENT,
	POKEMON_INVALID_ARGUMENT,
	POKEMON_HP_IS_AT_MAX,
	POKEMON_SUCCESS
}PokemonResult;


/* creates a new pokemon with a copy of species given and with initial
 * cp and id given. the pokemon will be created with 100 hp and level =1 and
 * pokemon's id = given id.
 * ----return-----
 * return NULL if given species or result are NULL, or if cp or index are not
 * positive, or if any allocation failed.
 * the reason for NULL return is written into result
 * argument (null\invalid_arg or out_of_mem). otherwise returns new pokemon.
 */

Pokemon pokemonCreate(const char* species, int cp, int id,
		PokemonResult* result);


/* free all memory kept in pokemon and free pokemon.
 * if given NULL then does nothing.
 */

void pokemonDestroy(Pokemon pokemon);


/* given a pokemon, the function writes to the given
 * pointers the following pokemon's attributes: species, cp, hp and level.
 * ----return-----
 * POKEMON_NULL_ARGUMENT - if any pointer given was NULL.
 * POKEMON_SUCCESS - otherwise.
 */

PokemonResult pokemonGetParameters(Pokemon pokemon,
		char** pokemon_species, int* pokemon_cp, double* pokemon_hp,
		int* pokemon_level);


/* ----return-----
 * returns -1 if pokemon is NULL.
 * else returns given pokemon hp.
 */

double pokemonGetHp(Pokemon pokemon);


/* function sets given pokemon's hp and level.
 * ----return-----
 * POKEMON_NULL_ARGUMENT - if pokemon is NULL .
 * POKEMON_INVALID_ARGUMENT - if level <= 0, or hp is not in between 0 and 100.
 * POKEMON_SUCCESS - otherwise.
 */

PokemonResult pokemonSetParameters(Pokemon pokemon, double new_hp,
		int new_level);


/* returns true if the given pokemon's hp > 0.
 * returns false if pokemon is NULL or if pokemon_hp == 0.
 */

bool isPokemonAlive(Pokemon pokemon);


/* the function checks if the pokemon given current level is >= evolve_level.
 * if so, it copies the species given as an argument and gives it to pokemon.
 * pokemon previous species is destroyed.
 * sets: pokemon cp=  cp  pokemon gained by candies + new_cp given.
 * ----return-----
 * POKEMON_NULL_ARGUMENT - if given pokemon or new_species are NULL.
 * POKEMON_OUT_OF_MEMORY - if allocation for new species failed.
 * POKEMON_INVALID_ARGUMENT - if evolve_level <= 0, or new cp <= 0.
 * POKEMON_SUCCESS - if none above happen. success returned also if no evolve
 * happend.
 */

PokemonResult pokemonEvolve(Pokemon pokemon, const char* new_species,
		int evolve_level, int new_cp);


/* checks if the given pokemon has 100 hp.
 * if doesn't function gives pokemon addition to hp as value given.
 * if pokemon hp after addition ais above legal then pokemon gets maximum
 * hp (=100).
 * ----return-----
 * POKEMON_NULL_ARGUMENT - if pokemon given is NULL.
 * POKEMON_INVALID_ARGUMENT - id value given is <= 0.
 * POKEMON_HP_IS_AT_MAX - if pokemon had 100 hp.
 * POKEMON_SUCCESS - if pokemon was healed.
 */

PokemonResult pokemonHeal(Pokemon pokemon, int value);


/* try to train a pokemon. pokemon new cp is oldest_cp + new cp.
 * return false if given pokemon is null or if candy_value is not positive
 * number.
 * if trained, pokemons candy_cp grows by candy_value given.
 * returns true if pokemon was successfully train.
 * false otherwise.
 */

bool isPokemonTrained(Pokemon pokemon, int candy_value);


/* the function copies a given pokemon and return the copy.
 * returns NULL if allocation for copy failed or if pokemon given as
 * an argument is NULL.
 * otherwise returns the copy.
 */

Pokemon pokemonCopy(Pokemon pokemon);


/* prints the info of the pokemon: his id, species, hp, cp and level to a given
 * open file.
 * ----return-----
 * POKEMON_NULL_ARGUMENT - if pokemon is NULL.
 * POKEMON_SUCCESS - if printing was successful.
 */

PokemonResult pokemonShowInfo(FILE* output_file, Pokemon pokemon);


#endif /* POKEMON_POKEMON_H_ */
