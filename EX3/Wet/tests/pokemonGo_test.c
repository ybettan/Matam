#include <string.h>
#include "../test_utilities.h"
#include "../pokemonGo.h"



static bool openFiles(FILE** pokedex_f, FILE** evolutions_f,
		FILE** locations_f){

	*pokedex_f = fopen("pokedex.txt", "r" );
	if(!*pokedex_f) return false;
	*evolutions_f = fopen("evolutions.txt", "r" );
	if(!*evolutions_f){
		fclose(*pokedex_f);
		return false;
	}
	*locations_f = fopen("locations.txt", "r" );
	if(!*locations_f){
		fclose(*evolutions_f);
		fclose(*pokedex_f);
		return false;
	}

	return true;
}
static void closeFiles(FILE** pokedex_f, FILE** evolutions_f,
		FILE** locations_f){

	fclose(*locations_f);
	fclose(*evolutions_f);
	fclose(*pokedex_f);
}

static bool testPokemonGoCreateDestroy(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));

    PokemonGo pg = pokemonGoCreate(NULL, stdout, stdout, stdout);
    ASSERT_TEST((pg) == (NULL));
    pg = pokemonGoCreate(stdout, NULL, stdout, stdout);
    ASSERT_TEST((pg) == (NULL));
    pg = pokemonGoCreate(stdout, stdout, NULL, stdout);
    ASSERT_TEST((pg) == (NULL));
    pg = pokemonGoCreate(stdout, stdout, stdout, NULL);
    ASSERT_TEST((pg) == (NULL));

    pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f, stdout);
    ASSERT_TEST(pg);

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}




static bool testPokemonGoAddTrainer(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
     	 stdout);
    ASSERT_TEST(pg);

    // NULL_ARGUMENT and(versus) INVALID_ARGUMENT.
    ASSERT_TEST(pokemonGoAddTrainer(NULL, "ash", 100, "paris") ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoAddTrainer(pg, NULL, 100, "paris") ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", -1, "paris") ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, NULL) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks LOCATION_DOES_NOT_EXIST b4 any trainers on map.
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "pariss") ==
    		POKEMONGO_LOCATION_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "") ==
    		POKEMONGO_LOCATION_DOES_NOT_EXIST);

    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "paris") ==
    		POKEMONGO_SUCCESS);

    // checks LOCATION_NOT_EXIST vs TRAINER_ALREADT_EXIST.
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "pariss") ==
    		POKEMONGO_TRAINER_NAME_ALREADY_EXISTS);

    // checks if adding trainer in a place erases species caught from location.
    ASSERT_TEST(pokemonGoAddTrainer(pg, "josef", 100, "sydney") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "mike", 100, "sydney") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "brook", 100, "sydney") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "sam", 100, "sydney") ==
    		POKEMONGO_SUCCESS);

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}




static bool testPokemonGoTrainerMoveAndHunt(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
    		stdout);
    ASSERT_TEST(pg);

    // NULL_ARGUMENT check.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(NULL, "ash", "berlin") ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, NULL, "berlin") ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", NULL) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks TRAINER_DOES_NOT_EXIST and(versus) LOCATION_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", "jerusalem") ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "berlin") ==
    		POKEMONGO_SUCCESS);      // starts with no pokemon and only place
                                     // to move is sydney.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", "jerusalem") ==
    		POKEMONGO_LOCATION_DOES_NOT_EXIST);

   // checks TRAINER_ALREADY_IN_LOCATION and(versus) LOCATION_IS_NOT_REACHABLE.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", "berlin") ==
    		POKEMONGO_TRAINER_ALREADY_IN_LOCATION);// berlin is not adjacent to
                                                   // itself.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", "madrid") ==
        		POKEMONGO_LOCATION_IS_NOT_REACHABLE);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "ash", "sydney") ==
    		POKEMONGO_SUCCESS);            // sydney is a trap.
                                           // the only adjacent city is itself.


    // lets make a tour in the world and catch some, ohh what fun!:
    // rome(pikachu) -> paris(pidgey) -> madrid(bulbasaur) -> paris(rattata)->
    // london(pidgey) -> paris(pikachu) -> berlin(NONE) -> sydney(pikachu).
    ASSERT_TEST(pokemonGoAddTrainer(pg, "myself", 0, "rome") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "paris") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "madrid") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "paris") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "london") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "paris") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "berlin") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "myself", "sydney") ==
    		POKEMONGO_SUCCESS);

    // last check: paris only pokemon left is bulbasaur.
    // checks if move and hunt remove pokemons caught from location.
    ASSERT_TEST(pokemonGoAddTrainer(pg, "brook", 0, "rome") ==
    		POKEMONGO_SUCCESS);//caught charmander.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		POKEMONGO_SUCCESS);//last bulbasaur caught.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "madrid") ==
    		POKEMONGO_SUCCESS);// went to the super-cllasico. barca won 4-0.
                               // caught pidgey
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		POKEMONGO_SUCCESS);//move was successful but no hunt this time.

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}



