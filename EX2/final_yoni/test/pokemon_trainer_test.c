#include <stdbool.h>
#include "pokemon_trainer.h"
#include "aux_macros.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
//=============================================================================
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define TEST

#ifdef TEST
//=============================================================================
// 							USEFULL FUNCTION
//=============================================================================

static void errorTrainerCreate(bool* result_ptr,char* name,
			Pokemon initial_pokemon,int max_num_local, int max_num_remote) {

		TEST_EQUALS((*result_ptr), (pokemonTrainerCreate(name,initial_pokemon,
										max_num_local,max_num_remote)),(NULL));
}

static void testTrainerCreate(bool* result_ptr,char* name,int max_num_local,
		int max_num_remote) {

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate(name,pokemon,max_num_local,
																max_num_remote);
	if (trainer == NULL) { //malloc failled
		TEST_EQUALS((*result_ptr),(trainer),(NULL));
	}else{
		TEST_DIFFERENT((*result_ptr),(trainer),(NULL));
	}

	pokemonDestroy(pokemon);
	pokemonTrainerDestroy(trainer);
}
//-----------------------------------------------------------------------------
static void errorTrainerCopy(bool* result_ptr) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerCopy(NULL)),(NULL));
}

static void testTrainerCopy(bool* result_ptr,PokemonTrainer trainer) {

	PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
	if (trainer == NULL) { //malloc failled
		TEST_EQUALS((*result_ptr),(trainer_copy),(NULL));
	}else{
		TEST_DIFFERENT((*result_ptr),(trainer_copy),(NULL));
	}

	pokemonTrainerDestroy(trainer_copy);
}
//-----------------------------------------------------------------------------
static void errorTrainerAddPokemon(bool* result_ptr,PokemonTrainer trainer,
															Pokemon pokemon) {

	if (trainer == NULL  ||  pokemon == NULL) {
		TEST_EQUALS((*result_ptr),(pokemonTrainerAddPokemon(trainer,pokemon)),
												(POKEMON_TRAINER_NULL_ARG));
		return;
	}
	Pokemon pokemon_int = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer_int = pokemonTrainerCreate("Ash", pokemon_int,1,1);
	TEST_EQUALS((*result_ptr),(pokemonTrainerAddPokemon(trainer_int,pokemon)),
												(POKEMON_TRAINER_PARTY_FULL));
	pokemonDestroy(pokemon_int);
	pokemonTrainerDestroy(trainer_int);
}

static void testTrainerAddPokemon(bool* result_ptr,PokemonTrainer trainer,
															Pokemon pokemon) {
	if (trainer == NULL) { //previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerAddPokemon(trainer,pokemon)),
												(POKEMON_TRAINER_NULL_ARG));
	}else{
		TEST_EQUALS((*result_ptr),(pokemonTrainerAddPokemon(trainer,pokemon)),
													(POKEMON_TRAINER_SUCCESS));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerGetPokemon(bool* result_ptr,PokemonTrainer trainer,
																int index) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerGetPokemon(trainer,index)),(NULL));
}

static void testTrainerGetPokemon(bool* result_ptr,PokemonTrainer trainer,
																int index) {
	if (trainer == NULL) { //previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerGetPokemon(trainer,index)),
																	(NULL));
	}else{
		TEST_DIFFERENT((*result_ptr),(pokemonTrainerGetPokemon(trainer,index)),
																	(NULL));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerRemovePokemon(bool* result_ptr,PokemonTrainer trainer,
							int index,PokemonTrainerResult expected_result) {

	if (trainer == NULL) { //previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerRemovePokemon(trainer,index)),
												(POKEMON_TRAINER_NULL_ARG ));
	}else{
	TEST_EQUALS((*result_ptr),(pokemonTrainerRemovePokemon(trainer,index)),
														(expected_result));
	}
}

