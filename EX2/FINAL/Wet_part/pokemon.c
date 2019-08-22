#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pokemon.h"

#define FIRST_TYPE TYPE_NORMAL
#define LAST_TYPE TYPE_ELECTRIC
#define MAX_EXPERIENCE 9901
#define MIN_EXPERIENCE 1
#define MIN_MOVE_STRENGTH 1
#define MIN_NUMBER_OF_MOVES 1
#define DEFAULT_ATTACK_FACTOR 1
#define STRONG_ATTACK_FACTOR 2
#define MIN_LETTERS_TO_CUT 4

typedef enum{
	ALLOCATION_SUCCESS,
	ALLOCATION_FAILED
}AllocationFlag;

typedef enum{
	MOVES,
	NAMES
}FieldToFree;


//-----------------------------------------------------------------------------
//                      STATIC FUNCTION
//-----------------------------------------------------------------------------


/** checks if name is an empty string. */

static bool isNameNotEmpty(char* name){
	if(!strlen(name)) return false;
	return true;
}


/** checks if experience is a positive number. */

static bool isExperienceValid(int experience){
	if(experience>MAX_EXPERIENCE || experience<MIN_EXPERIENCE) return false;
	return true;
}


/** checks if type is one of the values in the enum PokemonType. */

static bool isTypeValid(PokemonType type){
	if(type < FIRST_TYPE || type > LAST_TYPE) return false;
	return true;
}


/** checks if max_number_of moves is a positive number. */

static bool isMaxMovesValid(int max_number_of_moves){
	if(max_number_of_moves < MIN_NUMBER_OF_MOVES) return false;
	return true;
}


/** checks if move's strength is a positive number. */

static bool isMoveStrengthValid(int moveStrength){
	if (moveStrength < MIN_MOVE_STRENGTH) return false;
	return true;
}


/** checks if health is a positive number. */

static bool isHealthPositve(int health_points){
	if(!health_points) return false;
	return true;
}


/**returns the upper bound of num. */

static int upperBound(double num) {
    int result;
    if (num > (int)num) {
        result = (int)num + 1;
    }else{
        assert(num == (double)((int)num));
        result = (int)num;
    }
    return result;
}


/** returns pokemon hp according to its level. */

static int getHealthPoints(Pokemon pokemon){
	assert(pokemon != NULL);
	return (100 + pokemonGetLevel(pokemon))*10;
}


/** the function gets the index in which previous allocation failed,
 *  and gets an array of moves. the function frees the fields
 *  in correspondence to the third argument it received. */

static void freeAllocatedFields(int index, PokemonMove* moves,
		FieldToFree field){
	if(field == MOVES){
		for(int j = index; j > 0; j--){
			free(moves[j-1]);
		}
	} else {
		for(int j = index; j > 0; j--){
			free(moves[j-1]->name);
		}
	}
}


/** gets two arrays of moves as an argument. the function allocates memory for
 *  moves in destination array according to source array number of moves.
 *  it also allocates memory in destination array for each move name and copy
 *  it from source array.
 *  it returns one of the values of the enum "AllocationFlag" according to the
 *  allocation result. if allocation failed, it calls freeAllocatedFields in
 *  order to free previous allocations. */

static AllocationFlag moveAllocateAndCopy(PokemonMove* destination,
		PokemonMove* source, int number_of_moves, int max_number_of_moves){
	for(int i = 0; i < number_of_moves; i++){
		destination[i] = malloc(sizeof(*destination[i]));
		if(!destination[i]){
			freeAllocatedFields(i, destination, NAMES);
			freeAllocatedFields(i, destination, MOVES);
			return ALLOCATION_FAILED;
		}
		destination[i]->name = malloc(strlen(source[i]->name)+1);
		if(!destination[i]->name){
			freeAllocatedFields(i, destination, NAMES);
			freeAllocatedFields(i+1, destination, MOVES);
			return ALLOCATION_FAILED;
		}
		strcpy(destination[i]->name,source[i]->name);
		destination[i]->type = source[i]->type;
		destination[i]->power_points = source[i]->power_points;
		destination[i]->max_power_points = source[i]->max_power_points;
		destination[i]->strength = source[i]->strength;
	}
	return ALLOCATION_SUCCESS;
}

/** the function gets an empty pointer of type PokemonMove and allocates memory for
 *   the move and its name. it returns whether allocation failed or succeeded.   */

