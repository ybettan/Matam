#include <string.h>
#include "../test_utilities.h"
#include "../pokemon.h"

static bool testPokemonCreateDestroy(){

	PokemonResult result;

	ASSERT_TEST(pokemonCreate(NULL, 0, 0, &result) == NULL);
	ASSERT_TEST(result == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonCreate("pikachu", 0, 0, &result) == NULL);
	ASSERT_TEST(result == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonCreate("pikachu", 1, 0, &result) == NULL);
	ASSERT_TEST(result == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonCreate("pikachu", 0, 1, &result) == NULL);
	ASSERT_TEST(result == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonCreate(NULL, 1, 1, &result) == NULL);
	ASSERT_TEST(result == POKEMON_NULL_ARGUMENT);
	Pokemon pokemon =pokemonCreate("pikachu", 1, 1, &result);
	ASSERT_TEST(pokemon != NULL);
	ASSERT_TEST(result == POKEMON_SUCCESS);
	pokemonDestroy(pokemon);

	return true;
}

static bool testPokemonGetSetParameters(){
	PokemonResult result;
	Pokemon pokemon = pokemonCreate("charizard", 18, 1, &result);
	ASSERT_TEST(pokemon != NULL);
	ASSERT_TEST(result == POKEMON_SUCCESS);

	char* species;
	int pokemon_cp, pokemon_level;
	double pokemon_hp;
	pokemonGetParameters(pokemon, &species, &pokemon_cp, &pokemon_hp,
			&pokemon_level);
	ASSERT_TEST(strcmp(species, "charizard") == 0);
	ASSERT_TEST(pokemon_cp == 18);
	ASSERT_TEST(pokemon_hp == 100);
	ASSERT_TEST(pokemon_level == 1);

	ASSERT_TEST(pokemonGetParameters(NULL, &species, &pokemon_cp, &pokemon_hp,
				&pokemon_level) == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonGetParameters(pokemon, NULL, &pokemon_cp, &pokemon_hp,
				&pokemon_level) == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, NULL, &pokemon_hp,
					&pokemon_level) == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp, NULL,
						&pokemon_level) == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, NULL) == POKEMON_NULL_ARGUMENT);



	ASSERT_TEST(pokemonSetParameters(pokemon, 42, 3) ==
			POKEMON_SUCCESS);
	pokemonGetParameters(pokemon, &species, &pokemon_cp, &pokemon_hp,
			&pokemon_level);
	ASSERT_TEST(pokemon_hp == 42);
	ASSERT_TEST(pokemon_level == 3);


	ASSERT_TEST(pokemonSetParameters(pokemon, 100, 3) ==
			POKEMON_SUCCESS);
	ASSERT_TEST(pokemonSetParameters(pokemon, 101, 3) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonSetParameters(pokemon, -1, 3) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonSetParameters(pokemon, -0.5, 3) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonSetParameters(pokemon, 42, 0) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonSetParameters(pokemon, 42, -1) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonSetParameters(pokemon, 42, 4000) ==
			POKEMON_SUCCESS);
	pokemonDestroy(pokemon);


	return true;
}

static bool testPokemonIsPokemonAliveHealGetHp(){

	PokemonResult result;
	Pokemon pokemon = pokemonCreate("charizard", 18, 1, &result);
	ASSERT_TEST(pokemon != NULL);
	ASSERT_TEST(result == POKEMON_SUCCESS);

	ASSERT_TEST(pokemonSetParameters(pokemon, 0, 22) == POKEMON_SUCCESS);
	ASSERT_TEST(isPokemonAlive(pokemon) == false);
	ASSERT_TEST(pokemonSetParameters(pokemon, 22, 22) == POKEMON_SUCCESS);
	ASSERT_TEST(isPokemonAlive(pokemon) == true);

	char* species;
	int pokemon_cp, pokemon_level;
	double pokemon_hp;
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetHp(pokemon) == 22);
	ASSERT_TEST(pokemonHeal(pokemon, 28) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetHp(pokemon) == 50);
	ASSERT_TEST(pokemonHeal(pokemon, 51) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetHp(pokemon) == 100);
	ASSERT_TEST(pokemonHeal(pokemon, 1) == POKEMON_HP_IS_AT_MAX);
	ASSERT_TEST(pokemonGetHp(pokemon) == 100);

	ASSERT_TEST(pokemonSetParameters(pokemon, 22, 22) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetHp(pokemon) == 22);
	ASSERT_TEST(pokemonHeal(NULL, 28) == POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonHeal(pokemon, -1) == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonHeal(pokemon, 0) == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonHeal(pokemon, 0.5) == POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonHeal(pokemon, 1) == POKEMON_SUCCESS);

	ASSERT_TEST(pokemonSetParameters(pokemon, 99, 22) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonHeal(pokemon, 100) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetHp(pokemon) == 100);
	ASSERT_TEST(pokemonSetParameters(pokemon, 99, 22) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonHeal(pokemon, 1) == POKEMON_SUCCESS);
	ASSERT_TEST(isPokemonAlive(pokemon) == true);

	pokemonDestroy(pokemon);

	return true;
}