static bool testPokemonGoStoreAddItemsTrainerPurchaseMoveAndHunt(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
    		stdout);
    ASSERT_TEST(pg);

    // checks NULL_ARGUMENT. for storeAddItem.
    ASSERT_TEST(pokemonGoStoreAddItem(NULL, "potion", 20, 2) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, NULL, 20, 2) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks INVALID_ARGUMENR for storeAddItem.
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potionn", 20, 2) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "candyy", 20, 2) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", -1, 2) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 0, 2) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 20, -1) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 20, 0) ==
    		POKEMONGO_INVALID_ARGUMENT);

    //checks NULL_ARGUMENT vs INVALID_ARGUMENT vs TRAINER_NOT_EXIST
    //for trainerPurchase.
    ASSERT_TEST(pokemonGoTrainerPurchase(NULL, "ash", "potion", 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, NULL, "potion", 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", NULL, 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", -1) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 1) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);

    // checks TRAINER_NOT_EXIST and(versus) ITEM_OUT_OF_STOCK
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 1) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "brook", 0, "berlin") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 1) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 1) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);

    // lets put: 2 potions value=10, 1 potion value=1, 2 candies value=10.
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 10, 3) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 1, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "candy", 10, 2) ==
    		POKEMONGO_SUCCESS);

    // we added brook to the world before with no budget.
    // unfortunately he starts in berlin and no pokemon there.
    // his only pokeCoins are received by moving and hunting pokemons.

    // checks ITEM_OUT_OF_STOCK and(versus) BUDGET_IS_NOT_SUFFICIENT.
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 5) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 10) ==
    		POKEMONGO_BUDGET_IS_NOT_SUFFICIENT);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 5) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 10) ==
    		POKEMONGO_BUDGET_IS_NOT_SUFFICIENT);

    // lets make brook catch some.
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		POKEMONGO_SUCCESS);//caught pidgey -> pokeCoins=30
    for(int i=1; i<=3; i++){
    	  ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 10) ==
    			  POKEMONGO_SUCCESS);
    }                // has 3 potions valued 10. no more pokeCoins and no more
                     // potions of this value in store.
	  ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 10) ==
			  POKEMONGO_ITEM_OUT_OF_STOCK);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 1) ==
    		 POKEMONGO_BUDGET_IS_NOT_SUFFICIENT);
     ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "madrid")
    		 == POKEMONGO_SUCCESS);// caught bulbasaur -> has 30 pokeCoins.
     ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		 POKEMONGO_SUCCESS);   // caught rattata -> has 40 pokeCoins.
     //checks a candy worth twice the value
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 10) ==
    		 POKEMONGO_SUCCESS);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 10) ==
    		 POKEMONGO_SUCCESS);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 10) ==
    		 POKEMONGO_ITEM_OUT_OF_STOCK);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 1) ==
    		 POKEMONGO_BUDGET_IS_NOT_SUFFICIENT);

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}