static void testTrainerRemovePokemon(bool* result_ptr,PokemonTrainer trainer,
																int index) {

	if (trainer == NULL) { //previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerRemovePokemon(trainer,index)),
												(POKEMON_TRAINER_NULL_ARG));
	}else{
		TEST_EQUALS((*result_ptr),(pokemonTrainerRemovePokemon(trainer,index)),
												(POKEMON_TRAINER_SUCCESS));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerDepositPokemon(bool* result_ptr,PokemonTrainer trainer,
							int index,PokemonTrainerResult expected_result) {

	if (trainer == NULL) {//previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerDepositPokemon(trainer,index)),
													(POKEMON_TRAINER_NULL_ARG));
	}else{
	TEST_EQUALS((*result_ptr),(pokemonTrainerDepositPokemon(trainer,index)),
															(expected_result));
	}
}

static void testTrainerDepositPokemon(bool* result_ptr,PokemonTrainer trainer,
																int index) {

	if (trainer == NULL) {//previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerDepositPokemon(trainer,index)),
													(POKEMON_TRAINER_NULL_ARG));
	}else{
		TEST_EQUALS((*result_ptr),(pokemonTrainerDepositPokemon(trainer,index)),
													(POKEMON_TRAINER_SUCCESS));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerWithdrawPokemon(bool* result_ptr,PokemonTrainer trainer,
							int index,PokemonTrainerResult expected_result) {

	if (trainer == NULL) {//previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerWithdrawPokemon
								(trainer,index)),(POKEMON_TRAINER_NULL_ARG));
	}else{
		TEST_EQUALS((*result_ptr),(pokemonTrainerWithdrawPokemon
										(trainer,index)),(expected_result));
	}
}

static void testTrainerWithdrawPokemon(bool* result_ptr,PokemonTrainer trainer,
																int index) {

	if (trainer == NULL) {//previous allocation failed
		TEST_EQUALS((*result_ptr),(pokemonTrainerWithdrawPokemon
								(trainer,index)),(POKEMON_TRAINER_NULL_ARG));
	}else{
	TEST_EQUALS((*result_ptr),(pokemonTrainerWithdrawPokemon(trainer,index)),
													(POKEMON_TRAINER_SUCCESS));
	}
}
//-----------------------------------------------------------------------------
static void testTrainerGetNumberOfPokemons(bool* result_ptr,
							PokemonTrainer trainer,int number_of_pokemons) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerGetNumberOfPokemons(trainer)),
														(number_of_pokemons));
}
//-----------------------------------------------------------------------------
static void errorTrainerGetMostRankedPokemon(bool* result_ptr,
													PokemonTrainer trainer) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerGetMostRankedPokemon(trainer)),
																		(NULL));
}

static void testTrainerGetMostRankedPokemon(bool* result_ptr,
													PokemonTrainer trainer) {

	if (trainer == NULL) {//previous failurs of malloc
		TEST_EQUALS((*result_ptr),(pokemonTrainerGetMostRankedPokemon(trainer))
																	,(NULL));
	}else{
	TEST_DIFFERENT((*result_ptr),(pokemonTrainerGetMostRankedPokemon(trainer))
																	,(NULL));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerMakeMostRankedParty(bool* result_ptr,
													PokemonTrainer trainer) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerMakeMostRankedParty(trainer)),
												(POKEMON_TRAINER_NULL_ARG));
}

