
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "map.h"
#include "set.h"
#include "list.h"
#include "pokemonGo.h"
#include "trainer.h"
#include "store.h"
#include "print_utils.h"


//=============================================================================
//								MACROS
//=============================================================================


#define LINE_SIZE 256
#define HUNT_BONUS 10
#define NORMAL_FACTOR 1
#define SPECIAL_FACTOR 2
#define VERY_SPECIAL_FACTOR 3
#define SINGEL_ITEM 1

#define MAX(x, y)       ( ( (x) > (y) ) ? (x) : (y) )
#define ONE_TENTH(x)    ( ( (x)/10.0 ) )
#define CEIL(x)         ((((double)(x))>((int)(x)))? (((int)(x))+1) : (int)(x))
#define DELTA(x, y, z)  ( (x) * (y) * ( 1 + ((z)/200.0) ) )
#define LEVEL_UP(x, y)  ( (x) + CEIL( ((x)+(y)) / 4.0 ) )


//=============================================================================
//								STRUCTs
//=============================================================================


/* PokemonGo (PokemonGo_t*) is a pointer to a struct contains 4 different
 * map containers and 2 other fields:
 * 1.pokedex- is a map that contains Species(char*) as key and SpeciesData as
 *   data. pokedex contains basic information about all pokemons in game.
 * 2.evolutions- is a map that contains Species as key and EvolutionData as
 *   data.
 *   evolutions contains information about all the possible evolutions in game.
 * 3.locations- is a map that contains Location as key and LocationData as data
 *   locations contains information about the topological map in the game.
*  4.trainers- is a map that contains Trainer_name as key and Trainer as data.
 *   trainers map contains pointers to all trainers existing in game.
 *   others:
 * 1.store- is a place for trainers to buy there potions and candies. with
 *   those items trainers can train or heal their pokemons.
 * 2.output_file- an open file in which all the printing will be made.
 *
 */


struct PokemonGo_t {
	Map pokedex;
	Map evolutions;
	Map locations;
	Map trainers;
	Store store;
	FILE* output_file;
};


//* note: typedefs for char* are consistently used in the game
//*       implementation. Species, Location = char* . Species is being used
//*       as pokedex map key, and Location is used as locations map
//*       key.


/* Species and SpeciesData for pokedex map.
 */

typedef char* Species;

typedef struct speciesData_t {
	int initial_cp;
	Set types;
} *SpeciesData ;


/* EvolutionData for evolutions map.
 */

typedef struct evolutionData_t {
	char* evolution;
	int evolve_level;
} *EvolutionData ;


/* Location and LocationData for locations map.
 */

typedef char* Location;

typedef struct locationData_t {
	Set adjacencies;
	List pokemons;
} *LocationData ;


//=============================================================================
//					STRING COPY/DESTROY/COMPARE FUNCTIONS
//=============================================================================


/* creates a copy of a string.
 * since type is void* it can fit map, set and list all at once.
 * -----return-----
 * a pointer to the copy.
 * NULL if malloc failed or string == NULL.
 */

static MapKeyElement copyString(MapKeyElement string) {

	if (string == NULL)	return NULL;
	int len = strlen( (char*)string);
	char* string_copy = malloc( sizeof(*string_copy) * (len + 1) );
	if (string_copy == NULL) return NULL;

	strcpy(string_copy,(char*)string);
	return (MapKeyElement)string_copy;
}


/* frees all the memory allocated for the string.
 */

static void destroyString(MapKeyElement string) {

	free(string);
}


/* compare 2 strings.
 * -----return-------
 * 	1  ==> str1 > str2 (in lexicografic order)
 * 	0  ==> str == str2
 * -1  ==> str1 < str2
 */

static int compareString(MapKeyElement str1,MapKeyElement str2) {

	int compare= strcmp( (char*)str1,(char*)str2 );
	if(!compare) return 0;
	return compare > 0 ? 1 : -1;
}


//=============================================================================
//								POKEDEX CREATE
//=============================================================================


/* create a new species and initialize it to string given.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed or if str == NULL.
 */

static Species createSpecie(char* str) {
	                                           //not a generic function.
	if (!str ) return NULL;
	Species specie = malloc( sizeof(*specie) * (strlen(str)) + 1 );
	if ( !specie )	return NULL;

	strcpy(specie,str);
	return specie;
}


/* create a new specieData.
 * "types" is a string of multiple types.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed of if types == NULL.
 */