static bool testPokemonGoBattleTrane(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
    		stdout);
    ASSERT_TEST(pg);

    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "rome") ==
    		POKEMONGO_SUCCESS);// ash:(xp=1, budget=100)
                               // caught pikachu(cp=30,hp=100,level=1).
    ASSERT_TEST(pokemonGoAddTrainer(pg, "brook", 100, "madrid") ==
    		POKEMONGO_SUCCESS);// brook:(xp=1, budget=100)
                               // caught bulbasaur(cp=10,hp=100,level=1).

    //checks NULL_ARGUMENT vs INVALID_ARGUMENT vs TRAINER_NOT_EXIST.
    ASSERT_TEST(pokemonGoBattle(NULL, "ash", "brook", 1, 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoBattle(pg, NULL, "brook", 1, 1) ==
    		POKEMONGO_NULL_ARGUMENT );
    ASSERT_TEST(pokemonGoBattle(pg, "ash", NULL, 1, 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "ash", 1, 1) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoBattle(pg, "yossi", "yossi", 1, 1) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoBattle(pg, "yossi", "brook", 1, 1) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "yossi", 1, 1) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);

	//checks TRAINER_DOES_NOT_EXIST and(versus) POKEMON_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoBattle(pg, "yossi", "brook", 2, 2) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "yossi", 2, 2) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "ash", 2, 2) ==
    		POKEMONGO_INVALID_ARGUMENT);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 2, 2) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1, 2) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 2, 1) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);

    //a reminder:
    // ash:(xp=1, budget=100)
    // caught pikachu(cp=30,hp=100,level=1).
    //
    // brook:(xp=1, budget=100)
    // caught bulbasaur(cp=10,hp=100,level=1).
    //
    // battle time!
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1, 1) ==
    		POKEMONGO_SUCCESS);
    //battle results:      ************  Pokemon Battle  *************
    //                   ash              		vs.		            brook
    //                   pikachu          		   		        bulbasaur
    //                   30               		CP 		               10
    //                   100.00 --> 85.00 		HP 		100.00 -->  55.00
    //                          --> 2     		LVL		     1 -->      2
    //                   1.00   --> 5.50  		XP 		  1.00 -->   2.50

    //well look like we should have evolution:
    //pikachu is level 2 -> raichu. raichu next evolution is baichu(3).
    //ash likes his new raichu so much he whould like to train him...
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "candy", 10, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "candy", 10) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 10) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK); //hoo..he was late :(

    // checks NULL_ARGUMENT of pokemonGoPokemonTrain.
    ASSERT_TEST(pokemonGoPokemonTrain(NULL, "ash", 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoPokemonTrain(pg, NULL, 1) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks NULL_ARGUMENT versus POKEMON_TRAINER_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonTrain(NULL, "yossi", 1) ==
    		POKEMONGO_NULL_ARGUMENT);

    // NULL_ARGUMENT versus POKEMONGO_POKEMON_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonTrain(NULL, "ash", 2) ==
    		POKEMONGO_NULL_ARGUMENT);

    // POKEMON_TRAINER_DOES_NOT_EXIST versus POKEMONGO_POKEMON_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonTrain(pg, "yossi", 2) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);

    // POKEMONGO_POKEMON_DOES_NOT_EXIST versus POKEMONGO_NO_ITEM_FOUND.
    ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", -1) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", 2) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", 1) ==
    		POKEMONGO_NO_AVAILABLE_ITEM_FOUND);

    // finally ash can train raichu:
    ASSERT_TEST(pokemonGoPokemonTrain(pg, "ash", 1) == POKEMONGO_SUCCESS);

    //brook tries again:
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1, 1) ==
    		POKEMONGO_SUCCESS);
    //ohh so many things happend:
    //ash: raichu level 3. -> baichu -> ranchu -> sanchu. next evol' panchu(4).
    //     sanchu is 90cp+10 candy cp.
    //brook: poor brook. he lost 2 fights, has no pokemons and no items.
    //       he decides to make a move. he had a dream that if he catches a
    //       pokemon with 20 cp, his level his good enough to kill opponent
    //       pokemon, no matter his hp, both will die cause no more
    //       then 100 hp!
    //the day after: store opens with brand new items.
    //
    //ash thought that brook might have a plan so he heal his sanchu.
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "candy", 50, 2) ==
    		POKEMONGO_SUCCESS);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "candy", 50) ==
    		POKEMONGO_SUCCESS);
     ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "candy", 50) ==
       		POKEMONGO_SUCCESS);
     ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", 1) ==
    		 POKEMONGO_POKEMON_DOES_NOT_EXIST); // died in battle.

     ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		 POKEMONGO_SUCCESS);
                           // caught pidgey with cp = 20. ash days are over!
                           // his id is 2.
     ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1 , 1) ==
    		 POKEMONGO_POKEMON_DOES_NOT_EXIST);
     ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1 , 2) ==
    		 POKEMONGO_SUCCESS);

     // both pokemons died. sanchu could evolve to panchu if wasn't dead.
     // check:
     ASSERT_TEST(pokemonGoPokemonTrain(pg, "ash", 1) ==
    		 POKEMONGO_POKEMON_DOES_NOT_EXIST);
     ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", 1) ==
    		 POKEMONGO_POKEMON_DOES_NOT_EXIST);
     ASSERT_TEST(pokemonGoPokemonTrain(pg, "brook", 2) ==
    		 POKEMONGO_POKEMON_DOES_NOT_EXIST);

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}

