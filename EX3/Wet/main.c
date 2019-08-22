#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "print_utils.h"
#include "parcer.h"
#include "pokemonGo.h"

#define MAX_COMMAND_LEN 256

#define MIN_NUM_OF_ARGUMENTS 7
#define MED_NUM_OF_ARGUMENTS 9
#define MAX_NUM_OF_ARGUMENTS 11

#define POKEDEX_FLAG 1
#define POKEDEX_FILE 2
#define EVOLUTIONS_FLAG 3
#define EVOLUTIONS_FILE 4
#define LOCATIONS_FLAG 5
#define LOCATIONS_FILE 6

#define FIRST_OPTIONAL_FLAG 7
#define FIRST_OPTIONAL_FILE 8
#define SECOND_OPTIONAL_FLAG 9
#define SECOND_OPTIONAL_FILE 10

#define FLAG 0
#define FLAG_1 0
#define FLAG_2 2



//=============================================================================
//								STATIC FUNCTIONS
//=============================================================================

/* assuming we have at least all must program inputs :
 * (pokemon_go_mtm –p <pokedex_file> -e <evolutions_file> -l <locations_file>)
 * (argc >= MIN_NUM_OF_ARGUMENTS)
 * the function checks if the command line is legal
 * -----return-------
 * true if the command line is legal
 * false otherwise
 */
static bool isValidCommanLineParametersBasic(int argc,char** argv) {

	if (argc < MIN_NUM_OF_ARGUMENTS) {
		return false;
	}
	if (strcmp(argv[POKEDEX_FLAG],"-p")!=0 ) {
		return false;
	}
	if (strcmp(argv[EVOLUTIONS_FLAG],"-e")!=0 ) {
		return false;
	}
	if (strcmp(argv[LOCATIONS_FLAG],"-l")!=0 ) {
		return false;
	}

	return true;
}

/* assuming we have all must program inputs correct and we have
 * 1 more flag and file : [-i <input_file>] or [-o <output_file>]
 * (argc == MED_NUM_OF_ARGUMENTS)
 * the function checks if the command line is legal
 * -----return-------
 * true if the command line is legal
 * false otherwise
 */
static bool isValidCommanLineParametersOneExtention(char** small_extention) {

	if (	strcmp(small_extention[FLAG],"-i") != 0		&&
			strcmp(small_extention[FLAG],"-o") != 0				) {
		return false;
	}

	return true;
}

/* assuming we have all must program inputs correct and we hava
 * 2 more flag and file : [-i <input_file>] and [-o <output_file>]
 * (argc == MAX_NUM_OF_ARGUMENTS)
 * the function checks if the command line is legal
 * -----return-------
 * true if the command line is legal
 * false otherwise
 */
static bool isValidCommanLineParametersTwoExtention(char** large_extention) {

	if (	strcmp(large_extention[FLAG_1],"-i") != 0	||
			strcmp(large_extention[FLAG_2],"-o") != 0		 ) {

		return false;
	}

	return true;
}

/* the function checks if the command line is legal
 * -----return-------
 * true if the command line is legal
 * false otherwise
 */
static bool isValidCommanLineParameters(int argc,char** argv) {

	bool result = true;

	if (	argc != MIN_NUM_OF_ARGUMENTS  &&
			argc != MED_NUM_OF_ARGUMENTS  &&
			argc != MAX_NUM_OF_ARGUMENTS		) {

		result = false;
	}
	if ( !isValidCommanLineParametersBasic(argc,argv) ) {
		result = false;
	}
	if ( argc == MED_NUM_OF_ARGUMENTS) {
		if (!isValidCommanLineParametersOneExtention(argv+FIRST_OPTIONAL_FLAG)){
			result = false;
		}
	}
	if ( argc == MAX_NUM_OF_ARGUMENTS) {
		if (!isValidCommanLineParametersTwoExtention(argv+FIRST_OPTIONAL_FLAG)){
			result = false;
		}
	}

	return result;
}
//------------------------------------------------------------------------------

/* close all files at once
 * can recive also NULL - in this case nothing will be done to NULL the file
 */
static void closeFiles(FILE* pokedex,FILE* evolutions,FILE* locations) {

	if (pokedex != NULL) {
		fclose(pokedex);
	}
	if (evolutions != NULL) {
		fclose(evolutions);
	}
	if (locations != NULL) {
		fclose(locations);
	}
}

/* open pokedex_file , evolutions_files and locations_files and insert them into
 * there correct pointers
 * -----return-------
 * false if any file failed to be opened
 * true otherwise
 */