static SpeciesData createSpeciesData(int initial_cp, char* types) {
	                                           //not a generic function.

	if (!types) return NULL;
	SpeciesData specie_data = malloc( sizeof(*specie_data) );
	if ( !specie_data )	return NULL;
	specie_data->initial_cp = initial_cp;
	specie_data->types = setCreate(copyString,destroyString,compareString);
	if ( !specie_data->types ) {
		free(specie_data);
		return NULL;
	}

	char* type_running = strtok(types," \r\n");
	assert(type_running);
	SetResult result;
	while ( type_running ) {
		result = setAdd(specie_data->types,type_running);
		if ( result == SET_OUT_OF_MEMORY) {
			/* if item alredy exist we continue without any problem.
			 * no other errors can occured in our case */
			setDestroy(specie_data->types);
			free(specie_data);
			return NULL;
		}
		type_running = strtok(NULL," \r\n");
	}
	return specie_data;
}


/*  releases all the memory allocated for pokedex data element.
 */

static void destroySpeciesData(MapDataElement element) {

	if (element != NULL) {
		setDestroy(((SpeciesData)element)->types);
	}
	free(element);
}


/* creates a copy of a pokedex data element.
 * -----return-----
 * a pointer to the copy.
 * NULL if any allocation failed or if element == NULL.
 */

static MapDataElement copySpeciesData(MapDataElement element) {

	if ( !element )	return NULL;
	SpeciesData element_copy = malloc( sizeof(*element_copy) );
	if (element_copy == NULL) return NULL;

	element_copy->initial_cp = ((SpeciesData)element)->initial_cp;
	element_copy->types = setCopy(((SpeciesData)element)->types);
	if (element_copy->types == NULL) {
		free(element_copy);
		return NULL;
	}
	return (MapDataElement)element_copy;
}


/* transmits all the data stored in a line of pokedex_file into pokedex.
 * -----return------
 * false - if any memory allocation failed.
 * true - otherwise.
 */

static bool lineInsertToPokedex(Map pokedex, char* line) {
	assert(pokedex  &&  line);

	char* pokemon_name = strtok(line," \t\r\n");
	int pokemon_cp = atoi(strtok(NULL," \t\r\n"));
	char* types = strtok(NULL,"\r\n");

	Species specie = createSpecie(pokemon_name);
	if ( !specie )	return false;
	SpeciesData specie_data = createSpeciesData(pokemon_cp,types);
	if ( !specie_data ) {
		destroyString(specie);
		return false;
	}
	MapResult result = mapPut(pokedex,specie,specie_data);
	destroyString(specie);
	destroySpeciesData(specie_data);
	if ( result == MAP_OUT_OF_MEMORY ) return false;
	assert(result == MAP_SUCCESS);
	return true;
}


/* creates a new pokedex unit.
 * assumes (and checked with ssert) that file != NULL.
 * -----return------
 * its pointer.
 * NULL if allocation failed.
 */

static Map pokedexCreate(FILE* pokedex_f) {
	assert(pokedex_f);

	Map pokedex = mapCreate(copyString, copySpeciesData, destroyString,
			destroySpeciesData, compareString);
	if(!pokedex) return NULL;

	char line[LINE_SIZE];
	bool result;
	while( fgets(line,LINE_SIZE,pokedex_f) ) {
		result = lineInsertToPokedex(pokedex,line);
		if ( !result ) {
			mapDestroy(pokedex);
			return NULL;
		}
	}
	return pokedex;
}


//=============================================================================
//							EVOLUTIONS CREATE
//=============================================================================


/* create a new evolutionData and initialize it.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed or if evolution_name == NULL.
 */

static EvolutionData createEvolutionData(char* evolution_name,
		int required_lvl) {
													//not a generic function.
	if ( !evolution_name ) return NULL;
	EvolutionData evolution_data = malloc( sizeof(*evolution_data) );
	if ( !evolution_data ) return NULL;
	evolution_data->evolution = malloc( strlen(evolution_name) + 1);
	if ( !evolution_data->evolution ) {
		free(evolution_data);
		return NULL;
	}

	strcpy(evolution_data->evolution,evolution_name);
	evolution_data->evolve_level = required_lvl;
	return evolution_data;
}


/* releases all the memory allocated for evolution data element.
 */

static void destroyEvolutionData(MapDataElement element) {

	if (element != NULL) {
		free(((EvolutionData)element)->evolution);
	}
	free(element);
}


/* creates a copy of an evolution data element.
 * -----return-----
 * a pointer to the copy.
 * NULL if any allocation failed or if element == NULL.
 */

