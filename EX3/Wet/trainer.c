
#include "trainer.h"
#include "pokemon.h"
#include "stock.h"
#include "print_utils.h"
#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


//=============================================================================
//								MACROS
//=============================================================================


#define FIRST_IS_BIGGER 1
#define EQUAL 0
#define SECOND_IS_BIGGER -1
#define MIN_HUNT_BONUS 1
#define INITIAL_XP 1
#define MIN_VALUE 1
#define SINGEL_ITEM 1
#define CANDY_FACTOR 2
#define INVALID_PRICE -1
#define POKEMON_FULL_HP 100

#define CEIL(x)       ((((double)(x)) > ((int)(x)))? (((int)(x))+1) : (int)(x))


//=============================================================================
//								STRUCT
//=============================================================================


struct trainer_t{
	char* name;
	char* location;
	int pokemons_caught, pokeCoins;
	double xp;
	Stock bag;
	Map pokemons;
};


//=============================================================================
//					       COPY/FREE/COMPARE FUNCTIONS
//=============================================================================


/* function creates a copy of keyElement- pokemon's id (of type int*).
 * returns NULL if allocation failed.
 * otherwise returns the copy made.
 */

static MapKeyElement copyPokemonId(MapKeyElement key_element){
	assert(key_element);
	int* id_copy = malloc(sizeof(*id_copy));
	if(!id_copy) return NULL;
	*id_copy = *(int*)(key_element);
	return (MapKeyElement)id_copy;
}


/* function creates a copy of dataElement- a pokemon (of type Pokemon).
 * returns NULL if allocation failed.
 * otherwise returns the copy made.
 */

static MapDataElement copyPokemon(MapDataElement data_element){

	assert(data_element);
	Pokemon data_element_copy = pokemonCopy((Pokemon)data_element);
	return (MapDataElement)data_element_copy;
}


/* function frees a given key element- an id of pokemon(of type int*).
 */

static void freePokemonId(MapKeyElement key_element){

	assert(key_element);
	free(key_element);
}


/* function frees a given data element- a pokemon(of type Pokemon).
 */

static void freePokemon(MapDataElement data_element){

	assert(data_element);
	pokemonDestroy((Pokemon)data_element);
}


/* function compares between 2 key element- two pokemon's id (of type int*).
 * returns 1 if first key argument is bigger then the second (int copmare).
 * returns 0 if two argument are equal.
 * returns -1 if second argument is bigger.
 */

static int ComparePokemonId(MapKeyElement key_element_first,
		MapKeyElement key_element_second){

	assert(key_element_first && key_element_second);
	if(*(int*)key_element_first == *(int*)key_element_second) return EQUAL;
	return (*(int*)key_element_first - *(int*)key_element_second > 0)?
			FIRST_IS_BIGGER : SECOND_IS_BIGGER;
}


/* function allocates memory for a trainer. malloc is for trainer, its name
 * and location in size as arguments. if bool flag given as an argument is true
 * the function allocates memory for trainer's bag (type Stock) and
 * pokemons (type Map).
 * bool given is true if the caller is trainerCreate and false if trainerCopy.
 * ----return----
 * returns NULL if any allocation failed.
 * return trainer allocated if allocation was successful.
 */

static Trainer trainerAllocation(const char* name, const char* location,
		bool allocate_map_and_stock){
	assert(name && location);
	Trainer trainer = malloc(sizeof(*trainer));
	if(!trainer) return NULL;
	trainer->name = malloc(strlen(name) + 1);
	if (!trainer->name ){
		free(trainer);
		return NULL;
	}
	trainer->location = malloc(strlen(location) + 1);
	if(!trainer->location){
		free(trainer->name);
		free(trainer);
		return NULL;
	}
	if(!allocate_map_and_stock) return trainer;
	trainer->bag = stockCreate();
	trainer->pokemons = mapCreate(copyPokemonId, copyPokemon, freePokemonId,
			freePokemon, ComparePokemonId);
	if(!trainer->bag || !trainer->pokemons){
		trainerDestroy(trainer);
		return NULL;
	}
	return trainer;
}

