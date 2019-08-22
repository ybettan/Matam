#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
//#define NDEBUG
#include <assert.h>
#include "pokemon_trainer.h"

#define NON_POSSIBLE_VALUE -1


//-----------------------------------------------------------------------------
//							STATIC FUNCTION
//-----------------------------------------------------------------------------
//return true if pokemonTrainerCreate inputs are valid and false otherwise
bool trainerCreateCheckInputs(char* name, Pokemon initial_pokemon,
									int max_num_local, int max_num_remote) {
	int flag = 0;
	if ((name == NULL) || ( strcmp(name,"") == 0 )) {
		flag = 1;
	}
	if (initial_pokemon == NULL) {
		flag = 1;
	}
	if ((max_num_remote <= 0) || (max_num_local <= 0)) {
		flag = 1;
	}
	if (flag == 1)	return false;

	return true;
}
//-----------------------------------------------------------------------------
//allocate the memory for pokemonTrainerCreate
//return a pointer to the struct or NULL if malloc failed
PokemonTrainer trainerCreateAllocateMemory(char*name,int max_num_local,
													int max_num_remote) {
	PokemonTrainer trainer = malloc(sizeof(*trainer));
	if (trainer == NULL)	return NULL;

	trainer->name = malloc(sizeof(*trainer->name)*(strlen(name)+1));
	if (trainer->name == NULL) {
		free(trainer);
		return NULL;
	}

	trainer->trainer_pokemons_array = malloc(max_num_local *
									sizeof(*(trainer->trainer_pokemons_array)));
	if (trainer->trainer_pokemons_array == NULL) {
		free(trainer->name);
		free(trainer);
		return NULL;
	}

	trainer->proffessor_pokemons_array =
		malloc(max_num_remote * sizeof(*(trainer->proffessor_pokemons_array)));
	if (trainer->proffessor_pokemons_array == NULL) {
		free(trainer->trainer_pokemons_array);
		free(trainer->name);
		free(trainer);
		return NULL;
	}
	return trainer;
}
//-----------------------------------------------------------------------------
//initiallize trainer with all the data
PokemonTrainer trainerCreateInitiallize(PokemonTrainer trainer,char* name,
			Pokemon initial_pokemon,int max_num_local, int max_num_remote) {

	assert(
			trainer != NULL     						     &&
			trainer->name != NULL  					    	 &&
			trainer->trainer_pokemons_array != NULL    	     &&
			trainer->proffessor_pokemons_array != NULL
		  );

	for (int i=0 ; i< max_num_local ; i++) {
		(trainer->trainer_pokemons_array)[i] = NULL;
	}
	for (int i=0 ; i< max_num_remote ; i++) {
		(trainer->proffessor_pokemons_array)[i] = NULL;
	}

	trainer->name = strcpy(trainer->name,name);
	trainer->max_pokemons_local = max_num_local;
	trainer->max_pokemons_remote = max_num_remote;
	trainer->local_counter = 1;
	trainer->remote_counter = 0;
	(trainer->trainer_pokemons_array)[0] = pokemonCopy(initial_pokemon);

	return trainer;
}
//-----------------------------------------------------------------------------
//return the pointer to the most ranked pokemon in pokemons_arr
//it returns the pointer itself and not a copy
//the function update pokemon_index to hold the index of most ranked pokemon
//																	in array
//if there are more than 1 with maximal rank it will return the one with
//																smaller index
//----Errors:----
//return NULL if the array is empty
static Pokemon getMostRankedPokemonFromArray(const Pokemon* array , int size
													, int* pokemon_index) {
	if (size == 0)	return NULL;
	assert(array != NULL   &&   size > 0);
	assert(pokemon_index != NULL);

	int max_rank = pokemonGetRank(array[0]);
	int max_ranked_pokemon_index = 0;

	for (int i = 1 ; i < size ; i++) {
		if ( pokemonGetRank(array[i]) > max_rank) {
			max_rank = pokemonGetRank(array[i]);
			max_ranked_pokemon_index = i;
		}
	}

	*pokemon_index = max_ranked_pokemon_index;
	return array[max_ranked_pokemon_index];
}
//-----------------------------------------------------------------------------
//create an array and copy to it all trainer's pokemon
//return a pointer to the array
//----Errors:-----
//return NULL if memory allocation failed
static Pokemon* buildPokemonArrayFromTrainer(PokemonTrainer trainer) {
	assert(trainer != NULL);

	int total_array_size = trainer->local_counter + trainer->remote_counter;
	Pokemon* total_array = malloc( sizeof(*total_array) * total_array_size);
	if (total_array == NULL)	return NULL;

	for (int i = 0 ; i < trainer->local_counter ; i++) {
		total_array[i] = pokemonCopy(trainer->trainer_pokemons_array[i]);
	}
	for (int i = 0 ; i < trainer->remote_counter ; i++) {
		total_array[trainer->local_counter + i] =
						pokemonCopy(trainer->proffessor_pokemons_array[i]);
	}
	return total_array;
}