static MapDataElement copyEvolutionData(MapDataElement element) {

	if (element == NULL) return NULL;
	EvolutionData element_copy = malloc( sizeof(*element_copy) );
	if (element_copy == NULL) return NULL;
	element_copy->evolution = malloc( sizeof(*element_copy->evolution) *
						(strlen(((EvolutionData)element)->evolution)) + 1 );
	if (element_copy->evolution == NULL) {
		free(element_copy);
		return NULL;
	}

	strcpy(element_copy->evolution,((EvolutionData)element)->evolution);
	element_copy->evolve_level = ((EvolutionData)element)->evolve_level;
	return (MapDataElement)element_copy;
}


/* transmits all the data stored in a line of evolutions_file into evolutions.
 * -----return------
 * false - if any memory allocation failed.
 * true - otherwise.
 */

static bool lineInsertToEvolutions(Map evolutions,char* line) {
	assert(evolutions  &&  line);

	char* pokemon_name = strtok(line," \t\r\n");
	char* evolution_name = strtok(NULL," \t\r\n");
	int required_lvl =  atoi(strtok(NULL," \t\r\n"));

	Species specie = createSpecie(pokemon_name);
	if ( !specie ) return false;
	EvolutionData evolution_data = createEvolutionData(evolution_name,
			required_lvl);
	if ( !evolution_data ) {
		destroyString(specie);
		return false;
	}

	MapResult result = mapPut(evolutions,specie,evolution_data);
	destroyString(specie);
	destroyEvolutionData(evolution_data);
	if (result == MAP_OUT_OF_MEMORY) return POKEMONGO_OUT_OF_MEMORY;
	assert(result == MAP_SUCCESS);
	return POKEMONGO_SUCCESS;
}


/* creates a new evolutions unit.
 * assumes (and checked with assert) that file != NULL.
 * -----return------
 * its pointer.
 * NULL if allocation failed.
 */

static Map evolutionsCreate(FILE* evolutions_f) {
	assert(evolutions_f);

	Map evolutions = mapCreate(copyString, copyEvolutionData, destroyString,
			destroyEvolutionData, compareString);
	if( !evolutions ) return NULL;

	char line[LINE_SIZE];
	bool result;
	while( fgets(line,LINE_SIZE,evolutions_f) ) {
		result = lineInsertToEvolutions(evolutions,line);
		if ( !result ) {
			mapDestroy(evolutions);
			return NULL;
		}
	}
	return evolutions;
}


//=============================================================================
//							LOCATION CREATE
//=============================================================================


/* creates a new location and initialize it to string given.
 * ----return------
 * its pointer.
 * NULL if any mallocation failed or if str == NULL.
 */

static Location createLocation(char* str) {
	                                           //not a generic function.

	if ( !str )	return NULL;
	Location location = malloc( sizeof(*location) * (strlen(str) + 1) );
	if ( !location ) return NULL;
	strcpy(location,str);
	return location;
}


/* receives a  string of multiple adjacencies and creates a new set of
 * adjacencies.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed.
 */

static Set createLocationDataSet(char* adjacencies) {
	                                           //not a generic function.
	                                           // pointers to functions are
	                                           // always != NULL.

	Set adjacency_set = setCreate(copyString,destroyString,compareString);
	if ( !adjacency_set ) return NULL;
	Location adjacent_running = strtok(adjacencies," \r\n");
	SetResult result;
	while ( adjacent_running ) {
		result = setAdd(adjacency_set, adjacent_running);
		if ( result == SET_OUT_OF_MEMORY) {
			/* if item already exist we continue without any problem.
			 * no other errors can occured in our case */
			setDestroy(adjacency_set);
			return NULL;
		}
		adjacent_running = strtok(NULL," \r\n");
	}
	return adjacency_set;
}


/* receives a string of multiple pokemons and creates a new list of pokemons.
 * the insertion is always to the last place in insertion chronological order.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed.
 */

static List createLocationDataList(char* pokemons) {
	                                           //not a generic function.
	                                           //pointers to functions != NULL.
	List pokemons_list = listCreate(copyString,destroyString);
	if ( !pokemons_list ) return NULL;
	Species pokemon_running = strtok(pokemons," \r\n");
	ListResult result;
	while ( pokemon_running ) {
		result = listInsertLast(pokemons_list,(ListElement)pokemon_running);
		if ( result == LIST_OUT_OF_MEMORY) {
			listDestroy(pokemons_list);
			return NULL;
		}
		pokemon_running = strtok(NULL," \r\n");
	}
	return pokemons_list;
}


/* creates a new locationData. receives multiple strings for adjacencies
 * and a multiple strings to put in a list of pokemons.
 * ----return------
 * its pointer.
 * NULL if any memory allocation failed or if adjacencies or pokemons are NULL.
 */