static void testTrainerMakeMostRankedParty(bool* result_ptr,
													PokemonTrainer trainer) {

	if (trainer == NULL) {//previous failurs of malloc
		TEST_EQUALS((*result_ptr),(pokemonTrainerMakeMostRankedParty(trainer)),
													(POKEMON_TRAINER_NULL_ARG));
	}else{
	TEST_EQUALS((*result_ptr),(pokemonTrainerMakeMostRankedParty(trainer)),
													(POKEMON_TRAINER_SUCCESS));
	}
}
//-----------------------------------------------------------------------------
static void errorTrainerPrintEnumeration(bool* result_ptr,
										PokemonTrainer trainer,FILE* file) {

	TEST_EQUALS((*result_ptr),(pokemonTrainerPrintEnumeration(trainer,file)),
												(POKEMON_TRAINER_NULL_ARG ));
}
//=============================================================================
//								testCombo
//=============================================================================
static bool testCombo() {

	bool result = true;

    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_DIFFERENT(result, trainer, NULL);
	}else{
		TEST_EQUALS(result, trainer, NULL);
	}
	pokemonDestroy(pikachu);

    PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_DIFFERENT(result, trainer_copy, NULL);
	}else{
		TEST_EQUALS(result, trainer_copy, NULL);
	}
	pokemonTrainerDestroy(trainer_copy);

    Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
    PokemonTrainerResult score = pokemonTrainerAddPokemon(trainer, squirtle);
	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result,score,POKEMON_TRAINER_SUCCESS);
	}else{
		TEST_EQUALS(result,score,POKEMON_TRAINER_NULL_ARG);
	}
	pokemonDestroy(squirtle);

	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);
	}
	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
	}else{
		TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
	}
	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
    												POKEMON_TRAINER_SUCCESS);
	}else{
		TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
												POKEMON_TRAINER_NULL_ARG);
	}

	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
    												POKEMON_TRAINER_SUCCESS);
	}else{
		TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
													POKEMON_TRAINER_NULL_ARG);
	}

	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
													POKEMON_TRAINER_SUCCESS);
	}else{
		TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
													POKEMON_TRAINER_NULL_ARG);
	}

	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
										pokemonTrainerGetPokemon(trainer,1));
	}else{
		TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
																		NULL);
	}

	if (trainer != NULL) {//to exclude the case that malloc failed
		TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
													POKEMON_TRAINER_SUCCESS);
	}else{
		TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
													POKEMON_TRAINER_NULL_ARG);
	}

    pokemonTrainerDestroy(trainer);

    return result;
}
//=============================================================================
//						pokemonTrainerCreate
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerCreate() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);

	errorTrainerCreate(&result,NULL,pokemon,100,5);
	errorTrainerCreate(&result,"",pokemon,100,5);
	errorTrainerCreate(&result,"Ash",NULL,100,5);
	errorTrainerCreate(&result,"Ash",pokemon,0,5);
	errorTrainerCreate(&result,"Ash",pokemon,-3,5);
	errorTrainerCreate(&result,"Ash",pokemon,6,0);
	errorTrainerCreate(&result,"Ash",pokemon,6,-4);

	testTrainerCreate(&result,"Ash",5,5);
	testTrainerCreate(&result,"Gary",6,1);
	testTrainerCreate(&result,"Brook",1,1);

	pokemonDestroy(pokemon);

    return result;
}
//=============================================================================
//						pokemonTrainerDestroy
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerDestroy() {

	bool result = true;

//checked in valgrind

    return result;
}
//=============================================================================
//						pokemonTrainerCopy
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerCopy() {

	bool result = true;

	errorTrainerCopy(&result);

	Pokemon pokemon1 = pokemonCreate("bullbasaur",TYPE_GRASS, 20, 4);
	Pokemon pokemon2 = pokemonCreate("charmander",TYPE_FIRE, 20, 4);
	Pokemon pokemon3 = pokemonCreate("squirtel",TYPE_WATER, 20, 4);
	Pokemon pokemon4 = pokemonCreate("Ho-ho",TYPE_NORMAL, 20, 4);
	Pokemon pokemon5 = pokemonCreate("electrobuz",TYPE_ELECTRIC, 20, 4);

	PokemonTrainer trainer1 = pokemonTrainerCreate("Ash",pokemon1,6,6);
	PokemonTrainer trainer2 = pokemonTrainerCreate("Brook",pokemon2,6,6);
	PokemonTrainer trainer3 = pokemonTrainerCreate("Gary",pokemon3,6,6);
	PokemonTrainer trainer4 = pokemonTrainerCreate("Misty",pokemon4,6,6);
	PokemonTrainer trainer5 = pokemonTrainerCreate("Ohk",pokemon5,6,6);

	testTrainerCopy(&result,trainer1);
	testTrainerCopy(&result,trainer2);
	testTrainerCopy(&result,trainer3);
	testTrainerCopy(&result,trainer4);
	testTrainerCopy(&result,trainer5);

	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);

	pokemonTrainerDestroy(trainer1);
	pokemonTrainerDestroy(trainer2);
	pokemonTrainerDestroy(trainer3);
	pokemonTrainerDestroy(trainer4);
	pokemonTrainerDestroy(trainer5);

    return result;
}
//=============================================================================
//					pokemonTrainerAddPokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerAddPokemon() {

	bool result = true;

    Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon, 6, 20);

	errorTrainerAddPokemon(&result,NULL,pokemon);
	errorTrainerAddPokemon(&result,trainer,NULL);

	Pokemon pokemon1 = pokemonCreate("evey",TYPE_NORMAL,10,10);
	Pokemon pokemon2 = pokemonCreate("electrobuz",TYPE_ELECTRIC,10,10);
	Pokemon pokemon3 = pokemonCreate("Gramer",TYPE_FIRE,10,10);

	errorTrainerAddPokemon(&result,trainer,pokemon1);
	errorTrainerAddPokemon(&result,trainer,pokemon2);

	testTrainerAddPokemon(&result,trainer,pokemon1);
	testTrainerAddPokemon(&result,trainer,pokemon2);
	testTrainerAddPokemon(&result,trainer,pokemon3);

	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonTrainerDestroy(trainer);

    return result;
}
//=============================================================================
//					pokemonTrainerGetPokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerGetPokemon() {

	bool result = true;

    Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 6);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	errorTrainerGetPokemon(&result,NULL,2);
	errorTrainerGetPokemon(&result,NULL,0);
	errorTrainerGetPokemon(&result,trainer,-2);
	errorTrainerGetPokemon(&result,trainer,7);
	errorTrainerGetPokemon(&result,trainer,100);
	errorTrainerGetPokemon(&result,trainer,2);

	Pokemon pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	Pokemon pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
	Pokemon pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
	Pokemon pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
	Pokemon pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
	Pokemon pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);
	Pokemon pokemon7 = pokemonCreate ("Electrobuz",TYPE_ELECTRIC,50,50);
	Pokemon pokemon8 = pokemonCreate ("Tondebolt",TYPE_ELECTRIC,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);
	pokemonTrainerAddPokemon(trainer,pokemon3);
	pokemonTrainerAddPokemon(trainer,pokemon4);
	pokemonTrainerAddPokemon(trainer,pokemon5);
	pokemonTrainerAddPokemon(trainer,pokemon6);
	pokemonTrainerAddPokemon(trainer,pokemon7);
	pokemonTrainerAddPokemon(trainer,pokemon8);

	testTrainerGetPokemon(&result,trainer,1);
	testTrainerGetPokemon(&result,trainer,2);
	testTrainerGetPokemon(&result,trainer,3);
	testTrainerGetPokemon(&result,trainer,4);
	testTrainerGetPokemon(&result,trainer,5);
	testTrainerGetPokemon(&result,trainer,6);
	testTrainerGetPokemon(&result,trainer,7);
	testTrainerGetPokemon(&result,trainer,8);
	testTrainerGetPokemon(&result,trainer,9);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);
	pokemonDestroy(pokemon6);
	pokemonDestroy(pokemon7);
	pokemonDestroy(pokemon8);

    return result;
}
//=============================================================================
//					pokemonTrainerRemovePokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerRemovPokemon() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	errorTrainerRemovePokemon(&result,NULL,1,POKEMON_TRAINER_NULL_ARG);
	errorTrainerRemovePokemon(&result,trainer,0,POKEMON_TRAINER_INVALID_INDEX);
	errorTrainerRemovePokemon(&result,trainer,2,POKEMON_TRAINER_INVALID_INDEX);
	errorTrainerRemovePokemon(&result,trainer,-5,POKEMON_TRAINER_INVALID_INDEX);
	errorTrainerRemovePokemon(&result,trainer,10,POKEMON_TRAINER_INVALID_INDEX);

	Pokemon pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	Pokemon pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
	Pokemon pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
	Pokemon pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
	Pokemon pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
	Pokemon pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);
	Pokemon pokemon7 = pokemonCreate ("Electrobuz",TYPE_ELECTRIC,50,50);
	Pokemon pokemon8 = pokemonCreate ("Tondebolt",TYPE_ELECTRIC,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);
	pokemonTrainerAddPokemon(trainer,pokemon3);
	pokemonTrainerAddPokemon(trainer,pokemon4);
	pokemonTrainerAddPokemon(trainer,pokemon5);
	pokemonTrainerAddPokemon(trainer,pokemon6);
	pokemonTrainerAddPokemon(trainer,pokemon7);
	pokemonTrainerAddPokemon(trainer,pokemon8);

	testTrainerRemovePokemon(&result,trainer,1);
	testTrainerRemovePokemon(&result,trainer,8);
	testTrainerRemovePokemon(&result,trainer,7);
	testTrainerRemovePokemon(&result,trainer,2);
	testTrainerRemovePokemon(&result,trainer,5);
	testTrainerRemovePokemon(&result,trainer,4);
	testTrainerRemovePokemon(&result,trainer,1);
	testTrainerRemovePokemon(&result,trainer,1);

	errorTrainerRemovePokemon(&result,trainer,1,POKEMON_TRAINER_REMOVE_LAST);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);
	pokemonDestroy(pokemon6);
	pokemonDestroy(pokemon7);
	pokemonDestroy(pokemon8);

    return result;
}
//=============================================================================
//					pokemonTrainerDepositPokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerDespositPokemon() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,3,3);

  errorTrainerDepositPokemon(&result,NULL,1,POKEMON_TRAINER_NULL_ARG);
  errorTrainerDepositPokemon(&result,trainer,0,POKEMON_TRAINER_INVALID_INDEX);
  errorTrainerDepositPokemon(&result,trainer,2,POKEMON_TRAINER_INVALID_INDEX);
  errorTrainerDepositPokemon(&result,trainer,-10,POKEMON_TRAINER_INVALID_INDEX);
  errorTrainerDepositPokemon(&result,trainer,1,POKEMON_TRAINER_DEPOSIT_LAST);

	Pokemon pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	Pokemon pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);

	testTrainerDepositPokemon(&result,trainer,2);
	testTrainerDepositPokemon(&result,trainer,1);

	errorTrainerDepositPokemon(&result,trainer,1,POKEMON_TRAINER_DEPOSIT_LAST);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
    return result;
}
//=============================================================================
//					pokemonTrainerWithdrawPokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerWithdrawPokemon() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,4,4);

	Pokemon pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	Pokemon pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
	Pokemon pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);
	pokemonTrainerAddPokemon(trainer,pokemon3);

	pokemonTrainerDepositPokemon(trainer,1);
	pokemonTrainerDepositPokemon(trainer,1);