static void removePokemonFromArray(Pokemon* array,int* size,int index) {
	assert(array != NULL);
	assert(index >= 0   ||   index < *size);
	assert(*size > 0);

	for (int i = index + 1 ; i < *size ; i++) {
		array[i-1] = array[i];
	}
	array[*size - 1] = NULL;
	*size = *size - 1;
}
//-----------------------------------------------------------------------------
//the function sort the array as described in 	sortPokemonsArray
//----Errors:----
//return POKEMON_TRAINER_OUT_OF_MEM if malloc failed
//return POKEMON_TRAINER_SUCCESS otherwise
static PokemonTrainerResult sortPokemonsArray(Pokemon* pokemons_array,
																	int size) {
	assert(pokemons_array != NULL);
	assert(size > 0);
	Pokemon* pokemons_array_copy = malloc( sizeof(*pokemons_array_copy) * size);
	if (pokemons_array_copy == NULL)	return POKEMON_TRAINER_OUT_OF_MEM;
	for (int i = 0 ; i < size ; i++) {
		pokemons_array_copy[i] = pokemonCopy(pokemons_array[i]);
	}
	for (int i = 0 ; i < size ; i++) {
		pokemonDestroy(pokemons_array[i]);
		pokemons_array[i] = NULL;
	}

	int pokemons_array_copy_size = size;
	int* pokemons_array_copy_size_ptr = &pokemons_array_copy_size;

	Pokemon max_ranked = NULL;
	int max_ranked_index = NON_POSSIBLE_VALUE;
	int* max_ranked_index_ptr = &max_ranked_index;

	for (int i = 0 ; i < size ; i++) {
		max_ranked = getMostRankedPokemonFromArray(pokemons_array_copy,
								pokemons_array_copy_size,max_ranked_index_ptr);
		pokemons_array[i] = max_ranked;
		removePokemonFromArray(pokemons_array_copy,
								pokemons_array_copy_size_ptr,max_ranked_index);
	}
	free(pokemons_array_copy);
	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
//the function destroy all pokemons in trainer (including proffessor)
//the function set all pokemons pointers to NULL
static void resetTrainer(PokemonTrainer trainer) {
	assert(trainer != NULL);

	for (int i = 0 ; i < trainer->local_counter ; i++) {
		pokemonDestroy(trainer->trainer_pokemons_array[i]);
		trainer->trainer_pokemons_array[i] = NULL;
	}
	for (int i = 0 ; i < trainer->remote_counter ; i++) {
		pokemonDestroy(trainer->proffessor_pokemons_array[i]);
		trainer->proffessor_pokemons_array[i] = NULL;
	}
	trainer->local_counter = 0;
	trainer->remote_counter = 0;
}
//-----------------------------------------------------------------------------
//insert all pokemons located in array to trainer in order
static void trainerReinsertPokemons(PokemonTrainer trainer,
							Pokemon* pokemons_array,int pokemons_array_size) {
	assert(trainer != NULL);
	assert(pokemons_array != NULL);
	assert(pokemons_array_size >= 0);

	for (int i= 0 ; i < pokemons_array_size ; i++) {
		if (trainer->local_counter < trainer->max_pokemons_local) {
			trainer->trainer_pokemons_array[i] = pokemonCopy(pokemons_array[i]);
			trainer->local_counter = trainer->local_counter + 1;
		}else{
			trainer->proffessor_pokemons_array[i - trainer->local_counter] =
											pokemonCopy(pokemons_array[i]);
		}
	}
}
//-----------------------------------------------------------------------------
//destroy all the memory allocated for pokemons array including memory for each
//																		pokemon
static void destroyPokemonArray(Pokemon* array,int size) {
	assert(size >= 0);

	if (array != NULL) {
		for (int i = 0 ; i < size ; i++) {
			pokemonDestroy(array[i]);
		}
	}
	free(array);
}
//-----------------------------------------------------------------------------
//the function open files ,write the name and voice of pokemon to file and
//														then close the files
//-----Errors:-----
//return POKEMON_TRAINER_OUT_OF_MEM for any file action failure
//retrun POKEMON_TRAINER_SUCCESS othewise
static PokemonTrainerResult printEnumerationWriteInputToFile(Pokemon pokemon) {
	assert(pokemon != NULL);

	FILE* pokemon_name_file = fopen("pokemon name.txt","w");
	if (pokemon_name_file == NULL)	return POKEMON_TRAINER_OUT_OF_MEM;

	FILE* pokemon_voice_file = fopen("pokemon voice.txt","w");
	if (pokemon_voice_file == NULL) {
		fclose(pokemon_name_file);
		return POKEMON_TRAINER_OUT_OF_MEM;
	}
	assert(pokemon_name_file != NULL);
	assert(pokemon_voice_file != NULL);

	PokemonResult name_result , voice_result;
	name_result = pokemonPrintName(pokemon,pokemon_name_file);
	voice_result = pokemonPrintVoice(pokemon,pokemon_voice_file);
	fclose(pokemon_name_file);
	fclose(pokemon_voice_file);

	if (name_result == POKEMON_SUCCESS  &&  voice_result == POKEMON_SUCCESS) {
		return POKEMON_TRAINER_SUCCESS;
	}else{
		return POKEMON_TRAINER_OUT_OF_MEM;
	}
}
//-----------------------------------------------------------------------------
//the function open the input files and read pokemon name from file to the
//pointer pokemon_name and pokemon voice to pokemon_voice
//then the function close the files
//-----Errors:-----
//return POKEMON_TRAINER_OUT_OF_MEM for any file action or malloc failure
//retrun POKEMON_TRAINER_SUCCESS othewise
static PokemonTrainerResult printEnumerationReadInputFromFile(Pokemon pokemon
									,char* pokemon_name,char* pokemon_voice) {
	assert(pokemon_name != NULL);
	assert(pokemon_voice != NULL);

	FILE* pokemon_name_file = fopen("pokemon name.txt","r");
	if (pokemon_name_file == NULL)	return POKEMON_TRAINER_OUT_OF_MEM;

	FILE* pokemon_voice_file = fopen("pokemon voice.txt","r");
	if (pokemon_voice_file == NULL) {
		fclose(pokemon_name_file);
		return POKEMON_TRAINER_OUT_OF_MEM;
	}
	assert(pokemon_name_file != NULL);
	assert(pokemon_voice_file != NULL);

	int fscanf_result1 , fscanf_result2;
	fscanf_result1 = fscanf(pokemon_name_file,"%s",pokemon_name);
	fscanf_result2 = fscanf(pokemon_voice_file,"%s",pokemon_voice);

	fclose(pokemon_name_file);
	fclose(pokemon_voice_file);

	if ( fscanf_result1   &&   fscanf_result2 ) {
		return POKEMON_TRAINER_SUCCESS;
	}else{
		return POKEMON_TRAINER_OUT_OF_MEM;
	}
}
//-----------------------------------------------------------------------------
//the function chains enumaration for one pokemon to the end of file
static void printEnumerationWriteOutputToFile(PokemonTrainer trainer,
						char* pokemon_name,char* pokemon_voice,FILE* file) {
	assert(pokemon_name != NULL);
	assert(pokemon_voice != NULL);
	assert(file != NULL);

	fprintf(file,"%s: %s\n",trainer->name,pokemon_name);
	fprintf(file,"%s: %s\n",pokemon_name,pokemon_voice);

}
//-----------------------------------------------------------------------------
//							HEADER FUNCTION
//-----------------------------------------------------------------------------
PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
                                 int max_num_local, int max_num_remote) {

	if ( ! trainerCreateCheckInputs(name,initial_pokemon,
			max_num_local,max_num_remote)  )	return NULL;

	PokemonTrainer trainer = trainerCreateAllocateMemory(name,max_num_local,
															max_num_remote);

	trainer = trainerCreateInitiallize(trainer,name,initial_pokemon,
												max_num_local,max_num_remote);
	return trainer;

}
//-----------------------------------------------------------------------------
void pokemonTrainerDestroy(PokemonTrainer trainer) {
	if (trainer != NULL) {
		for (int i=0 ; i<trainer->max_pokemons_local ; i++) {
			pokemonDestroy(  (trainer->trainer_pokemons_array)[i]  );
		}
		for (int i=0 ; i<trainer->max_pokemons_remote ; i++) {
			pokemonDestroy(  (trainer->proffessor_pokemons_array)[i]  );
		}
		free(trainer->name);
		free(trainer->trainer_pokemons_array);
		free(trainer->proffessor_pokemons_array);
	}
	free(trainer);
}
//-----------------------------------------------------------------------------
PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer) {
	if (trainer == NULL)	return NULL;
	assert(trainer != NULL);

	PokemonTrainer trainer_copy =
					pokemonTrainerCreate(trainer->name,
										(trainer->trainer_pokemons_array)[0],
										trainer->max_pokemons_local,
										trainer->max_pokemons_remote);

	if (trainer_copy == NULL)	return NULL;
	assert(trainer_copy != NULL);

	for (int i = 1 ; i < trainer->local_counter ; i++) {
		//first pokemon was alredy copied
		(trainer_copy->trainer_pokemons_array)[i] =
						pokemonCopy((trainer->trainer_pokemons_array)[i]);
		if (trainer_copy->trainer_pokemons_array[i] == NULL)	return NULL;
	}
	for (int i = 0 ; i < trainer->remote_counter ; i++) {
		(trainer_copy->proffessor_pokemons_array)[i] =
						pokemonCopy((trainer->proffessor_pokemons_array)[i]);
		if (trainer_copy->proffessor_pokemons_array[i] == NULL)	return NULL;
	}
	assert(trainer_copy->trainer_pokemons_array != NULL);
	assert(trainer_copy->proffessor_pokemons_array != NULL);

	trainer_copy->local_counter = trainer->local_counter;
	trainer_copy->remote_counter = trainer->remote_counter;
	return trainer_copy;
}
//-----------------------------------------------------------------------------
PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
															Pokemon pokemon) {

	if (trainer == NULL   ||   pokemon == NULL)	{
		return POKEMON_TRAINER_NULL_ARG;
	}

	Pokemon pokemon_copy = pokemonCopy(pokemon);
	if (pokemon_copy == NULL) {
		return POKEMON_TRAINER_OUT_OF_MEM;
	}

	if (trainer->local_counter == trainer->max_pokemons_local) {
		pokemonDestroy(pokemon_copy);
		return POKEMON_TRAINER_PARTY_FULL;
	}else{
	   (trainer->trainer_pokemons_array)[trainer->local_counter] = pokemon_copy;
	   trainer->local_counter = trainer->local_counter +1;
	}

	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
//IMPORTANT NOTE : in the implementation pokemons have indexes 0--> n-1
Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index) {
	if (trainer == NULL)	return NULL;
	if (pokemon_index < 1 || pokemon_index > trainer->local_counter ){
		return NULL;
	}
	assert(trainer != NULL     							  &&
		   pokemon_index >=1 							  &&
		   pokemon_index <= trainer->local_counter);

	return (trainer->trainer_pokemons_array)[pokemon_index - 1];
}
//-----------------------------------------------------------------------------
//IMPORTANT NOTE : in the implementation pokemons have indexes 0--> n-1
PokemonTrainerResult pokemonTrainerRemovePokemon(
								   PokemonTrainer trainer, int pokemon_index) {
	if (trainer == NULL)	return POKEMON_TRAINER_NULL_ARG;
	if (pokemon_index < 1 || pokemon_index > trainer->local_counter ) {
		return POKEMON_TRAINER_INVALID_INDEX;
	}
	if (trainer->local_counter == 1)	return POKEMON_TRAINER_REMOVE_LAST;

	pokemonDestroy(   (trainer->trainer_pokemons_array)[pokemon_index - 1]   );

	for (int i = pokemon_index ; i < trainer->local_counter ; i++) {
		(trainer->trainer_pokemons_array)[i-1] =
										(trainer->trainer_pokemons_array)[i];
	}
	trainer->local_counter = trainer->local_counter - 1;
	(trainer->trainer_pokemons_array)[trainer->local_counter] = NULL;

	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
//IMPORTANT NOTE : in the implementation pokemons have indexes   0--> n-1
// 	    													and  0--> m-1
PokemonTrainerResult pokemonTrainerDepositPokemon(
								PokemonTrainer trainer, int pokemon_index) {
	if (trainer == NULL)	return POKEMON_TRAINER_NULL_ARG;
	if (pokemon_index < 1 || pokemon_index > trainer->local_counter ) {
		return POKEMON_TRAINER_INVALID_INDEX;
	}
	if (trainer->local_counter == 1)	return POKEMON_TRAINER_DEPOSIT_LAST;
	if (trainer->remote_counter == trainer->max_pokemons_remote) {
		return POKEMON_TRAINER_DEPOSIT_FULL;
	}

	Pokemon pokemon_ptr = pokemonTrainerGetPokemon(trainer,pokemon_index);
	(trainer->proffessor_pokemons_array)[trainer->remote_counter] = pokemon_ptr;
	trainer->remote_counter = trainer->remote_counter + 1;
	for (int i = pokemon_index ; i < trainer->local_counter ; i++) {
		(trainer->trainer_pokemons_array)[i-1] =
										(trainer->trainer_pokemons_array)[i];
	}
	trainer->local_counter = trainer->local_counter - 1;
	(trainer->trainer_pokemons_array)[trainer->local_counter] = NULL;

	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
//IMPORTANT NOTE : in the implementation pokemons have indexes   0--> n-1
// 	    													and  0--> m-1
PokemonTrainerResult pokemonTrainerWithdrawPokemon(
								PokemonTrainer trainer, int pokemon_index) {
	if (trainer == NULL)	return POKEMON_TRAINER_NULL_ARG;
	if (pokemon_index < 1   ||   pokemon_index > trainer->remote_counter) {
		return POKEMON_TRAINER_INVALID_INDEX;
	}
	if (trainer->local_counter == trainer->max_pokemons_local) {
		return POKEMON_TRAINER_PARTY_FULL;
	}

	Pokemon pokemon_ptr =
					(trainer->proffessor_pokemons_array)[pokemon_index - 1];
	(trainer->trainer_pokemons_array)[trainer->local_counter] = pokemon_ptr;
	trainer->local_counter = trainer->local_counter + 1;
	for (int i = pokemon_index ; i < trainer->remote_counter ; i++) {
		(trainer->proffessor_pokemons_array)[i-1] =
									(trainer->proffessor_pokemons_array)[i];
	}
	trainer->remote_counter = trainer->remote_counter - 1;
	(trainer->proffessor_pokemons_array)[trainer->remote_counter] = NULL;

	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer) {
	assert(trainer != NULL);

	return trainer->local_counter + trainer->remote_counter;
}
//-----------------------------------------------------------------------------
Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer) {
	if (trainer == NULL)	return NULL;
	assert(trainer != NULL);

	Pokemon* trainer_arr = trainer->trainer_pokemons_array;
	Pokemon* proffessor_arr = trainer->proffessor_pokemons_array;
	int local_size = trainer->local_counter;
	int remote_size = trainer->remote_counter;

	int most_ranked_index = NON_POSSIBLE_VALUE;
	int* most_ranked_index_ptr = &most_ranked_index;
	Pokemon trainer_most_ranked_pokemon = getMostRankedPokemonFromArray
								(trainer_arr,local_size,most_ranked_index_ptr);
	Pokemon proffessor_most_ranked_pokemon = getMostRankedPokemonFromArray
							(proffessor_arr,remote_size,most_ranked_index_ptr);
	assert(trainer_most_ranked_pokemon != NULL);

	if (proffessor_most_ranked_pokemon == NULL) {
		return trainer_most_ranked_pokemon;
	}
	assert(proffessor_most_ranked_pokemon != NULL);

	int trainer_max_rank = pokemonGetRank(trainer_most_ranked_pokemon);
	int proffessor_max_rank = pokemonGetRank(proffessor_most_ranked_pokemon);

	if (trainer_max_rank >= proffessor_max_rank) {
		return trainer_most_ranked_pokemon;
	}else{
		return proffessor_most_ranked_pokemon;
	}
}
//-----------------------------------------------------------------------------
PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer) {
	if (trainer == NULL)	return POKEMON_TRAINER_NULL_ARG;
	assert(trainer != NULL);

	Pokemon* pokemons_array = buildPokemonArrayFromTrainer(trainer);
	if (pokemons_array == NULL)		return POKEMON_TRAINER_OUT_OF_MEM;

	int pokemons_array_size = trainer->local_counter + trainer->remote_counter;
	PokemonTrainerResult result =
						sortPokemonsArray(pokemons_array,pokemons_array_size);
	if (result == POKEMON_TRAINER_OUT_OF_MEM) {
		return POKEMON_TRAINER_OUT_OF_MEM;
	}

	resetTrainer(trainer);
	trainerReinsertPokemons(trainer,pokemons_array,pokemons_array_size);
	destroyPokemonArray(pokemons_array,pokemons_array_size);

	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
PokemonTrainerResult pokemonTrainerPrintEnumeration(PokemonTrainer trainer,
																FILE* file) {
#define CHUNK_SIZE 256
	if (trainer == NULL   ||   file == NULL)	return POKEMON_TRAINER_NULL_ARG;
	PokemonTrainerResult result;
	char* pokemon_name = malloc( sizeof(*pokemon_name) * CHUNK_SIZE);
	if (pokemon_name == NULL) return POKEMON_TRAINER_OUT_OF_MEM;
	char* pokemon_voice = malloc( sizeof(*pokemon_voice) * CHUNK_SIZE);
	if (pokemon_voice == NULL) {
		free(pokemon_name);
		return POKEMON_TRAINER_OUT_OF_MEM;
	}

	for (int i = 0 ; i < trainer->local_counter ; i++) {
		result = printEnumerationWriteInputToFile
										(trainer->trainer_pokemons_array[i]);
		if ( result == POKEMON_TRAINER_OUT_OF_MEM) {
			return POKEMON_TRAINER_OUT_OF_MEM;
		}
		result = printEnumerationReadInputFromFile
				(trainer->trainer_pokemons_array[i],pokemon_name,pokemon_voice);
		if ( result == POKEMON_TRAINER_OUT_OF_MEM) {
			return POKEMON_TRAINER_OUT_OF_MEM;
		}
		printEnumerationWriteOutputToFile(trainer,pokemon_name,
														pokemon_voice,file);
	}
	free(pokemon_name);
	free(pokemon_voice);
	return POKEMON_TRAINER_SUCCESS;
}
//-----------------------------------------------------------------------------
//									EOF
//-----------------------------------------------------------------------------