static LocationData createLocationData(char* adjacencies,char* pokemons) {
	                                           //not a generic function.

	LocationData location_data = malloc( sizeof(*location_data) );
	if ( !location_data ) return NULL;
    location_data->adjacencies = createLocationDataSet(adjacencies);
	location_data->pokemons = createLocationDataList(pokemons);

	if ( !location_data->adjacencies || !location_data->pokemons) {
		setDestroy(location_data->adjacencies);
		listDestroy(location_data->pokemons);
		free(location_data);
		return NULL;
	}
	return location_data;
}


/* releases all the memory allocated for location data element.
 */

static void destroyLocationData(MapDataElement element) {

	if (element != NULL) {
		setDestroy(((LocationData)element)->adjacencies);
		listDestroy(((LocationData)element)->pokemons);
	}
	free(element);
}


/* creates a copy of a location data element.
 * -----return-----
 * a pointer to the copy.
 * NULL if allocation failed or element == NULL.
 */

static MapDataElement copyLocationData(MapDataElement element) {

	if (element == NULL) return NULL;
	LocationData element_copy = malloc( sizeof(*element_copy) );
	if (element_copy == NULL) return NULL;
	element_copy->adjacencies = setCopy(((LocationData)element)->adjacencies);
	if (element_copy->adjacencies == NULL) {
		free(element_copy);
		return NULL;
	}

	element_copy->pokemons = listCopy(((LocationData)element)->pokemons);
	if (element_copy->pokemons == NULL) {
		setDestroy(element_copy->adjacencies);
		free(element_copy);
		return NULL;
	}
	return (MapDataElement)element_copy;
}


/* transmits all the data stored in a line of locations_file into locations.
 * -----return------
 * false - if any memory allocation failed.
 * true - otherwise.
 */

static bool lineInsertToLocations(Map locations,char* line) {
	assert(locations  &&  line);

	char* city_and_pokemons = strtok(line,";\r\n");
	char* adjacencies = strtok(NULL,"\r\n");
	char* city_name = strtok(city_and_pokemons," \r\n");
	char* pokemons = strtok(NULL,";\r\n");
	Location location = createLocation(city_name);
	if (!location) return false;
	LocationData location_data = createLocationData(adjacencies,pokemons);
	if ( !location_data ) {
		destroyString(location);
		return false;
	}

	MapResult result = mapPut(locations,location,location_data);
	destroyString(location);
	destroyLocationData(location_data);
	if (result == MAP_OUT_OF_MEMORY) return false;
	assert(result == MAP_SUCCESS);
	return true;
}


/* creates a new locations unit.
 * assumes (checked with ssert) that file != NULL.
 * -----return------
 * its pointer.
 * NULL if allocation failed.
 */

static Map locationsCreate(FILE* locations_f) {
	assert(locations_f);

	Map locations = mapCreate(copyString, copyLocationData, destroyString,
			destroyLocationData, compareString);
	if( !locations ) return NULL;

	char line[LINE_SIZE];
	bool result;
	while( fgets(line,LINE_SIZE,locations_f) ) {
		result = lineInsertToLocations(locations,line);
		if ( !result ) {
			mapDestroy(locations);
			return NULL;
		}
	}
	return locations;
}


//=============================================================================
//								TRAINER CREATE
//=============================================================================


/* makes a copy of a trainer.
 * -----return-----
 * a pointer to the copy.
 * NULL if malloc failed or element == NULL.
 */

static MapDataElement copyTrainer(MapDataElement trainer) {

	if (trainer == NULL) return NULL;
	Trainer trainer_copy = trainerCopy( (Trainer)trainer );
	if (trainer_copy == NULL) return NULL;
	return (MapDataElement)trainer_copy;
}


/* releases all the memory allocated for trainer.
 */

static void destroyTrainer(MapDataElement trainer) {
	trainerDestroy( (Trainer)trainer );
}


/* creates a map for trainers.
 * -----return------
 * its pointer.
 * NULL if allocation failed.
 */

static Map trainersCreate() {

	Map trainers_map = mapCreate(copyString, copyTrainer, destroyString,
			destroyTrainer, compareString);
		if(!trainers_map) return NULL;
		return trainers_map;
}


//=============================================================================
//							  STATIC FUNCTION
//=============================================================================

/* makes a translation from type TrainerResult to type PokemonGoResult.
 * returns the translation.
 */

