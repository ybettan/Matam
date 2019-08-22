#include <string.h>
#include <assert.h>

#include "../test_utilities.h"
#include "../parcer.h"


#define MAX_COMMAND_LEN 256

#define OPEN_FILES		FILE* pokedex_f = fopen("pokedex.txt", "r" ); \
						FILE* locations_f = fopen("locations.txt","r");\
						FILE* evolutions_f = fopen("evolutions.txt","r")

#define CLOSE_FILES		fclose(locations_f);	\
						fclose(evolutions_f); 	\
						fclose(pokedex_f)

#define ARE_EQUALS(str,score)\
								strcpy(command,str);	\
								result = parcerRunAdtFunction(command,pg); \
								ASSERT_TEST(result == score)

#define SENDING_NULL_STRUCT(str,score)\
								strcpy(command,str);	\
								result = parcerRunAdtFunction(command,NULL); \
								ASSERT_TEST(result == score)

#define	BEGIN	\
			OPEN_FILES;\
			PokemonGo pg = \
				pokemonGoCreate(pokedex_f,evolutions_f, locations_f,stdout);\
			ParcerResult result;	\
			char command[MAX_COMMAND_LEN]

#define END			    pokemonGoDestroy(pg); \
						CLOSE_FILES; \
						return true

//-----------------------------------------------------------------------------
static bool testParcerNullArgument() {

	BEGIN;

    result = parcerRunAdtFunction(NULL,pg);
    ASSERT_TEST(result == PARCER_NULL_ARGUMENT);


    SENDING_NULL_STRUCT(("trainer add brook 300 london"),
    												(PARCER_NULL_ARGUMENT));

    SENDING_NULL_STRUCT(("trainer go brook london"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("store add potion 40 7"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("trainer purchase brook potion 40"),
    											(PARCER_NULL_ARGUMENT));

    SENDING_NULL_STRUCT(("battle fight ash brook squirtel bulbasaure "),
    											(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("pokemon heal ash 4"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("pokemon train ash 4"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("report trainer brook"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("report locations"),(PARCER_NULL_ARGUMENT));
    SENDING_NULL_STRUCT(("report stock"),(PARCER_NULL_ARGUMENT));


    END;
}
//-----------------------------------------------------------------------------
static bool testParcerTrainerAdd() {

	BEGIN;

    ARE_EQUALS(("trainer add brook -70 paris"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("trainer add brook 300 paris"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer add brook 150 london"),
    									(PARCER_TRAINER_NAME_ALREADY_EXISTS));
    ARE_EQUALS(("trainer add ash 10 nesher"),(PARCER_LOCATION_DOES_NOT_EXIST));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerTrainerGo() {

	BEGIN;

    ARE_EQUALS(("trainer go ash london"),(PARCER_TRAINER_DOES_NOT_EXIST));
    ARE_EQUALS(("trainer add ash 10 london"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer go ash nesher"),(PARCER_LOCATION_DOES_NOT_EXIST));
    ARE_EQUALS(("trainer go ash london"),(PARCER_TRAINER_ALREADY_IN_LOCATION));
    ARE_EQUALS(("trainer go ash madrid"),(PARCER_LOCATION_IS_NOT_REACHABLE));
    ARE_EQUALS(("trainer go ash paris"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerStoreAdd() {

	BEGIN;

    ARE_EQUALS(("store add lolipop 100 5"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion 0 5"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion 100 0"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion -5 5"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion 100 -10"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion 0 0"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion -100 -10"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("store add potion 100 10"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerTrainerPurchase() {

	BEGIN;

    ARE_EQUALS(("trainer purchase misty potion 30"),
    									(PARCER_TRAINER_DOES_NOT_EXIST));
    ARE_EQUALS(("trainer add misty 500 paris"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase misty lolipop 30"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("trainer purchase misty candy 0"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("trainer purchase misty candy -10"),(PARCER_INVALID_ARGUMENT));
    ARE_EQUALS(("trainer purchase misty candy 50"),(PARCER_ITEM_OUT_OF_STOCK));
    ARE_EQUALS(("store add potion 600 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase misty potion 600"),
    										(PARCER_BUDGET_IS_NOT_SUFFICIENT));
    ARE_EQUALS(("store add candy 300 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase misty candy 300"),
    									(PARCER_BUDGET_IS_NOT_SUFFICIENT));
    ARE_EQUALS(("store add potion 100 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 100 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase misty potion 100"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase misty candy 100"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerBattleFight() {

	BEGIN;

	ARE_EQUALS(("trainer add ash 300 london"),(PARCER_SUCCESS));
	ARE_EQUALS(("trainer add brook 1000 rome"),(PARCER_SUCCESS));
	ARE_EQUALS(("battle fight itzik brook 1 1"),(PARCER_TRAINER_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight ash brook -1 -1"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight ash brook -1 0"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight ash brook 0 -1"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight ash brook 0 0"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight ash brook 1 2"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("battle fight itzik itzik 1 1"),(PARCER_INVALID_ARGUMENT));
	ARE_EQUALS(("battle fight ash ash 1 1"),(PARCER_INVALID_ARGUMENT));
	ARE_EQUALS(("battle fight ash brook 1 1"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerPokemonHeal() {

	BEGIN;


	ARE_EQUALS(("pokemon heal ash 3"),(PARCER_TRAINER_DOES_NOT_EXIST));
	ARE_EQUALS((" trainer    add ash 300 rome"),(PARCER_SUCCESS));
	ARE_EQUALS(("pokemon heal ash 2"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon heal ash 0"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon heal ash -5"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon heal ash 1"),(PARCER_HP_IS_AT_MAX));
	ARE_EQUALS((" trainer    add brook 300 paris"),(PARCER_SUCCESS));
	ARE_EQUALS(("battle fight ash brook 1 1"),(PARCER_SUCCESS));
	ARE_EQUALS(("pokemon heal ash 1"),(PARCER_NO_AVAILABLE_ITEM_FOUND));
    ARE_EQUALS(("store add potion 100 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase ash potion 100"),(PARCER_SUCCESS));
	ARE_EQUALS(("pokemon heal ash 1"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerPokemonTrain() {

	BEGIN;

	ARE_EQUALS(("pokemon train gary 1"),(PARCER_TRAINER_DOES_NOT_EXIST));
	ARE_EQUALS((" trainer add gary 1000 paris"),(PARCER_SUCCESS));
	ARE_EQUALS(("pokemon train gary 2"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon train gary 0"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon train gary -100"),(PARCER_POKEMON_DOES_NOT_EXIST));
	ARE_EQUALS(("pokemon train gary 1"),(PARCER_NO_AVAILABLE_ITEM_FOUND));
    ARE_EQUALS(("store add candy 500 10"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary candy 500"),(PARCER_SUCCESS));
	ARE_EQUALS(("pokemon train gary 1"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerReportTrainer() {

	BEGIN;

	ARE_EQUALS(("report trainer haim"),(PARCER_TRAINER_DOES_NOT_EXIST));
	ARE_EQUALS((" trainer add gary 1000 paris"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 20 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 40 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 60 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 20 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 40 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 60 3"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary potion 20"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary potion 40"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary potion 60"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary candy 20"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary candy 40"),(PARCER_SUCCESS));
    ARE_EQUALS(("trainer purchase gary candy 60"),(PARCER_SUCCESS));
	ARE_EQUALS(("report trainer gary"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerReportLocations() {

	BEGIN;

	ARE_EQUALS(("report locations"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerReportStock() {

	BEGIN;

	ARE_EQUALS(("report stock"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 60 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 80 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 60 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 20 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 10 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 60 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add potion 20 1"),(PARCER_SUCCESS));
    ARE_EQUALS(("store add candy 10 1"),(PARCER_SUCCESS));
	ARE_EQUALS(("report stock"),(PARCER_SUCCESS));

    END;
}
//-----------------------------------------------------------------------------
static bool testParcerRunAdtFunction() {

	bool result_arr[11];

	result_arr[0] = testParcerNullArgument();
    result_arr[1] = testParcerTrainerAdd();
    result_arr[2] = testParcerTrainerGo();
    result_arr[3] = testParcerStoreAdd();
    result_arr[4] = testParcerTrainerPurchase();
    result_arr[5] = testParcerBattleFight();
    result_arr[6] = testParcerPokemonHeal();
    result_arr[7] = testParcerPokemonTrain();
    result_arr[8] = testParcerReportTrainer();
    result_arr[9] = testParcerReportLocations();
    result_arr[10] = testParcerReportStock();

    for (int i = 1 ; i < 11 ; i++) {
    	if (result_arr[i] == false)
    		return false;
    }

	return true;
}



//=============================================================================

int main() {

	 RUN_TEST(testParcerRunAdtFunction);

	 return 0;
}