errorTrainerWithdrawPokemon(&result,NULL,1,POKEMON_TRAINER_NULL_ARG );
errorTrainerWithdrawPokemon(&result,trainer,0,POKEMON_TRAINER_INVALID_INDEX);
errorTrainerWithdrawPokemon(&result,trainer,-100,POKEMON_TRAINER_INVALID_INDEX);
errorTrainerWithdrawPokemon(&result,trainer,3,POKEMON_TRAINER_INVALID_INDEX);
errorTrainerWithdrawPokemon(&result,trainer,100,POKEMON_TRAINER_INVALID_INDEX);

	Pokemon pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
	Pokemon pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon4);
	pokemonTrainerAddPokemon(trainer,pokemon5);

	errorTrainerWithdrawPokemon(&result,trainer,1,POKEMON_TRAINER_PARTY_FULL);

	pokemonTrainerRemovePokemon(trainer,1);
	pokemonTrainerRemovePokemon(trainer,1);

	testTrainerWithdrawPokemon(&result,trainer,2);
	testTrainerWithdrawPokemon(&result,trainer,1);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);

    return result;
}
//=============================================================================
//					pokemonTrainerGetNumberOfPokemons
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted

//no errors possible in this function
static bool TrainerGetNumberOfPokemons() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	Pokemon pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	Pokemon pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
	Pokemon pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
	Pokemon pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
	Pokemon pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
	Pokemon pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);
	Pokemon pokemon7 = pokemonCreate ("Electrobuz",TYPE_ELECTRIC,50,50);
	Pokemon pokemon8 = pokemonCreate ("Tondebolt",TYPE_ELECTRIC,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);
	pokemonTrainerAddPokemon(trainer,pokemon3);
	pokemonTrainerAddPokemon(trainer,pokemon4);
	pokemonTrainerAddPokemon(trainer,pokemon5);
	pokemonTrainerAddPokemon(trainer,pokemon6);
	pokemonTrainerAddPokemon(trainer,pokemon7);
	pokemonTrainerAddPokemon(trainer,pokemon8);

	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerDepositPokemon(trainer,1);

	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);
	pokemonTrainerWithdrawPokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,9);

	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,8);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,7);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,6);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,5);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,4);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,3);

	pokemonTrainerWithdrawPokemon(trainer,1);
	pokemonTrainerWithdrawPokemon(trainer,1);

	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,2);
	pokemonTrainerRemovePokemon(trainer,1);
	testTrainerGetNumberOfPokemons(&result,trainer,1);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);
	pokemonDestroy(pokemon6);
	pokemonDestroy(pokemon7);
	pokemonDestroy(pokemon8);

    return result;
}
//=============================================================================
//					pokemonTrainerGetMostRankedPokemon
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerGetMostRankedPokemon() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	errorTrainerGetMostRankedPokemon(&result,NULL);
	testTrainerGetMostRankedPokemon(&result,trainer);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);

    return result;
}
//=============================================================================
//					pokemonTrainerMakeMostRankedParty
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerMakeMostRankedParty() {

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	errorTrainerMakeMostRankedParty(&result,NULL);
	testTrainerMakeMostRankedParty(&result,trainer);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);

    return result;
}
//=============================================================================
//					pokemonTrainerPrintEnumeration
//=============================================================================
// most of the tests where done as a "white-box" and were not submitted
static bool TrainerPrintEnumeration() {
#define CONST 5

	bool result = true;

	Pokemon pokemon = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pokemon,10,10);

	FILE* file = (void*)CONST;
	errorTrainerPrintEnumeration(&result,NULL,file);
	errorTrainerPrintEnumeration(&result,trainer,NULL);

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);

    return result;
}
//-----------------------------------------------------------------------------
int main() {

	 RUN_TEST(testCombo);
	 RUN_TEST(TrainerCreate);
	 RUN_TEST(TrainerDestroy);
	 RUN_TEST(TrainerCopy);
	 RUN_TEST(TrainerAddPokemon);
	 RUN_TEST(TrainerGetPokemon);
	 RUN_TEST(TrainerRemovPokemon);
	 RUN_TEST(TrainerDespositPokemon);
	 RUN_TEST(TrainerWithdrawPokemon);
	 RUN_TEST(TrainerGetNumberOfPokemons);
	 RUN_TEST(TrainerGetMostRankedPokemon)
	 RUN_TEST(TrainerMakeMostRankedParty);
	 RUN_TEST(TrainerPrintEnumeration);

	return 0;
}
#endif