/* function receive a trainer and a new location and copies the location.
 * if allocation was successful it frees previous location and put new
 * location instead.
 * returns false if allocation for new location failed.
 * otherwise returns true.
 */

static bool isTrainerLocationSet(Trainer trainer, const char* new_location){

	assert(trainer && new_location);
	char* location_copy = malloc(strlen(new_location) + 1);
	if(!location_copy) return false;

	strcpy(location_copy, new_location);
	free(trainer->location);
	trainer->location = location_copy;
	return true;
}


/* function makes a translation from PokemonResult to TrainerResult by the
 *  result received as an argument.
 *  returns the appropriate translation result.
 */

static TrainerResult pokemonResultToTrainerResult(PokemonResult result){
	switch(result){

	case POKEMON_OUT_OF_MEMORY : return TRAINER_OUT_OF_MEMORY;
	case POKEMON_NULL_ARGUMENT : return TRAINER_NULL_ARGUMENT;
	case POKEMON_INVALID_ARGUMENT : return TRAINER_INVALID_ARGUMENT;
	case POKEMON_HP_IS_AT_MAX : return TRAINER_HP_IS_AT_MAX;
	default : return TRAINER_SUCCESS;
	}
}


/* function makes a translation from MapResult to TrainerResult by the
 *  result received as an argument.
 *  returns the appropriate translation result.
 */

static TrainerResult mapResultToTrainerResult(MapResult result){
	switch(result){

	case MAP_OUT_OF_MEMORY : return TRAINER_OUT_OF_MEMORY;
	case MAP_NULL_ARGUMENT : return TRAINER_NULL_ARGUMENT;
	case MAP_ITEM_DOES_NOT_EXIST : return TRAINER_POKEMON_DOES_NOT_EXIST;
	default : return TRAINER_SUCCESS;
	}
}


/* function makes a translation from StockResult to TrainerResult by the
 *  result received as an argument.
 *  returns the appropriate translation result.
 */

static TrainerResult stockResultToTrainerResult(StockResult result){
	switch(result){

	case STOCK_OUT_OF_MEM : return TRAINER_OUT_OF_MEMORY;
	case STOCK_NULL_ARGUMENT : return TRAINER_NULL_ARGUMENT;
	case STOCK_INVALID_ARGUMENT : return TRAINER_INVALID_ARGUMENT;
	case STOCK_ITEM_OUT_OF_STOCK : return TRAINER_NO_AVAILABLE_ITEM_FOUND;
	case STOCK_NO_AVAILABLE_ITEM_FOUND :
		return TRAINER_NO_AVAILABLE_ITEM_FOUND;
	default : return TRAINER_SUCCESS;
	}
}


/* given item with type and value function returns the price of the item.
 * for "potion" price = value.
 * for "candy" price = value*2.
 * for any type different from types mention above, function
 * returns -1 (indicates invalid type).
 */

static int getItemPrice(const char* type, int value){

	assert(type && value >= MIN_VALUE);
	if(strcmp(type,"potion") == EQUAL) return value;
	if(strcmp(type, "candy") == EQUAL) return CANDY_FACTOR * value;
	return INVALID_PRICE;
}


//=============================================================================
//                          HEADER FUNCTIONS
//=============================================================================


Trainer trainerCreate(const char* name, const char* location, int budget,
		TrainerResult* result){
	if(!name || !location || !result){
		*result = TRAINER_NULL_ARGUMENT;
		return NULL;
	}
	if(budget < 0){
		*result = TRAINER_INVALID_ARGUMENT;
		return NULL;
	}
	Trainer trainer = trainerAllocation(name, location, true);
	if(!trainer) return NULL;
	strcpy(trainer->name, name);
	strcpy(trainer->location, location);
	trainer->pokemons_caught = 0;
	trainer->pokeCoins = budget;
	trainer->xp = INITIAL_XP;
	*result = TRAINER_SUCCESS;
	return trainer;
}