static bool testPokemonEvolveTrainSetParameters(){

	PokemonResult result;

	Pokemon pokemon = pokemonCreate("first_name", 18, 1, &result);
	ASSERT_TEST(pokemon != NULL);
	char* species;
	int pokemon_cp, pokemon_level;
	double pokemon_hp;
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_cp == 18);
	ASSERT_TEST(isPokemonTrained(pokemon, 10) == true);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_cp == 28);

	ASSERT_TEST(isPokemonTrained(pokemon, 0) == false);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_cp == 28);

	ASSERT_TEST(isPokemonTrained(pokemon, -1) == false);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_cp == 28);

	ASSERT_TEST(isPokemonTrained(NULL, 1) == false);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_cp == 28);

	ASSERT_TEST(isPokemonTrained(pokemon, 1) == true);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemon_level ==1);
	ASSERT_TEST(pokemon_cp == 29);
	ASSERT_TEST(strcmp(species, "first_name") == 0);

	ASSERT_TEST(pokemonEvolve(pokemon, "second_name",1, 20) ==
			POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(strcmp(species, "second_name")==0);
	ASSERT_TEST(pokemon_cp == 31);//pokemon evolved and gained 11 candyCp +20.
	ASSERT_TEST(pokemonEvolve(pokemon, "third_name",2, 20)
			== POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(strcmp(species, "third_name") != 0);
	ASSERT_TEST(pokemonEvolve(pokemon, "fourth_name",2, 40)
			== POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(strcmp(species, "fourth_name") != 0);
	ASSERT_TEST(pokemon_cp != 51);
	ASSERT_TEST(pokemonSetParameters(pokemon, 100, 2) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonEvolve(pokemon, "fourth_name",2, 40)
			== POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(strcmp(species, "fourth_name") == 0);
	ASSERT_TEST(pokemon_cp == 51);
	ASSERT_TEST(isPokemonTrained(pokemon, 29) == true);
	ASSERT_TEST(pokemonEvolve(pokemon, "fifth_name",2, 60)
			== POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(strcmp(species, "fifth_name") == 0);
	ASSERT_TEST(pokemon_cp == 100);

	ASSERT_TEST(pokemonEvolve(NULL, "sixth_name" ,2, 60) ==
			POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonEvolve(pokemon, NULL, 2, 60) ==
			POKEMON_NULL_ARGUMENT);
	ASSERT_TEST(pokemonEvolve(pokemon, "sixth_name" ,-1, 60) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonEvolve(pokemon, "sixth_name" ,0, 60) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonEvolve(pokemon, "sixth_name" ,2, -1) ==
			POKEMON_INVALID_ARGUMENT);
	ASSERT_TEST(pokemonEvolve(pokemon, "sixth_name" ,2, 0) ==
			POKEMON_INVALID_ARGUMENT);

	pokemonDestroy(pokemon);

	return true;
}


static bool testPokemonCopy(){

	PokemonResult result;

	Pokemon pokemon = pokemonCreate("first_name", 18, 1, &result);
	ASSERT_TEST(pokemon != NULL);
	ASSERT_TEST(result == POKEMON_SUCCESS);

	Pokemon copy = pokemonCopy(pokemon);
	ASSERT_TEST(copy != NULL);

	ASSERT_TEST(pokemonCopy(NULL) == NULL);


	char* species;
	int pokemon_cp, pokemon_level;
	double pokemon_hp;

	char* copy_species;
	int copy_pokemon_cp, copy_pokemon_level;
	double copy_pokemon_hp;


	ASSERT_TEST(pokemonGetParameters(pokemon, &species, &pokemon_cp,
			&pokemon_hp, &pokemon_level) == POKEMON_SUCCESS);
	ASSERT_TEST(pokemonGetParameters(copy, &copy_species, &copy_pokemon_cp,
			&copy_pokemon_hp, &copy_pokemon_level) == POKEMON_SUCCESS);

	ASSERT_TEST(strcmp(species, copy_species) == 0);
	ASSERT_TEST(pokemon_cp == copy_pokemon_cp);
	ASSERT_TEST(pokemon_level == copy_pokemon_level);
	ASSERT_TEST(pokemon_hp == copy_pokemon_hp);

	pokemonDestroy(pokemon);
	pokemonDestroy(copy);

	return true;
}


int main(){
	RUN_TEST(testPokemonCreateDestroy);
	RUN_TEST(testPokemonGetSetParameters);
	RUN_TEST(testPokemonIsPokemonAliveHealGetHp);
	RUN_TEST(testPokemonEvolveTrainSetParameters);
	RUN_TEST(testPokemonCopy);
	return 0;
}