static PokemonResult createNewMove(char* new_name, PokemonType type,
		int max_power_points, int strength, PokemonMove* move){
	*move = malloc(sizeof(**move));
	if(!(*move)) return POKEMON_OUT_OF_MEM;
	(*move)->name = malloc(strlen(new_name)+1);
	if(!(*move)->name){
		free(*move);
		return POKEMON_OUT_OF_MEM;
	}
	strcpy((*move)->name, new_name);
	(*move)->type = type;
	(*move)->power_points = max_power_points;
	(*move)->max_power_points = max_power_points;
	(*move)->strength = strength;
	return POKEMON_SUCCESS;
}


/** the function search for the first string in lexicographic order and replace
 *  it with a given string. the function is called only when the array of move
 *   of pokemon is full. */

static void ReplaceMove(Pokemon pokemon, PokemonMove new_move){
	int position = 0;
	for(int i = 1; i < pokemon->max_number_of_moves; i++){
		if(strcmp(pokemon->moves[position]->name ,
				pokemon->moves[i]->name) > 0 ){
			position = i;
		}
	}
	free(pokemon->moves[position]->name);
	free(pokemon-> moves[position]);
	pokemon->moves[position] = new_move;
}


/** the function search for a specific string in the array of moves and
 *  returns its position if found. */

static int findMovePosition(Pokemon pokemon, char* new_name){
	assert(pokemon && new_name);
	for (int i = 0; i < pokemon->number_of_moves ; i++){
		assert(pokemon->moves[i]);
		if(!strcmp(new_name, pokemon->moves[i]->name)) return i;
	}
	return -1;
}


/** calculates a factor which is used in pokemonUseMove. */

static int getAttackFactor(PokemonType attack_type,
		PokemonType attacked_pokemon_type){
	if(attack_type == TYPE_WATER && attacked_pokemon_type == TYPE_FIRE){
		return STRONG_ATTACK_FACTOR;
	}
	if(attack_type == TYPE_FIRE && attacked_pokemon_type == TYPE_GRASS){
		return STRONG_ATTACK_FACTOR;
	}
	if(attack_type == TYPE_GRASS && attacked_pokemon_type == TYPE_WATER){
		return STRONG_ATTACK_FACTOR;
	}
	if(attack_type == TYPE_ELECTRIC && attacked_pokemon_type == TYPE_WATER){
		return STRONG_ATTACK_FACTOR;
	}
	return DEFAULT_ATTACK_FACTOR;
}


/** calculates attack damage by attacker level and attack factor.
 *  if (damage > opponent pokemon's health points) then
 *   damage = opponent pokemon's health points */

static int getAttackDamage(Pokemon attack_pokemon, Pokemon attacked_pokemon,
		int move_position){
	assert(attack_pokemon && attacked_pokemon);
	int factor = getAttackFactor(attack_pokemon->moves[move_position]->type,
			attacked_pokemon->type);
	int attacker_level = pokemonGetLevel(attack_pokemon);
	int attack_strength = attack_pokemon->moves[move_position]->strength;
	int attack_damage = (attacker_level*2 + attack_strength)*factor;
	if(attacked_pokemon->health_points - attack_damage < 0){
		return attacked_pokemon->health_points;
	}
	return attack_damage;
}


/** returns the attacker pokemon new experience as its maximum value is up to
 *  MAX_EXPERIENCE defined as 9901. */

static int getNewExperience(Pokemon attack_pokemon, int attack_damge){
	assert(attack_pokemon != NULL);
	if(attack_pokemon->experience + attack_damge >= MAX_EXPERIENCE){
		return MAX_EXPERIENCE;
	}
	return (attack_pokemon->experience + attack_damge);
}


/** calculates the average strength of all moves pokemon knows. */

static int getAverageMovesStrength(Pokemon pokemon){
	assert(pokemon);
	int sum = 0;
	for(int i = 0; i < pokemon->number_of_moves; i++){
		sum += pokemon->moves[i]->strength;
	}
	return (int)(sum/(pokemon->number_of_moves));
}


/** checks if a givven type is compatible to the given pokemon. */

static bool isMoveTypeLegal(Pokemon pokemon, PokemonType type){
	if(type != pokemon->type && type != TYPE_NORMAL){
		return false;
	}
	return true;
}


/** the function remove the char at index shift left all letters that their
 * index is greater than index. */