static bool testPokemonGoBattleHeal(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
    		stdout);
    ASSERT_TEST(pg);

    ASSERT_TEST(pokemonGoAddTrainer(pg, "ash", 100, "rome") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "brook", 100, "madrid") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "shlomi", 100, "sydney") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pg, "shir", 100, "london") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveAndHunt(pg, "brook", "paris") ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoBattle(pg, "ash", "brook", 1, 1) ==
    		POKEMONGO_SUCCESS);
    // after 2 battles ash pokemon with id 1 have hp =53.13
    // brook has pidgey (full hp).
    // pikachu evolved to raichu that evolved to sanchu passing some evolutions
    // on the way. one more battle and sanchu will become panchu! the strongest
    // specie on earth.


    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 46, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 47, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoStoreAddItem(pg, "potion", 100, 1) ==
    		POKEMONGO_SUCCESS);

    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 46) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 47) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 100) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 46) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "ash", "potion", 47) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoTrainerPurchase(pg, "brook", "potion", 100) ==
    		POKEMONGO_ITEM_OUT_OF_STOCK);

    //ash has 2 potions with values: 46,47.
    //brook has 1 potion with value: 100.
    //shlomi has no items.

    // checks NULL_ARGUMENT.
    ASSERT_TEST(pokemonGoPokemonHeal(NULL, "ash", 1) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoPokemonHeal(pg, NULL, 1) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks NULL_ARGUMENT versus TRAINER_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonHeal(NULL, "yossi", 1) ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks TRAINER_DOES_NOT_EXIST versus POKEMON_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "yossi", 1) ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);

    // checks POKEMON_DOES_NOT_EXIST.
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", -1) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", 0) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", 2) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);

    // checks POKEMONGO_HP_IS_AT_MAX.
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "brook", 2) ==
    		POKEMONGO_HP_IS_AT_MAX);

    // checks NO_AVAILABLE_ITEM_FOUND
    ASSERT_TEST(pokemonGoBattle(pg, "shlomi", "shir", 1, 1) ==
    		POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "shlomi", 1) ==
    		POKEMONGO_NO_AVAILABLE_ITEM_FOUND);


    // ash evolved sanchu has 53.13 and ash has potions valued 46,47.
    // he is not sure which potion to use. shell we help him?
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", 1) == POKEMONGO_SUCCESS);
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", 1) == POKEMONGO_HP_IS_AT_MAX);
    //as should be: the potion which brings pokemon to 100 hp->47.

    ASSERT_TEST(pokemonGoBattle(pg, "ash", "shir", 1, 1) == POKEMONGO_SUCCESS);
    //did sanchu evolved even more?
    ASSERT_TEST(pokemonGoPokemonHeal(pg, "ash", 1) ==
    		POKEMONGO_POKEMON_DOES_NOT_EXIST);

    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}



static bool testPokemonGoShowInfos(){

	FILE* pokedex_f;
	FILE* evolutions_f;
	FILE* locations_f;

	ASSERT_TEST(openFiles(&pokedex_f, &evolutions_f, &locations_f));
    PokemonGo pg = pokemonGoCreate(pokedex_f, evolutions_f, locations_f,
    		stdout);
    ASSERT_TEST(pg);

    // checks NULL_ARGUMENT for ShowStoreInfo.
    ASSERT_TEST(pokemonGoShowStoreInfo(NULL)  ==
    		POKEMONGO_NULL_ARGUMENT);

    // checks NULL_ARGUMENT for ShowMapInfo.
    ASSERT_TEST(pokemonGoShowMapInfo(NULL) ==
    		POKEMONGO_NULL_ARGUMENT);


    // checks NULL_ARGUMENT vs TRAINER_DOES_NOT_EXIST for ShowTrainerInfo.
    ASSERT_TEST(pokemonGoShowTrainerInfo(NULL, "ash") ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoShowTrainerInfo(pg, NULL) ==
    		POKEMONGO_NULL_ARGUMENT);
    ASSERT_TEST(pokemonGoShowTrainerInfo(pg, "ash") ==
    		POKEMONGO_TRAINER_DOES_NOT_EXIST);


    pokemonGoDestroy(pg);

	closeFiles(&pokedex_f, &evolutions_f, &locations_f);
	return true;
}


int main() {

	 RUN_TEST(testPokemonGoCreateDestroy);
	 RUN_TEST(testPokemonGoAddTrainer);
	 RUN_TEST(testPokemonGoTrainerMoveAndHunt);
	 RUN_TEST(testPokemonGoStoreAddItemsTrainerPurchaseMoveAndHunt);
	 RUN_TEST(testPokemonGoBattleTrane);
	 RUN_TEST(testPokemonGoBattleHeal);
	 RUN_TEST(testPokemonGoShowInfos);
/*
	 RUN_TEST(testPokemonGoShowInfo);
*/

	 return 0;
}