static PokemonGoResult trainerToPokemonGoResult(TrainerResult result){
	switch(result){
	case TRAINER_OUT_OF_MEMORY :
		return POKEMONGO_OUT_OF_MEMORY;
	case TRAINER_NULL_ARGUMENT : return
			POKEMONGO_NULL_ARGUMENT;
	case TRAINER_INVALID_ARGUMENT :
		return POKEMONGO_INVALID_ARGUMENT;
	case TRAINER_POKEMON_DOES_NOT_EXIST :
		return POKEMONGO_POKEMON_DOES_NOT_EXIST;
	case TRAINER_BUDGET_IS_NOT_SUFFICIENT :
		return POKEMONGO_BUDGET_IS_NOT_SUFFICIENT;
	case TRAINER_HP_IS_AT_MAX :
		return POKEMONGO_HP_IS_AT_MAX;
	case TRAINER_NO_AVAILABLE_ITEM_FOUND :
		return POKEMONGO_NO_AVAILABLE_ITEM_FOUND;
	case TRAINER_TRAINER_ALREADY_IN_LOCATION :
		return POKEMONGO_TRAINER_ALREADY_IN_LOCATION;
	default : return POKEMONGO_SUCCESS;
	}
}


/* makes a translation from type StoreResult to type PokemonGoResult.
 * returns the translation.
 */

static PokemonGoResult storeToPokemonGoResult(StoreResult result){
	switch(result){
	case STORE_OUT_OF_MEM : return POKEMONGO_OUT_OF_MEMORY;
	case STORE_NULL_ARGUMENT : return POKEMONGO_NULL_ARGUMENT;
	case STORE_INVALID_ARGUMENT : return POKEMONGO_INVALID_ARGUMENT;
	case STORE_ITEM_OUT_OF_STOCK : return POKEMONGO_ITEM_OUT_OF_STOCK;
	default : return POKEMONGO_SUCCESS;
	}
}


/* returns the bonus given to a trainer for a specific catch based on the
 * types in speciesData given as an argument.
 * returns int: 10 for normal species
 *              20 for special species
 *              30 for a very special catch.
 */

static int getHuntBonus(SpeciesData speciesData){

	assert(speciesData);
	int hunt_bounus = HUNT_BONUS;
	int factor = NORMAL_FACTOR;

	SET_FOREACH(char*, type, speciesData->types){
		if(!strcmp(type, "FIRE") || !strcmp(type, "FLYING")
				|| !strcmp(type, "POISON"))
			return hunt_bounus * VERY_SPECIAL_FACTOR;

		if(!strcmp(type, "ROCK") || !strcmp(type, "ELECTRIC")
				||!strcmp(type, "WATER") ||!strcmp(type, "FAIRY")
				||!strcmp(type, "ICE")){
			factor = SPECIAL_FACTOR;
		}
	}
	return hunt_bounus * factor;
}


/* checks if a given trainer destination is reachable from trainer's current
 * location.
 * returns true if destination is reachable.
 * returns false otherwise.
 */

static bool isLocationReachable(PokemonGo pg, Trainer trainer,
		char* destination){
	Set adjacent_set = ((LocationData)mapGet(pg->locations,
					(MapKeyElement)trainerGetLocation(trainer)))->adjacencies;
		return setIsIn(adjacent_set, (SetElement)destination);
}


/* given a trainer and a location, function checks if exists a pokemon
 * in location given. if does, trainer owns the pokemon and gets pokeCoins
 * match to the types of pokemon caught.
 * writes to a given pointer the hunted pokemon species. species gets a unique
 * id equals to the number of total pokemons caught by his master.
 * ----return-----
 * POKEMONGO_NULL_ARGUMENT - if any NULL sent to lower levels.
 * POKEMON_GO_INVALID_ARGUMENT - if any lower level detected bad argument.
 * POKEMONGO_OUT_OF_MEM - if allocation for species caught failed.
 * POKEMON_SUCCESS - if hunt was successful.
 */

static PokemonGoResult makeHuntInLocation(PokemonGo pg, Trainer trainer,
		LocationData location, Species* species_caught){

	assert(pg && trainer && location && species_caught);
	*species_caught = listGetFirst(location->pokemons);//iterator set to first.
	if(!*species_caught) return POKEMONGO_SUCCESS;
	SpeciesData species_data = mapGet(pg->pokedex,
			(MapKeyElement)*species_caught);
	int hunt_bonus = getHuntBonus(species_data);//hunt bonus can't fail and
	                                            //returns only positive int.
	TrainerResult trainer_result = trainerHunt(trainer,*species_caught,
			species_data->initial_cp, hunt_bonus);
	if(trainer_result != TRAINER_SUCCESS) {
		return trainerToPokemonGoResult(trainer_result);
	}
	return POKEMONGO_SUCCESS;
}