static void removeCharFromString(char* string ,int index) {
    assert(string != NULL);
    assert(index >= 0  &&  index < strlen(string));

    for (int i = index + 1 ; i < strlen(string) ; i++) {
        string[i-1] = string[i];
    }
    string[strlen(string) - 1] = '\0';
}


/** the function cut the string in the middle. if the number of chars is odd
 *  then the cut is by the upperBound of strlen(string). */

static void cutString(char* string) {
    assert(string != NULL);
    string[upperBound((double)strlen(string)/2 )] = '\0';
}


//-----------------------------------------------------------------------------
//                      HEADER FUNCTION
//-----------------------------------------------------------------------------


int pokemonGetLevel(Pokemon pokemon) {
    assert (pokemon != NULL);
    int level = upperBound(((double)(pokemon->experience)) / 100);
    return level;
}


Pokemon pokemonCreate(char* name, PokemonType type, int experience,
		              int max_number_of_moves){
	if(!name || !isNameNotEmpty(name) || !isExperienceValid(experience) ||
			!isTypeValid(type) || !isMaxMovesValid(max_number_of_moves)){
		return NULL;
	}
	Pokemon pokemon = malloc(sizeof(*pokemon));
	if(!pokemon) return NULL;
	pokemon->name = malloc(strlen(name)+1);
	if (!pokemon->name){
		free(pokemon);
		return NULL;
	}
	pokemon->moves = calloc(max_number_of_moves, sizeof(*(pokemon->moves)));
	if(!pokemon->moves){
		free(pokemon->name);
		free(pokemon);
		return NULL;
	}
	strcpy(pokemon->name, name);
	pokemon->type=type;
	pokemon->experience=experience;
	pokemon->number_of_moves=0;
	pokemonHeal(pokemon);
	pokemon->max_number_of_moves=max_number_of_moves;
	return pokemon;
}


void pokemonDestroy(Pokemon pokemon){
	if(!pokemon) return;
	free(pokemon->name);
	if(pokemon->number_of_moves > 0){
		freeAllocatedFields(pokemon->number_of_moves, pokemon->moves, NAMES);
		freeAllocatedFields(pokemon->number_of_moves, pokemon->moves, MOVES);
	}
	free(pokemon->moves);
	free(pokemon);
}


Pokemon pokemonCopy(Pokemon pokemon){
	if(!pokemon) return NULL;
	Pokemon copy = pokemonCreate(pokemon->name, pokemon->type,
			pokemon->experience, pokemon->max_number_of_moves);
	if(!copy) return NULL;
	AllocationFlag result = moveAllocateAndCopy(copy->moves, pokemon->moves,
			pokemon->number_of_moves, pokemon->max_number_of_moves);
	if(result == ALLOCATION_FAILED){
		pokemonDestroy(copy);
		return NULL;
	}
	copy->number_of_moves = pokemon->number_of_moves;
	return copy;
}


PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
	PokemonType type, int max_power_points, int strength){
	if(!pokemon || !move_name) return POKEMON_NULL_ARG;
	if(!isNameNotEmpty(move_name)) return POKEMON_INVALID_MOVE_NAME;
	if(!isTypeValid(type)) return POKEMON_INVALID_TYPE;
	if(!isMaxMovesValid(max_power_points)) return POKEMON_INVALID_POWER_POINTS;
	if(!isMoveStrengthValid(strength)) return POKEMON_INVALID_STRENGTH;
	if(!isMoveTypeLegal(pokemon, type)) return POKEMON_INCOMPATIBLE_MOVE_TYPE;
	if(findMovePosition(pokemon, move_name) >= 0){
		return POKEMON_MOVE_ALREADY_EXISTS;
	}
	PokemonMove new_move;
	PokemonResult result = createNewMove(move_name, type, max_power_points,
			strength, &new_move);
	if(result != POKEMON_SUCCESS) return result;
	if(pokemon->max_number_of_moves == pokemon->number_of_moves){
		 ReplaceMove(pokemon, new_move);
	} else{
		pokemon->moves[(pokemon->number_of_moves)++] = new_move;
	}
	return POKEMON_SUCCESS;
}