void trainerDestroy(Trainer trainer){

	if(!trainer) return;
	mapDestroy(trainer->pokemons);
	stockDestroy(trainer->bag);
	free(trainer->name);
	free(trainer->location);
	free(trainer);
}


TrainerResult trainerMove(Trainer trainer,const char* new_location){

	if(!trainer || !new_location) return TRAINER_NULL_ARGUMENT;
	if(strcmp(trainer->location, new_location) == EQUAL)
		return TRAINER_TRAINER_ALREADY_IN_LOCATION;

	return (isTrainerLocationSet(trainer,new_location)) ? TRAINER_SUCCESS :
			TRAINER_OUT_OF_MEMORY;
}


TrainerResult trainerHunt(Trainer trainer, const char* pokemon_species,
		int pokemon_cp, int hunt_bonus){
	if(!trainer) return TRAINER_NULL_ARGUMENT;
	if(hunt_bonus < MIN_HUNT_BONUS)return TRAINER_INVALID_ARGUMENT;
	int id = trainer->pokemons_caught + 1;

	PokemonResult create_result;
	Pokemon new_pokemon = pokemonCreate(pokemon_species, pokemon_cp, id,
			&create_result);
	if(!new_pokemon) return pokemonResultToTrainerResult(create_result);

	MapResult put_result = mapPut(trainer->pokemons, (MapKeyElement)&id,
			(MapDataElement)new_pokemon);
	pokemonDestroy(new_pokemon);
	if(put_result != MAP_SUCCESS) return mapResultToTrainerResult(put_result);

	trainer->pokeCoins += hunt_bonus;
	trainer->pokemons_caught++;
	return TRAINER_SUCCESS;
}


TrainerResult trainerAddItem(Trainer trainer,const char* type, int value){

	if(!trainer || !type) return TRAINER_NULL_ARGUMENT;
	if(value < MIN_VALUE) return TRAINER_INVALID_ARGUMENT;
	int itemPrice = getItemPrice(type, value);
	if(itemPrice == INVALID_PRICE) return TRAINER_INVALID_ARGUMENT;
	if(trainer->pokeCoins < itemPrice) return TRAINER_BUDGET_IS_NOT_SUFFICIENT;

	StockResult add_result = stockAddItem(trainer->bag,type,value,SINGEL_ITEM);
	if(add_result != STOCK_SUCCESS)
		return stockResultToTrainerResult(add_result);
	trainer->pokeCoins -= itemPrice;
	return TRAINER_SUCCESS;
}



char* trainerGetLocation(Trainer trainer){

	if(!trainer) return NULL;
	return trainer->location;
}



bool isTrainerContainsPokemon(Trainer trainer, int index){

	if(!trainer) return false;
	return (!mapGet(trainer->pokemons, (MapKeyElement)&index))? false : true;
}