/* makes the mathematics calculations of a battle by the formulas #defined.
 * gets bunch of pointers to write the results.
 * since this function arguments declared in caller, no check for NULL.
 */

static void makeBattleCalculations(double* xp1_old, double* xp1_new,
		double* xp2_old, double* xp2_new, double* hp1_old, double* hp1_new,
		double* hp2_old, double* hp2_new, int cp1, int cp2,
		int* level1_old, int* level1_new, int* level2_old, int* level2_new){

	*level1_new= LEVEL_UP(*level1_old, *level2_old);
	*level2_new= LEVEL_UP(*level2_old, *level1_old);

	*hp1_new = MAX(0, (*hp1_old - DELTA(*xp2_old, cp2,*hp2_old)));
	*hp2_new = MAX(0, (*hp2_old - DELTA(*xp1_old, cp1,*hp1_old)));

	*xp1_new = *xp1_old + ONE_TENTH(DELTA(*xp1_old, cp1, *hp1_old));
	*xp2_new = *xp2_old + ONE_TENTH(DELTA(*xp2_old, cp2, *hp2_old));
}


/* collects all attributes needed to make a battle between two trainers.
 * calls a static to calculate stuff, and updates trainer's and involved
 * pokemon's attributes. after fight finished successfully, the function
 * prints the results of the fight using print_utils.h. two species
 * took part in fight are written into pointers given as arguments.
 * ----return-----
 * false- only if any pointer is NULL (actually this never happens because all
 * argument are announced by caller function).
 * true - if battle happened and all results are updated.
 */

static bool isBattleMade(PokemonGo pg,
		Trainer trainer1, Trainer trainer2, char* name1, char* name2,
		int index1, int index2, char** species1, char** species2,
		int* level1_new, int* level2_new){
	assert(trainer1 && trainer2 && name1 && name2);

	double xp1_old, xp1_new, xp2_old, xp2_new;
	double hp1_old, hp1_new, hp2_old, hp2_new;
	int cp1, cp2, level1_old, level2_old;
	TrainerResult result1, result2;
	result1 = trainerGetParameters(trainer1, &xp1_old, index1,
			species1, &cp1, &hp1_old, &level1_old);
	result2 = trainerGetParameters(trainer2, &xp2_old, index2,
			species2, &cp2, &hp2_old, &level2_old);
	if(result1 != TRAINER_SUCCESS || result2 != TRAINER_SUCCESS) return false;

	makeBattleCalculations(&xp1_old, &xp1_new, &xp2_old, &xp2_new,
			&hp1_old, &hp1_new,	&hp2_old, &hp2_new,
			cp1, cp2, &level1_old, level1_new, &level2_old, level2_new);

	trainerSetParameters(trainer1, xp1_new, index1, hp1_new, *level1_new);
	trainerSetParameters(trainer2, xp2_new, index2, hp2_new, *level2_new);

	mtmPrintBattle(pg->output_file, name1, name2,*species1, *species2,cp1, cp2,
		hp1_old,hp2_old, hp1_new, hp2_new, level1_old, level2_old, *level1_new,
		*level2_new, xp1_old, xp2_old, xp1_new, xp2_new, !hp1_new, !hp2_new);

	return true;
}


/* function gets a trainer and an index and make pokemon related to index
 * evolve if necessary. pokemon is evolved to the last evolution with
 * evolve_level <= species level. returns success also if no pokemon
 * in trainer's pokemons related to the given index. success also if no
 * evolution made.
 * ----return----
 * returns POKEMONGO_SUCCESS unless trainerEvolvePokemon failed. in that
 * case returns the translation of the TrainerResult error received.
 */

static PokemonGoResult makeEvolution(PokemonGo pg, Trainer trainer, int index,
		Species species, int level){

	if (!isTrainerContainsPokemon(trainer, index)) return POKEMONGO_SUCCESS;
	Species species_iterator = species;
	EvolutionData evolution_data=mapGet(pg->evolutions,(MapKeyElement)species);
	if (!evolution_data) return POKEMONGO_SUCCESS;
	int evolve_level, evolution_cp;

	while(evolution_data->evolve_level <= level){
		evolve_level = evolution_data->evolve_level;
		species_iterator = evolution_data->evolution;
		evolution_data= mapGet(pg->evolutions,(MapKeyElement)species_iterator);
		evolution_cp = ((SpeciesData)mapGet(pg->pokedex,
				(MapKeyElement)species_iterator))->initial_cp;
		if(!evolution_data) break;
	}
	return (strcmp(species, species_iterator)) ?
			trainerToPokemonGoResult(trainerEvolvePokemon(trainer, index,
			evolve_level, species_iterator, evolution_cp)) : POKEMONGO_SUCCESS ;
}


