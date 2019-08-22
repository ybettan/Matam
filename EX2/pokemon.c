#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pokemon.h"

#define NDEBUG

#define MAX_EXPERIENCE 9901
#define SOME_MEMORY_ALLOCATION_FAILED -1


//-----------------------------------------------------------------------------
//						STATIC FUNCTION
//-----------------------------------------------------------------------------


//return true if type is a valid taype and false otherwise
static bool isLegalType (PokemonType type) {
	int flag = 0;
	for (int i = TYPE_NORMAL ; i <= TYPE_ELECTRIC ; i++) {
		if (type == i) {
			flag = 1;
		}
	}
	if (flag == 1) {
		return true;
	}else{
		return false;
	}
}

//return the upper bound of num
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

//the function allocate the memory for the move and initiallize it
//return a pointer to the move or NULL if allocate failed
static  PokemonMove moveCreate (char* move_name,PokemonType type,
										int max_power_points,int strength) {
	PokemonMove move = malloc(sizeof(*move));
	if (move == NULL)	return NULL;

	move->name = malloc(sizeof(char) * (strlen(move_name) + 1 ) );
	if (move->name == NULL) {
		free(move);
		return NULL;
	}
	move->name = strcpy(move->name,move_name);
	move->type = type;
	move->max_power_points = max_power_points;
	move->power_points = max_power_points;
	move->strength = strength;

	return move;
}

//the function release the memory allocated for move
static void moveDestroy(PokemonMove move) {
	if (move != NULL) {
		free(move->name);
	}
	free(move);
}

//the function create a new move and copy into it the content of "move"
//return a pointer to the new move or NULL if malloc failed or move is NULL
static PokemonMove moveCopy(PokemonMove move) {
	if (move == NULL)	return NULL;

	PokemonMove move_copy = malloc(sizeof(*move_copy));
	if (move_copy == NULL)	return NULL;

	move_copy->name = malloc(sizeof(char) * (strlen(move->name) + 1 )  );
	if (move_copy->name == NULL) {
		free(move_copy);
		return NULL;
	}

	move_copy->name = strcpy(move_copy->name,move->name);
	move_copy->type = move->type;
	move_copy->power_points = move->power_points;
	move_copy->max_power_points = move->max_power_points;
	move_copy->strength = move->strength;

	return move_copy;
}

//the function return the average strength of all moves than 'pokemon' know
//the function asuume - assert that (pokemon != NULL)
//the renurn value is casted to integer
//the function assumes - (pokemon != NULL   &&   pokemon->number_of_moves > 0)
static int pokemonGetAvgMoveStrength (Pokemon pokemon) {
	assert(pokemon != NULL   &&   pokemon->number_of_moves > 0);

	int counter = 0;
	for (int i = 0 ; i < pokemon->number_of_moves ; i++) {
		counter = counter + ((pokemon->moves)[i])->strength;
	}
	return counter/pokemon->number_of_moves;
}

//the function return true if pokemon knows a move with same name as move_name
//end else other wise
//------Errors:------
//the function assume that - assert(move_name != NULL   &&   pokemon != NULL)
static bool alredyExistMove(Pokemon pokemon,char* move_name) {
	assert(move_name != NULL   &&   pokemon != NULL);

	bool flag = false;
	char* current_move_name = NULL;
	for (int i = 0 ; i < pokemon->number_of_moves ; i++) {
		current_move_name = ((pokemon->moves)[i])->name;
		if (  strcmp(current_move_name,move_name) == 0  ) {
			flag = 1;
		}
	}
	return flag;
}