TrainerResult trainerGetParameters(Trainer trainer, double* trainers_xp,
		int index, char** pokemon_species, int* pokemon_cp, double* pokemon_hp,
		int* pokemon_level){

	if(!trainer || !trainers_xp) return TRAINER_NULL_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if(!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;

	PokemonResult get_result = pokemonGetParameters(pokemon, pokemon_species,
			pokemon_cp, pokemon_hp, pokemon_level);
	if(get_result != POKEMON_SUCCESS)
		return pokemonResultToTrainerResult(get_result);
	*trainers_xp = trainer->xp;
	return TRAINER_SUCCESS;
}



TrainerResult trainerRemovePokemonIfDead(Trainer trainer, int index){

	if(!trainer) return TRAINER_NULL_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if(!pokemon)return TRAINER_POKEMON_DOES_NOT_EXIST;

	if(!pokemonGetHp(pokemon)){
		mapRemove(trainer->pokemons, (MapKeyElement)&index);
	}			                                    // map contains pokemon->
	return TRAINER_SUCCESS;                          //mapRemove cant fail.
}



TrainerResult trainerEvolvePokemon(Trainer trainer, int index,
		int evolve_level, const char* new_species, int new_cp){

	if(!trainer || !new_species) return TRAINER_NULL_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if (!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;

	PokemonResult evolve_result = pokemonEvolve(pokemon, new_species,
			evolve_level, new_cp);
	return (evolve_result == POKEMON_SUCCESS) ? TRAINER_SUCCESS:
			pokemonResultToTrainerResult(evolve_result);
}



TrainerResult trainerSetParameters(Trainer trainer, double new_xp, int index,
		double new_hp, int new_level){

	if(!trainer) return TRAINER_NULL_ARGUMENT;
	if(new_xp < INITIAL_XP) return TRAINER_INVALID_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if(!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;

	PokemonResult set_result = pokemonSetParameters(pokemon, new_hp,new_level);
	if(set_result != POKEMON_SUCCESS)
		return pokemonResultToTrainerResult(set_result);
	trainer->xp = new_xp;
	return TRAINER_SUCCESS;
}



TrainerResult trainerPokemonHeal(Trainer trainer, int index){

	if(!trainer) return TRAINER_NULL_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if(!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;

	int value_required = CEIL(POKEMON_FULL_HP - pokemonGetHp(pokemon));
	if(value_required == 0) return TRAINER_HP_IS_AT_MAX;
	int best_value_found;
	StockResult potion_search_result = stockFindBestPotion(trainer->bag,
			value_required, &best_value_found);
	if(potion_search_result != STOCK_SUCCESS)
		return stockResultToTrainerResult(potion_search_result);

	pokemonHeal(pokemon, best_value_found);//pokemon is not NULL and value
	                                       //found is valid.
	stockRemoveItem(trainer->bag, "potion", best_value_found);
	return TRAINER_SUCCESS;
}



TrainerResult trainerPokemonTrain(Trainer trainer, int index){

	if(!trainer) return TRAINER_NULL_ARGUMENT;
	Pokemon pokemon = mapGet(trainer->pokemons, (MapKeyElement)&index);
	if(!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;

	int most_valuable_candy;
	StockResult find_candy_result = stockFindMostValuableCandy(trainer->bag,
			&most_valuable_candy);
	if(find_candy_result != STOCK_SUCCESS)
		return stockResultToTrainerResult(find_candy_result);

	StockResult remove_result = stockRemoveItem(trainer->bag, "candy",
			most_valuable_candy);
	if(remove_result != STOCK_SUCCESS)
		return stockResultToTrainerResult(remove_result);
	return isPokemonTrained(pokemon, most_valuable_candy) ? TRAINER_SUCCESS :
			TRAINER_INVALID_ARGUMENT;
}



Trainer trainerCopy(Trainer trainer){

	Trainer copy = trainerAllocation(trainer->name, trainer->location, false);
	if(!copy) return NULL;
	copy->bag = stockCopy(trainer->bag);
	copy->pokemons = mapCopy(trainer->pokemons);
	if(!copy->bag || !copy->pokemons){
		trainerDestroy(copy);
		return NULL;
	}
	strcpy(copy->name, trainer->name);
	strcpy(copy->location, trainer->location);
	copy->pokeCoins = trainer->pokeCoins;
	copy->pokemons_caught = trainer->pokemons_caught;
	copy->xp = trainer->xp;
	return copy;
}



TrainerResult trainerShowInfo(FILE* output_file, Trainer trainer){

	if(!trainer || !output_file) return TRAINER_NULL_ARGUMENT;
	mtmPrintTrainerHeader(output_file, trainer->name,  trainer->location,
			trainer->pokeCoins, trainer->xp);

	mtmPrintItemsHeaderForTrainer(output_file);
	stockShowInfo(trainer->bag, output_file, false);
	mtmPrintPokemonsHeaderForTrainer(output_file);
	MAP_FOREACH(int* ,id, trainer->pokemons){
		Pokemon pokemon= (Pokemon)mapGet(trainer->pokemons, (MapKeyElement)id);
		pokemonShowInfo(output_file, pokemon);
	}
	return TRAINER_SUCCESS;
}