//=============================================================================
//					  		HEADER FUNCTION
//=============================================================================


PokemonGo pokemonGoCreate(FILE* pokedex_f, FILE* evolutions_f,
		FILE* locations_f, FILE* output_f){

	if (!pokedex_f || !evolutions_f || !locations_f || !output_f) return NULL;
	PokemonGo pg = malloc(sizeof(*pg));
	if(!pg) return NULL;

	pg->pokedex = pokedexCreate(pokedex_f);
	pg->evolutions = evolutionsCreate(evolutions_f);
	pg->locations = locationsCreate(locations_f);
	pg->trainers = trainersCreate();
	pg->store = storeCreate();
	pg->output_file = output_f;

	if (!pg->pokedex 	||
		!pg->evolutions ||
		!pg->locations  ||
		!pg->trainers   ||
		!pg->store			) {

		pokemonGoDestroy(pg);
		return NULL;
	}
	return pg;
}



void pokemonGoDestroy(PokemonGo pg) {

	if (pg) {
		mapDestroy(pg->pokedex);
		mapDestroy(pg->evolutions);
		mapDestroy(pg->locations);
		mapDestroy(pg->trainers);
		storeDestroy(pg->store);
	}
	free(pg);
}



PokemonGoResult pokemonGoAddTrainer(PokemonGo pg,char* name, int budget,
		char* start_point){

	if(!pg || !name || !start_point) return POKEMONGO_NULL_ARGUMENT;
	if(mapContains(pg->trainers, (MapKeyElement)name))
		return POKEMONGO_TRAINER_NAME_ALREADY_EXISTS;

	LocationData location = mapGet(pg->locations, (MapKeyElement)start_point);
	if(!location) return POKEMONGO_LOCATION_DOES_NOT_EXIST;
	TrainerResult trainer_result;
	Trainer trainer =trainerCreate(name, start_point, budget, &trainer_result);
	if(!trainer) return trainerToPokemonGoResult(trainer_result);
	Species species_caught = NULL;
	if(makeHuntInLocation(pg, trainer, location, &species_caught) !=
			POKEMONGO_SUCCESS){
		trainerDestroy(trainer);
		return POKEMONGO_OUT_OF_MEMORY;  // since cp is always > 0 only failure
	}                                    // possible is out_of_mem.
	MapResult map_result = mapPut(pg->trainers, (MapKeyElement)name,
			(MapDataElement)trainer);
	trainerDestroy(trainer);
	if(map_result != MAP_SUCCESS) return POKEMONGO_OUT_OF_MEMORY;
	mtmPrintCatchResult(pg->output_file, name, species_caught, start_point);
	listRemoveCurrent(location->pokemons);// iterator set to first in static.
	return POKEMONGO_SUCCESS;
}