//the function return the index in array of moves of the move with the first
//												string in lexicographical sort
//the function assumes that - assert(pokemon != NULL)
static int minLexikographyMove(PokemonMove* pokemon_moves , int size_of_moves){
	assert(pokemon_moves != NULL);

	int max_string_size = 0;
	for (int i = 0 ; i < size_of_moves ; i++) {
		if (  strlen(pokemon_moves[i]->name) > max_string_size) {
			max_string_size = strlen(pokemon_moves[i]->name);
		}
	}

	char* min_name = malloc(sizeof(*min_name) * max_string_size);
	char* current_name = malloc(sizeof(*min_name) * max_string_size);
	if (min_name == NULL || current_name == NULL) {
		return SOME_MEMORY_ALLOCATION_FAILED;
	}

	int min_index = 0;
	min_name = strcpy(min_name,pokemon_moves[0]->name);

	for (int i = 1 ; i < size_of_moves ; i++) {
		current_name = strcpy(current_name,pokemon_moves[i]->name);
		if (   strcmp(current_name,min_name) < 0   ) {
			min_index = i;
			min_name = strcpy(min_name,current_name);
		}
	}
	free(min_name);
	free(current_name);

	return min_index;
}

//the function remove the char at index
//shift left all letters that their index is greater than index
static void removeCharFromString(char* string ,int index) {
	assert(string != NULL);
	assert(index >= 0   &&   index < strlen(string));

	for (int i = index + 1 ; i < strlen(string) ; i++) {
		string[i-1] = string[i];
	}
	string[strlen(string) - 1] = '\0';
}

//the function cut the string in the middle
//if the number of chars is odd we cut at upperBound(strlen(string))
static void cutString(char* string) {
	assert(string != NULL);

	string[  upperBound( (double)strlen(string)/2 )  ] = '\0';
}
//-----------------------------------------------------------------------------
//						HEADER FUNCTION
//-----------------------------------------------------------------------------