PokemonResult pokemonUnteachMove(Pokemon pokemon, char* move_name){
	if(!pokemon || !move_name) return POKEMON_NULL_ARG;
	if(!isNameNotEmpty(move_name)) return POKEMON_INVALID_MOVE_NAME;
	int same_name_position = findMovePosition(pokemon, move_name);
	if(same_name_position < 0) return POKEMON_MOVE_DOES_NOT_EXIST;
	free(pokemon->moves[same_name_position]->name);
	free(pokemon->moves[same_name_position]);
	for(int j=same_name_position ; j < (pokemon->number_of_moves)-1; j++){
		pokemon->moves[j] = pokemon->moves[j+1];
	}
		pokemon->moves[--(pokemon->number_of_moves)] = NULL;
		return POKEMON_SUCCESS;
}

int pokemonGetRank(Pokemon pokemon){
	assert(pokemon != NULL);
	if (!(pokemon->number_of_moves)) return 0;
	return pokemonGetLevel(pokemon) + getAverageMovesStrength(pokemon);
}


PokemonResult pokemonUseMove(Pokemon pokemon, Pokemon opponent_pokemon,
	char* move_name){
	if(!pokemon || !opponent_pokemon || !move_name) return POKEMON_NULL_ARG;
	if(!isNameNotEmpty(move_name)) return POKEMON_INVALID_MOVE_NAME;
	int move_position = findMovePosition(pokemon, move_name);
	if(move_position < 0) return POKEMON_MOVE_DOES_NOT_EXIST;
	if(!(pokemon->moves[move_position]->power_points)){
		return POKEMON_NO_POWER_POINTS;
	}
	if(!isHealthPositve(pokemon->health_points) ||
			!isHealthPositve(opponent_pokemon->health_points)){
		return POKEMON_NO_HEALTH_POINTS;
	}
	int attack_damage = getAttackDamage(pokemon, opponent_pokemon,
			move_position);
	opponent_pokemon->health_points -= attack_damage;
	(pokemon->moves[move_position]->power_points)--;
	pokemon->experience = getNewExperience(pokemon, attack_damage);
	return POKEMON_SUCCESS;
}


PokemonResult pokemonHeal(Pokemon pokemon){
	if(!pokemon) return POKEMON_NULL_ARG;
	pokemon->health_points = getHealthPoints(pokemon);
	for(int i = 0; i < pokemon->number_of_moves; i++){
		pokemon->moves[i]->power_points = pokemon->moves[i]->max_power_points;
	}
	return POKEMON_SUCCESS;
}


PokemonResult pokemonEvolve(Pokemon pokemon, char* new_name){
	if(!pokemon || ! new_name) return POKEMON_NULL_ARG;
	if(!isNameNotEmpty(new_name)) return POKEMON_INVALID_NAME;
	if(pokemon->experience == MAX_EXPERIENCE) return POKEMON_CANNOT_EVOLVE;
	char* name_after_evolve = malloc(strlen(new_name)+1);
	if(!name_after_evolve) return POKEMON_OUT_OF_MEM;
	free(pokemon->name);
	int new_experience = ((pokemonGetLevel(pokemon)) * 100)+1;
	if(new_experience > MAX_EXPERIENCE){
		new_experience = MAX_EXPERIENCE;
	}
	strcpy(name_after_evolve, new_name);
	pokemon->name = name_after_evolve;
	pokemon->experience = new_experience;
	return POKEMON_SUCCESS;
}


PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file){
    if (!pokemon || !file) return POKEMON_NULL_ARG;
    fprintf(file,"%s",pokemon->name);
    return POKEMON_SUCCESS;
}


PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file){
	if(!pokemon || !file) return POKEMON_NULL_ARG;
    int name_size = strlen(pokemon->name);
    char* pokemon_name = malloc(sizeof(*pokemon_name)* (name_size + 1));
    if (pokemon_name == NULL) return POKEMON_OUT_OF_MEM;
    pokemon_name = strcpy(pokemon_name,pokemon->name);
    for (int i = 0; i < strlen(pokemon_name); i++){
        if(!((pokemon_name[i] <= 'z' && pokemon_name[i] >= 'a') ||
        		(pokemon_name[i] <= 'Z' && pokemon_name[i] >= 'A'))){
            removeCharFromString(pokemon_name,i);
            i--;
        }
    }
    if(strlen(pokemon_name) >= MIN_LETTERS_TO_CUT){
        cutString(pokemon_name);
    }
    fprintf(file,"%s-%s",pokemon_name,pokemon_name);
    free(pokemon_name);
    return POKEMON_SUCCESS;
}

