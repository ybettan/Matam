#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "parcer.h"
#include "pokemonGo.h"

#define MAX_COMMAND_LEN 10
#define MAX_NUM_OF_ARGUMENTS 4

#define FIRST_ARGUMENT 		args[0]
#define SECOND_ARGUMENT 	args[1]
#define THIRD_ARGUMENT 		args[2]
#define FORTH_ARGUMENT		args[3]

//=============================================================================
//								STATIC FUNCTIONS
//=============================================================================
//run an ADT command reguarding trainer action with the correct parameters
static PokemonGoResult translateAndRunTrainerCommand(char*sub_command,
													char** args,PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(sub_command,"add") == 0) {
		result = pokemonGoAddTrainer(pg,FIRST_ARGUMENT,atoi(SECOND_ARGUMENT),
											THIRD_ARGUMENT);
	}
	if (strcmp(sub_command,"go") == 0) {
		result = pokemonGoTrainerMoveAndHunt(pg,FIRST_ARGUMENT,SECOND_ARGUMENT);
	}
	if  (strcmp(sub_command,"purchase") == 0) {
		result = pokemonGoTrainerPurchase(pg,FIRST_ARGUMENT,SECOND_ARGUMENT,
													atoi(THIRD_ARGUMENT));
	}

	return result;
}
//------------------------------------------------------------------------------
//run an ADT command reguarding printing info action with the correct parameters
static PokemonGoResult translateAndRunReportCommand(char*sub_command,
											char** args,PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(sub_command,"trainer") == 0) {
		result = pokemonGoShowTrainerInfo(pg,FIRST_ARGUMENT);
	}
	if (strcmp(sub_command,"locations") == 0) {
		result = pokemonGoShowMapInfo(pg);
	}
	if (strcmp(sub_command,"stock") == 0) {
		result = pokemonGoShowStoreInfo(pg);
	}

	return result;
}
//------------------------------------------------------------------------------
//run an ADT command reguarding action on pokemon with the correct parameters
static PokemonGoResult translateAndRunPokemonCommand(char*sub_command,
													char** args,PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(sub_command,"heal") == 0) {
		result = pokemonGoPokemonHeal(pg,FIRST_ARGUMENT,atoi(SECOND_ARGUMENT));
	}
	if (strcmp(sub_command,"train") == 0) {
		result = pokemonGoPokemonTrain(pg,FIRST_ARGUMENT,atoi(SECOND_ARGUMENT));
	}

	return result;
}
//------------------------------------------------------------------------------
//run an ADT command reguarding store action with the correct parameters
static PokemonGoResult translateAndRunStoreCommand(char*sub_command,char** args,
															PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(sub_command,"add") == 0) {
		result = pokemonGoStoreAddItem(pg,FIRST_ARGUMENT,atoi(SECOND_ARGUMENT),
														atoi(THIRD_ARGUMENT));
	}

	return result;
}
//------------------------------------------------------------------------------
//run an ADT command reguarding battle action with the correct parameters
static PokemonGoResult translateAndRunBattleCommand(char*sub_command,
													char** args,PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(sub_command,"fight") == 0) {
		result = pokemonGoBattle(pg,FIRST_ARGUMENT,SECOND_ARGUMENT,
					atoi(THIRD_ARGUMENT),atoi(FORTH_ARGUMENT));
	}

	return result;
}
//-----------------------------------------------------------------------------
/* this function recives a command , a sub-command , and array of arguments ,
 *  main struct pg and output_channel and decside witch ATD command should
 * be ran and run it with the correct parameters
 */
static PokemonGoResult translateAndRun(char* command,char*sub_command,
												char** args,PokemonGo pg) {
	PokemonGoResult result;

	if (strcmp(command,"trainer") == 0) {
		result = translateAndRunTrainerCommand(sub_command,args,pg);
	}
	if (strcmp(command,"report") == 0) {
		result = translateAndRunReportCommand(sub_command,args,pg);
	}
	if (strcmp(command,"pokemon") == 0) {
		result = translateAndRunPokemonCommand(sub_command,args,pg);
	}
	if (strcmp(command,"store") == 0) {
		result = translateAndRunStoreCommand(sub_command,args,pg);
	}
	if (strcmp(command,"battle") == 0) {
		result = translateAndRunBattleCommand(sub_command,args,pg);
	}
	return result;
}
//-----------------------------------------------------------------------------
/* function makes a translation from PokemonGoResult to ParcerResult by the
 *  result received as an argument.
 *  returns the appropriate translation result.
 */
static ParcerResult pokemonGoResultToParcerResult(PokemonGoResult result) {

	switch(result) {

	case POKEMONGO_OUT_OF_MEMORY : return PARCER_OUT_OF_MEMORY;
	case POKEMONGO_NULL_ARGUMENT : return PARCER_NULL_ARGUMENT;
	case POKEMONGO_INVALID_ARGUMENT : return PARCER_INVALID_ARGUMENT;
	case POKEMONGO_TRAINER_NAME_ALREADY_EXISTS :
									return PARCER_TRAINER_NAME_ALREADY_EXISTS;
	case POKEMONGO_TRAINER_DOES_NOT_EXIST :return PARCER_TRAINER_DOES_NOT_EXIST;
	case POKEMONGO_LOCATION_DOES_NOT_EXIST :
									return PARCER_LOCATION_DOES_NOT_EXIST;
	case POKEMONGO_POKEMON_DOES_NOT_EXIST:return PARCER_POKEMON_DOES_NOT_EXIST;
	case POKEMONGO_ITEM_OUT_OF_STOCK : return PARCER_ITEM_OUT_OF_STOCK;
	case POKEMONGO_BUDGET_IS_NOT_SUFFICIENT :
										return PARCER_BUDGET_IS_NOT_SUFFICIENT;
	case POKEMONGO_HP_IS_AT_MAX : return PARCER_HP_IS_AT_MAX;
	case POKEMONGO_NO_AVAILABLE_ITEM_FOUND :
										return PARCER_NO_AVAILABLE_ITEM_FOUND;
	case POKEMONGO_TRAINER_ALREADY_IN_LOCATION :
									return PARCER_TRAINER_ALREADY_IN_LOCATION;

	case POKEMONGO_LOCATION_IS_NOT_REACHABLE :
									return PARCER_LOCATION_IS_NOT_REACHABLE;
	default : return PARCER_SUCCESS;
	}
}


//=============================================================================
//								HEADER FUNCTIONS
//=============================================================================
ParcerResult parcerRunAdtFunction(char* unix_command,PokemonGo pg) {

	if ( !unix_command )	return PARCER_NULL_ARGUMENT;

	char* command = strtok(unix_command," \t");
	char* sub_command = strtok(NULL," \t");
	char* args[MAX_NUM_OF_ARGUMENTS];

	if (command == NULL   ||   sub_command == NULL)		return PARCER_SUCCESS;

	for (int i = 0 ; i < MAX_NUM_OF_ARGUMENTS ; i++) {
		args[i] = strtok(NULL," \t");
	}
	/* args[] containe now all unix_command arguments continued by NULL untill
	 * the end of the array
	 */

	PokemonGoResult result = translateAndRun(command,sub_command,args,pg);

	return pokemonGoResultToParcerResult(result);
}