Pokemon pokemonCreate(char* name, PokemonType type, int experience,
		 	 	 	 	 	 	 	 	 	 	 	 int max_number_of_moves) {
	int flag = 0;

	if (name == NULL || strcmp(name,"") == 0) {
		flag = 1;
	}
	if ( !isLegalType(type) ) {
		flag = 1;
	}
	if ( experience <= 0  ||  experience > MAX_EXPERIENCE ) {
		flag = 1;
	}
	if (max_number_of_moves <= 0) {
		flag = 1;
	}

	if (flag == 1)	return NULL;

	Pokemon pokemon = malloc(sizeof(*pokemon));
	if (pokemon == NULL)	return NULL;

	pokemon->name = malloc(sizeof(*pokemon->name) * (strlen(name)+1));
	if (pokemon->name == NULL) {
		free(pokemon);
		return NULL;
	}
	pokemon->moves = malloc(max_number_of_moves * sizeof(*pokemon->moves));
	if (pokemon->moves == NULL) {
		free(pokemon->name);
		free(pokemon);
		return NULL;
	}

	for (int i=0 ; i<max_number_of_moves ; i++) {
		(pokemon->moves)[i] = NULL;
	}

	pokemon->name = strcpy(pokemon->name,name);
	pokemon->type = type;
	pokemon->experience = experience;
	pokemon->max_number_of_moves = max_number_of_moves;
	pokemon->number_of_moves = 0;
	pokemonHeal(pokemon);

	return pokemon;
}
//-----------------------------------------------------------------------------
void pokemonDestroy(Pokemon pokemon) {
	if (pokemon != NULL) {
		free(pokemon->name);
		for (int i=0 ; i<pokemon->max_number_of_moves ; i++) {
			moveDestroy((pokemon->moves)[i]);
		}
		free(pokemon->moves);
	}
	free(pokemon);
}
//-----------------------------------------------------------------------------
Pokemon pokemonCopy(Pokemon pokemon) {
	if (pokemon == NULL)	return NULL;

	Pokemon pokemon_copy = pokemonCreate(pokemon->name,pokemon->type,
							pokemon->experience,pokemon->max_number_of_moves);
	if (pokemon_copy == NULL)	return NULL;

	assert(pokemon != NULL);
	pokemon_copy->experience = pokemon->experience;
	pokemon_copy->health_points = pokemon->health_points;
	pokemon_copy->number_of_moves = pokemon->number_of_moves;
	for (int i=0 ; i<pokemon_copy->number_of_moves ; i++) {
		(pokemon_copy->moves)[i] = moveCopy((pokemon->moves)[i]);
	}
	return pokemon_copy;
}
//-----------------------------------------------------------------------------
PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
					PokemonType type, int max_power_points, int strength) {

	if (pokemon == NULL   ||   move_name == NULL)	return POKEMON_NULL_ARG;
	if (strcmp(move_name,"") == 0)	return POKEMON_INVALID_MOVE_NAME;
	if (type < TYPE_NORMAL || type > TYPE_ELECTRIC) return POKEMON_INVALID_TYPE;
	if (max_power_points <= 0 )		return POKEMON_INVALID_POWER_POINTS;
	if (strength <= 0 )		return POKEMON_INVALID_STRENGTH;
	if (type != TYPE_NORMAL   &&   type != pokemon->type) {
		return POKEMON_INCOMPATIBLE_MOVE_TYPE;
	}
	if (alredyExistMove(pokemon,move_name))	return POKEMON_MOVE_ALREADY_EXISTS;

	PokemonMove move = moveCreate (move_name,type,max_power_points,strength);
	if (move == NULL)	return POKEMON_OUT_OF_MEM;

	assert(move != NULL   &&   pokemon != NULL);

	if (pokemon->number_of_moves == pokemon->max_number_of_moves) {
		int index =
			minLexikographyMove(pokemon->moves,pokemon->max_number_of_moves);
		if (index == SOME_MEMORY_ALLOCATION_FAILED)	return POKEMON_OUT_OF_MEM;
		pokemon->moves[index] = move;
	}else{
		(pokemon->moves)[pokemon->number_of_moves] = move;
		pokemon->number_of_moves = pokemon->number_of_moves + 1;
	}

	return POKEMON_SUCCESS;
}
//-----------------------------------------------------------------------------
int pokemonGetLevel(Pokemon pokemon) {
	assert (pokemon != NULL);
	double temp_level = ( (double)(pokemon->experience) ) / 100;
	int level = upperBound(temp_level);
	return level;
}
//-----------------------------------------------------------------------------
PokemonResult pokemonHeal(Pokemon pokemon) {
	if (pokemon == NULL)	return POKEMON_NULL_ARG;

	int pokemon_level = pokemonGetLevel(pokemon);
	pokemon->health_points = (100 + pokemon_level) * 10;

	return POKEMON_SUCCESS;
}
//-----------------------------------------------------------------------------
int pokemonGetRank(Pokemon pokemon) {
	assert(pokemon != NULL);

	if (pokemon->number_of_moves == 0)	return 0;
	assert(pokemon->number_of_moves > 0);

	int rank = pokemonGetLevel(pokemon) + pokemonGetAvgMoveStrength(pokemon);
	return rank;
}
//-----------------------------------------------------------------------------
PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file) {
	if (pokemon == NULL   ||   file == NULL)	return POKEMON_NULL_ARG;
	assert(pokemon != NULL);
	assert(file != NULL);

	fprintf(file,"%s",pokemon->name);

	return POKEMON_SUCCESS;
}
//-----------------------------------------------------------------------------
PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file) {
#define MAX_LETTERS 4

	if (pokemon == NULL   ||   file == NULL)	return POKEMON_NULL_ARG;
	assert(pokemon != NULL);
	assert(file != NULL);

	int name_size = strlen(pokemon->name);
	char* pokemon_name = malloc( sizeof(*pokemon_name) * (name_size + 1) );
	if (pokemon_name == NULL)	return POKEMON_OUT_OF_MEM;

	pokemon_name = strcpy(pokemon_name,pokemon->name);

	for (int i = 0 ; i < strlen(pokemon_name) ; i++) {
		if (   !(   (pokemon_name[i] <= 'z'  &&  pokemon_name[i] >= 'a' )   ||
					(pokemon_name[i] <= 'Z'  &&  pokemon_name[i] >= 'A' ) ) ) {
			removeCharFromString(pokemon_name,i);
			i = i - 1;
		}
	}

	if ( strlen(pokemon_name) >= MAX_LETTERS ) {
		cutString(pokemon_name);
	}

	fprintf(file,"%s-%s",pokemon_name,pokemon_name);
	free(pokemon_name);

	return POKEMON_SUCCESS;
}

//-----------------------------------------------------------------------------
//									EOF
//-----------------------------------------------------------------------------