static bool openFiles(FILE** pokedex_f,char* pokedex_name,FILE** evolutions_f,
			char* evolutions_name,FILE** locations_f,char* locations_name) {

	*pokedex_f = fopen(pokedex_name,"r");
	*evolutions_f = fopen(evolutions_name,"r");
	*locations_f = fopen(locations_name,"r");

	if ( *pokedex_f == NULL || *evolutions_f == NULL || *locations_f == NULL) {
		closeFiles(*pokedex_f,*evolutions_f,*locations_f);
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
/* initiallize input and output channels to be taken from files or from
 * standart input/output
 * -----return-------
 * false if the function failed to open files
 * true otherwise
 */
static bool initiallizeChannels(int argc,char**argv,FILE** input_file,
													FILE** output_file) {
	switch (argc) {
	case MED_NUM_OF_ARGUMENTS :
		if ( strcmp(argv[FIRST_OPTIONAL_FLAG],"-i") == 0) {
			*input_file = fopen(argv[FIRST_OPTIONAL_FILE],"r");
			if ( !*input_file )		return false;
			*output_file = stdout;
		}else{
			*input_file = stdin;
			*output_file = fopen(argv[FIRST_OPTIONAL_FILE],"w");
			if ( !*output_file )	return false;
		}
		break;
	case MAX_NUM_OF_ARGUMENTS :
		*input_file = fopen(argv[FIRST_OPTIONAL_FILE],"r");
		if ( !*input_file )		return false;
		*output_file = fopen(argv[SECOND_OPTIONAL_FILE],"w");
		if ( !*output_file ) {
			fclose(*input_file);
			return false;
		}
		break;
	default :
		*input_file = stdin;
		*output_file = stdout;
		break;
	}
	return true;
}
//-----------------------------------------------------------------------------

//close the input/output channels
static void closeChannels(FILE* input_channel,FILE* output_channel) {

	if (input_channel != stdin && input_channel != NULL) {
		fclose(input_channel);
	}
	if (output_channel != stdout && output_channel != NULL) {
		fclose(output_channel);
	}
}
//-----------------------------------------------------------------------------
// translate ParcerResult to MtmErrorCode
static MtmErrorCode parcerToMtmErrorCode(ParcerResult result) {

	switch(result){

	case PARCER_OUT_OF_MEMORY : return MTM_OUT_OF_MEMORY;
	case PARCER_INVALID_ARGUMENT : return MTM_INVALID_ARGUMENT;
	case PARCER_TRAINER_NAME_ALREADY_EXISTS :
									return MTM_TRAINER_NAME_ALREADY_EXISTS;
	case PARCER_TRAINER_DOES_NOT_EXIST :return MTM_TRAINER_DOES_NOT_EXIST;
	case PARCER_LOCATION_DOES_NOT_EXIST :
									return MTM_LOCATION_DOES_NOT_EXIST;
	case PARCER_POKEMON_DOES_NOT_EXIST:return MTM_POKEMON_DOES_NOT_EXIST;
	case PARCER_ITEM_OUT_OF_STOCK : return MTM_ITEM_OUT_OF_STOCK;
	case PARCER_BUDGET_IS_NOT_SUFFICIENT :
										return MTM_BUDGET_IS_NOT_SUFFICIENT;
	case PARCER_HP_IS_AT_MAX : return MTM_HP_IS_AT_MAX;
	case PARCER_NO_AVAILABLE_ITEM_FOUND :
										return MTM_NO_AVAILABLE_ITEM_FOUND;
	case PARCER_TRAINER_ALREADY_IN_LOCATION :
									return MTM_TRAINER_ALREADY_IN_LOCATION;

	case PARCER_LOCATION_IS_NOT_REACHABLE :
									return MTM_LOCATION_IS_NOT_REACHABLE;

	default : return MTM_SUCCESS;
	}
}
//-----------------------------------------------------------------------------
/* execute all the command from the input channel
 * -----return-----
 * false if any memory allocation failed
 * true otherwise
 */
static bool executeCommands(PokemonGo pg,FILE* input_channel) {

	char input_buffer[MAX_COMMAND_LEN];
	char* buffer;
	MtmErrorCode result;

	while ( fgets(input_buffer,MAX_COMMAND_LEN,input_channel) ) {

		buffer = strtok(input_buffer,"\n\r");
		result = parcerToMtmErrorCode(parcerRunAdtFunction(buffer,pg));
		if (result != MTM_SUCCESS) {
			mtmPrintErrorMessage(stderr,result);
		}
		if (result == MTM_OUT_OF_MEMORY)	return false;
	}
	return true;
}


//=============================================================================
//								MAIN
//=============================================================================

int main(int argc,char** argv) {

	if ( !isValidCommanLineParameters(argc,argv) ) {
		mtmPrintErrorMessage(stderr,MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 1;
	}
	FILE *pokedex_f = NULL , *evolutions_f = NULL , *locations_f = NULL;
	if ( !openFiles(&pokedex_f,argv[POKEDEX_FILE],&evolutions_f,
				argv[EVOLUTIONS_FILE],&locations_f,argv[LOCATIONS_FILE]) ) {
		mtmPrintErrorMessage(stderr,MTM_CANNOT_OPEN_FILE);
		return 1;
	}
	FILE *input_channel = NULL  ,  *output_channel = NULL;
	if ( !initiallizeChannels(argc,argv,&input_channel,&output_channel) ) {
		mtmPrintErrorMessage(stderr,MTM_CANNOT_OPEN_FILE);
		return 1;
	}
	PokemonGo pg =
			pokemonGoCreate(pokedex_f,evolutions_f,locations_f,output_channel);
	if ( !pg ) {
		mtmPrintErrorMessage(stderr,MTM_OUT_OF_MEMORY);
		return 1;
	}
	closeFiles(pokedex_f,evolutions_f,locations_f);

	if ( !executeCommands(pg,input_channel) )	return 1;
	pokemonGoDestroy(pg);
	closeChannels(input_channel,output_channel);
}
