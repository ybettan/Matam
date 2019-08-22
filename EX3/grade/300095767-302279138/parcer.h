#ifndef PARCER_H_
#define PARCER_H_

#include "pokemonGo.h"

//possible errors
typedef enum {
	PARCER_OUT_OF_MEMORY,
	PARCER_NULL_ARGUMENT,
	PARCER_INVALID_ARGUMENT,
	PARCER_TRAINER_NAME_ALREADY_EXISTS,
	PARCER_TRAINER_DOES_NOT_EXIST,
	PARCER_LOCATION_DOES_NOT_EXIST,
	PARCER_POKEMON_DOES_NOT_EXIST,
	PARCER_ITEM_OUT_OF_STOCK,
	PARCER_BUDGET_IS_NOT_SUFFICIENT,
	PARCER_HP_IS_AT_MAX,
	PARCER_NO_AVAILABLE_ITEM_FOUND,
	PARCER_LOCATION_IS_NOT_REACHABLE,
	PARCER_TRAINER_ALREADY_IN_LOCATION,
	PARCER_SUCCESS
} ParcerResult;

/* this function recives a unix command with its parameters and tokenize it into
 * command , sub-command and parameters and call the correct ADT function with
 * its correct parameters and with main struct pg
 * assumes with assert the command is correct or an empty command ("    ")
 *
 * -----Return------
 *
 *	PARCER_OUT_OF_MEMORY :				- if any memory allocation failed
 *  PARCER_NULL_ARGUMENT:				- if a NULL argument was sent
 *	PARCER_INVALID_ARGUMENT :
 *		"trainer add" 					- budget isn't an integer >= 0
 *		"store add"/"trainer purchase" 	- if item isn't "potion" or "candy"
 *  									- quantity/value are not integer > 0
 *  	"battle fight"					- if trainer1 == trainer2
 *  PARCER_TRAINER_NAME_ALREADY_EXISTS :
 *  	"trainer add"					- trainer alredy exist in game
 *  PARCER_TRAINER_DOES_NOT_EXIST :
 *  	"trainer go"/"trainer purchase"/
 *  	"battle fight"/"pokemon heal"/
 *  	"pokemon traine"/"report trainer"- if trainer doesn't exist in game
 *  PARCER_LOCATION_DOES_NOT_EXIST :
 *  	"trainer add"/"trainer go"		- if location doesn't exist in game
 *  PARCER_POKEMON_DOES_NOT_EXIST :
 *  	"battle fight"/"pokemon heal"/
 *  				"pokemon trainer"	- if trainer doesn't have any pokemon
 *  														with pokemon_id
 *  PARCER_ITEM_OUT_OF_STOCK :
 *  	"trainer purchase"				- if item is not in store stock
 *  PARCER_BUDGET_IS_NOT_SUFFICIENT :
 *  	"trainer purchase"				- if trainer don't have enought mony to
 *  														make the purchase
 *  PARCER_HP_IS_AT_MAX :
 *  	"pokemon heal"					- if given pokemon alredy have HP = 100
 *  PARCER_NO_AVAILABLE_ITEM_FOUND :
 *  	"pokemon heal"					- if trainer don't have any potion
 *  	"pokemon traine"				- if trainer don't have any candy
 *  PARCER_LOCATION_IS_NOT_REACHABLE :
 *  	"trainer go"					- if location is not "one step reachable"
 *  													from current location
 *  PARCER_TRAINER_ALREADY_IN_LOCATION :
 *  	"trainer go"					- if trainer is alredy in location
 *
 *	PARCER_SUCCESS - if there wasn't any error
 */
ParcerResult parcerRunAdtFunction(char* unix_command,PokemonGo pg);

#endif /* PARCER_H_ */