PokemonGoResult pokemonGoTrainerMoveAndHunt(PokemonGo pg, char* name
		,char* destination){

	if(!pg || !name || !destination) return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(pg->trainers, (MapKeyElement)name);
	if(!trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	LocationData destination_data = mapGet(pg->locations,
			(MapKeyElement)destination);
	if(!destination_data) return POKEMONGO_LOCATION_DOES_NOT_EXIST;
	if(!strcmp(trainerGetLocation(trainer), destination))
		return POKEMONGO_TRAINER_ALREADY_IN_LOCATION;
	if(!isLocationReachable(pg, trainer, destination))
		return POKEMONGO_LOCATION_IS_NOT_REACHABLE;

	TrainerResult move_result = trainerMove(trainer, destination);
	if(move_result != TRAINER_SUCCESS) return POKEMONGO_OUT_OF_MEMORY;
	// since TRAINER_IN_LOCATION checked only error is OUT_OF_MEM.
	Species species_caught = NULL;
	if(makeHuntInLocation(pg, trainer,destination_data, &species_caught) !=
			POKEMONGO_SUCCESS) return POKEMONGO_OUT_OF_MEMORY;
	//since hunt bonus and cp positive hunt failure might be only OUT_OF_MEM.
	mtmPrintCatchResult(pg->output_file, name, species_caught, destination);
	listRemoveCurrent(destination_data->pokemons);// iterator set to first.
	return POKEMONGO_SUCCESS;
}



PokemonGoResult pokemonGoStoreAddItem(PokemonGo pg, char* type, int value,
		int quantity){

	return (!pg || ! type) ? POKEMONGO_NULL_ARGUMENT:
			storeToPokemonGoResult(storeAddItem(pg->store,type,value,quantity));
}



PokemonGoResult pokemonGoTrainerPurchase(PokemonGo pg, char* name ,char* type,
		int value){

	if(!pg || !name || !type) return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(pg->trainers, (MapKeyElement)name);
	if(!trainer){
		return value <= 0 ?
				POKEMONGO_INVALID_ARGUMENT : POKEMONGO_TRAINER_DOES_NOT_EXIST;
	}//error priority.
	StoreResult sell_result = storeRemoveItem(pg->store, type, value);
	if(sell_result !=STORE_SUCCESS) return storeToPokemonGoResult(sell_result);
	if(trainerAddItem(trainer, type,value) != TRAINER_BUDGET_IS_NOT_SUFFICIENT)
		return POKEMONGO_SUCCESS;//after sell_result guaranteed valid arg.
	                             //need to add store the item was just sold.
	return storeAddItem(pg->store, type,value, SINGEL_ITEM) == STORE_SUCCESS ?
			POKEMONGO_BUDGET_IS_NOT_SUFFICIENT : POKEMONGO_OUT_OF_MEMORY;
}	                                       //only error possible is OUT_OF_MEM.



PokemonGoResult pokemonGoBattle(PokemonGo pg, char* name1, char* name2,
		int index1 ,int index2){

	if(!pg || !name1 || !name2) return POKEMONGO_NULL_ARGUMENT;
	if(!strcmp(name1, name2)) return POKEMONGO_INVALID_ARGUMENT;
	Trainer trainer1 = mapGet(pg->trainers,(MapKeyElement) name1);
	Trainer trainer2 = mapGet(pg->trainers, (MapKeyElement)name2);
	if(!trainer1 || !trainer2) return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	Species species1 = NULL;
	Species species2 =NULL;
	int new_level1, new_level2;

	if(!isBattleMade(pg, trainer1, trainer2, name1, name2, index1, index2,
			&species1, &species2, &new_level1, &new_level2))
		return POKEMONGO_POKEMON_DOES_NOT_EXIST;

	trainerRemovePokemonIfDead(trainer1, index1);
	trainerRemovePokemonIfDead(trainer2, index2);

	PokemonGoResult evolution1, evolution2;
	evolution1 = makeEvolution(pg, trainer1, index1, species1, new_level1);
	evolution2 = makeEvolution(pg, trainer2, index2, species2, new_level2);
	return (evolution1 !=POKEMONGO_SUCCESS || evolution2 !=POKEMONGO_SUCCESS) ?
			POKEMONGO_OUT_OF_MEMORY : POKEMONGO_SUCCESS;
}



PokemonGoResult pokemonGoPokemonHeal(PokemonGo pg, char* name, int index){

	if(!pg || !name) return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(pg->trainers, (MapKeyElement)name);
	return (trainer) ? trainerToPokemonGoResult(trainerPokemonHeal(trainer,
			index)) : POKEMONGO_TRAINER_DOES_NOT_EXIST;
}



PokemonGoResult pokemonGoPokemonTrain(PokemonGo pg, char* name, int index){

	if(!pg || ! name) return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(pg->trainers, (MapKeyElement)name);
	return (trainer) ? trainerToPokemonGoResult(trainerPokemonTrain(trainer,
			index)): POKEMONGO_TRAINER_DOES_NOT_EXIST;
}



PokemonGoResult pokemonGoShowTrainerInfo(PokemonGo pg, char* name){

	if(!pg || ! name ) return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(pg->trainers, (MapKeyElement)name);
	return (trainer) ? trainerToPokemonGoResult(trainerShowInfo(pg->output_file,
			trainer)) : POKEMONGO_TRAINER_DOES_NOT_EXIST;
}



PokemonGoResult pokemonGoShowMapInfo(PokemonGo pg){

	if(!pg) return POKEMONGO_NULL_ARGUMENT;
	mtmPrintLocationsHeader(pg->output_file);

	MAP_FOREACH(Location, location, pg->locations){
		LocationData location_data = mapGet(pg->locations,
				(MapKeyElement)location);
		Species first_pokemon = listGetFirst(location_data->pokemons);
		mtmPrintLocation(pg->output_file, location, first_pokemon);
	}
	return POKEMONGO_SUCCESS;
}



PokemonGoResult pokemonGoShowStoreInfo(PokemonGo pg){

	if(!pg) return POKEMONGO_NULL_ARGUMENT;
	return storeToPokemonGoResult(storeShowStock(pg->store, pg->output_file));
}





