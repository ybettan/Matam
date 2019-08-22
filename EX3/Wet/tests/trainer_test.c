
#include <string.h>
#include "../test_utilities.h"
#include "../trainer.h"

static bool testTrainerCreateDestroy(){

	TrainerResult result;
	Trainer trainer_try = trainerCreate("ash", "haifa", 10, &result);
	ASSERT_TEST(trainer_try != NULL);
	trainerDestroy(trainer_try);

	ASSERT_TEST(trainerCreate(NULL, "haifa", 10, &result) == NULL);
	ASSERT_TEST(result == TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerCreate("ash", NULL, 10, &result) == NULL);
	ASSERT_TEST(result == TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerCreate("ash", "haifa", -1, &result) == NULL);
	ASSERT_TEST(result == TRAINER_INVALID_ARGUMENT);
	Trainer trainer = trainerCreate("ash", "haifa", 0, &result);
	ASSERT_TEST(trainer != NULL);
	trainerDestroy(trainer);

	return true;
}

static bool testTrinerGetLocationMove(){

	TrainerResult result;
	Trainer trainer= trainerCreate("ash", "haifa", 10, &result);
	ASSERT_TEST(trainer != NULL);
	const char* trainer_location = trainerGetLocation(trainer);
	ASSERT_TEST(trainer_location != NULL);
	ASSERT_TEST(strcmp(trainer_location, "haifa") == 0);

	ASSERT_TEST(trainerGetLocation(NULL) == NULL);

	ASSERT_TEST(trainerMove(trainer, "akko") == TRAINER_SUCCESS);
	trainer_location = trainerGetLocation(trainer);
	ASSERT_TEST(strcmp(trainer_location, "haifa") != 0);
	ASSERT_TEST(strcmp(trainer_location, "akko") == 0);



	ASSERT_TEST(trainerMove(NULL, "akko") == TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerMove(trainer, NULL) == TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerMove(trainer, "akko") ==
			TRAINER_TRAINER_ALREADY_IN_LOCATION);

	ASSERT_TEST(trainerMove(trainer, "haifa") == TRAINER_SUCCESS);
	trainer_location = trainerGetLocation(trainer);
	ASSERT_TEST(strcmp(trainer_location, "haifa") == 0);

	trainerDestroy(trainer);
	return true;
}


static bool testTrainerHuntGetSetParameters(){

	TrainerResult result;
	Trainer trainer = trainerCreate("ash", "technion", 10, &result);
	ASSERT_TEST(trainer != NULL);
	ASSERT_TEST(result == TRAINER_SUCCESS);

	double trainers_xp;
	char* pokemon_species;
	int pokemon_cp;
	double pokemon_hp;
	int pokemon_level;

	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerHunt(trainer, "pikachu", 1, 20) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(trainers_xp == 1);
	ASSERT_TEST(strcmp(pokemon_species, "pikachu") == 0);
	ASSERT_TEST(pokemon_cp == 1);
	ASSERT_TEST(pokemon_hp == 100);
	ASSERT_TEST(pokemon_level == 1);//has 1 pokemon.

	ASSERT_TEST(trainerHunt(NULL, "charizard", 20, 20) ==
			TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerHunt(trainer, NULL, 20, 20) ==
			TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerHunt(trainer, "charizard", -1, 20) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerHunt(trainer, "charizard", 0, 20) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerHunt(trainer, "charizard", 20, -1) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerHunt(trainer, "charizard", 20, 0) ==
			TRAINER_INVALID_ARGUMENT);


	ASSERT_TEST(trainerGetParameters(NULL, &trainers_xp, 1,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer, NULL, 1,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer,  &trainers_xp, 1,
				NULL, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer,  &trainers_xp, 1,
				&pokemon_species,NULL, &pokemon_hp, &pokemon_level)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer,  &trainers_xp, 1,
				&pokemon_species, &pokemon_cp,NULL, &pokemon_level)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
				&pokemon_species, &pokemon_cp, &pokemon_hp, NULL)	==
						TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, -1,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp , 0,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_POKEMON_DOES_NOT_EXIST);


	ASSERT_TEST(trainerHunt(trainer, "charizard", 20, 20) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_SUCCESS);

	ASSERT_TEST(trainers_xp == 1);
	ASSERT_TEST(strcmp(pokemon_species, "charizard") == 0);
	ASSERT_TEST(pokemon_cp == 20);
	ASSERT_TEST(pokemon_hp == 100);
	ASSERT_TEST(pokemon_level == 1);//has 2 pokemon.

	ASSERT_TEST(trainerSetParameters(trainer, 30, 2, 80, 2) ==
			TRAINER_SUCCESS);                            //set parameters
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
				&pokemon_species, &pokemon_cp, &pokemon_hp, &pokemon_level)	==
						TRAINER_SUCCESS);

	ASSERT_TEST(trainers_xp == 30);
	ASSERT_TEST(strcmp(pokemon_species, "charizard") == 0);
	ASSERT_TEST(pokemon_cp == 20);
	ASSERT_TEST(pokemon_hp == 80);
	ASSERT_TEST(pokemon_level == 2);

	ASSERT_TEST(trainerSetParameters(trainer , 30, 2, 80, 2) ==//as b4
			TRAINER_SUCCESS);

	ASSERT_TEST(trainerSetParameters(NULL, 30, 2, 80, 2) == //trainer
			TRAINER_NULL_ARGUMENT);

	ASSERT_TEST(trainerSetParameters(trainer, -1, 2, 80, 2) == //xp
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer, -0.1, 2, 80, 2) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer, 0.1, 2, 80, 2) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer, 0.99, 2, 80, 2) ==
			TRAINER_INVALID_ARGUMENT);

	ASSERT_TEST(trainerSetParameters(trainer ,30, -1, 80, 2) == //index
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerSetParameters(trainer ,30, 0, 80, 2) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerSetParameters(trainer ,30, 3, 80, 2) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);//has 2 pokemons.

	ASSERT_TEST(trainerSetParameters(trainer,30, 2, -1, 2) == //hp
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer,30, 2, -0.1, 2) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer,30, 2, 101, 2) ==
			TRAINER_INVALID_ARGUMENT);

	ASSERT_TEST(trainerSetParameters(trainer,30, 2, 80, -1) == //level
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerSetParameters(trainer,30, 2, 80, 0) ==
			TRAINER_INVALID_ARGUMENT);

	trainerDestroy(trainer);

	return true;
}



static bool testTrainerHuntPokeCoinsGainAddItemHeal(){

	TrainerResult result;
	double trainers_xp;
	char* pokemon_species;
	int pokemon_cp;
	double pokemon_hp;
	int pokemon_level;


	Trainer trainer = trainerCreate("ash", "the_moon", 10, &result);
	ASSERT_TEST(trainer != NULL);
	ASSERT_TEST(result == TRAINER_SUCCESS);
	ASSERT_TEST(trainerHunt(trainer, "pikachu", 1, 20) == TRAINER_SUCCESS);
	                                              //should have 30 pokeCoins.

	ASSERT_TEST(trainerAddItem(NULL, "potion", 30) ==
			TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerAddItem(trainer, "potion ", 30) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerAddItem(trainer, "candy ", 30) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerAddItem(trainer, "", 30) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerAddItem(trainer, "potion", -1) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerAddItem(trainer, "potion", 0) ==
			TRAINER_INVALID_ARGUMENT);


	ASSERT_TEST(trainerAddItem(trainer, "potion", 31) ==//has 30 pokeCoins
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);
	ASSERT_TEST(trainerAddItem(trainer, "potion", 30) ==
			TRAINER_SUCCESS); //has 0 pokeCoins and 1 potion: value = 30/
	                          //1 pokemon: "pikachu" id =1
	ASSERT_TEST(trainerAddItem(trainer, "potion", 1) ==
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);


	ASSERT_TEST(trainerHunt(trainer, "charizard", 1, 50) == TRAINER_SUCCESS);
	                                              //should have 50 pokeCoins.
	ASSERT_TEST(trainerAddItem(trainer, "potion", 51) ==
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);
	ASSERT_TEST(trainerAddItem(trainer, "potion", 50) ==
			TRAINER_SUCCESS); //has 0 pokeCoins and 2 potion: value = 30
	                          //                              value = 50
	                          //2 pokemons: "pikachu" id =1
	                          //            "charzizard" id =2
	ASSERT_TEST(trainerAddItem(trainer, "potion", 1) ==
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);


	ASSERT_TEST(trainerHunt(trainer, "mew", 1, 100) == TRAINER_SUCCESS);
	                                              //should have 100 pokeCoins.
	ASSERT_TEST(trainerAddItem(trainer, "candy", 51) ==
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);
	ASSERT_TEST(trainerAddItem(trainer, "candy", 30) ==
			TRAINER_SUCCESS); //has 40 pokeCoins and   1 candy: value 30
	                          //                      2 potion: value = 30
	                          //                                value = 50
	                          //3 pokemons: "pikachu" id =1
	                          //            "charzizard" id =2
	                          //            "mew" id =3


	ASSERT_TEST(trainerSetParameters(trainer, 30, 2, 40, 2)== TRAINER_SUCCESS);
	                                               //charizard hp =40
	ASSERT_TEST(trainerPokemonHeal(trainer, 2) == TRAINER_SUCCESS);
	                                             //should give +50 hp =90 total


	                        //trainer has 40 pokeCoins and 1 candy: value 30
		                    //                             1 potion: value 30
		                    //
		                    //3 pokemons: "pikachu" id =1
		                    //            "charzizard" id =2
		                    //            "mew" id =3
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(pokemon_hp == 90);
	ASSERT_TEST(pokemon_level ==2);
	ASSERT_TEST(trainerSetParameters(trainer, 30, 2, 80, 2)== TRAINER_SUCCESS);
	                                               //charizard hp =80
	ASSERT_TEST(trainerPokemonHeal(trainer, 2) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(pokemon_hp == 100);
                             //trainer has 40 pokeCoins and 1 candy: value 30
                             //
                             //3 pokemons: "pikachu" id =1
                             //            "charzizard" id =2
                             //            "mew" id =3
	ASSERT_TEST(trainerSetParameters(trainer, 30, 2, 1, 2)== TRAINER_SUCCESS);
	                                               //charizard hp =1
	ASSERT_TEST(trainerPokemonHeal(trainer, 2) ==
			TRAINER_NO_AVAILABLE_ITEM_FOUND);

	//trainer has 40 pokeCoins
	//pikachu hp = 100 id=1
	//charizard hp =1 id=2
	//mew hp =1 id=3
	//no potions
	ASSERT_TEST(trainerAddItem(trainer, "potion", 40) ==
			TRAINER_SUCCESS); // now has 1 potion value=30

	ASSERT_TEST(trainerPokemonHeal(NULL, 2) == TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerPokemonHeal(trainer, -1) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerPokemonHeal(trainer, 0) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerPokemonHeal(trainer, 4) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerPokemonHeal(trainer, 1) == TRAINER_HP_IS_AT_MAX);
	ASSERT_TEST(trainerPokemonHeal(trainer, 2) == TRAINER_SUCCESS);
	                                    //charizard hp = 41
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 2,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(pokemon_hp == 41);
	ASSERT_TEST(trainerHunt(trainer, "psy-duck", 10, 1000) == TRAINER_SUCCESS);

	for(int i=1; i<=50; i++ ){
		ASSERT_TEST(trainerAddItem(trainer, "candy", 10) == TRAINER_SUCCESS);
	}                                               //has 0 pokeCoins
	                                                // has 1 candy value 30
	                                                //      50 candies value 10
	ASSERT_TEST(trainerAddItem(trainer, "potion", 1) ==
			TRAINER_BUDGET_IS_NOT_SUFFICIENT);
	for(int i=1; i<=50; i++ ){
		ASSERT_TEST(trainerPokemonTrain(trainer, 4) == TRAINER_SUCCESS);
	}//total 30+94*10 candy_cp + 10 initial = 530

	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 4,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(strcmp(pokemon_species, "psy-duck") == 0);
	ASSERT_TEST(pokemon_cp == 530);


	trainerDestroy(trainer);

	return true;
}


static bool testTrainerPokemonTrainEvolveOrKill(){

	TrainerResult result;
	double trainers_xp;
	char* pokemon_species;
	int pokemon_cp;
	double pokemon_hp;
	int pokemon_level;


	Trainer trainer = trainerCreate("ash", "the roof of amado", 200, &result);
	ASSERT_TEST(trainer != NULL);
	ASSERT_TEST(result == TRAINER_SUCCESS); // ash has 200 pokeCoins

	ASSERT_TEST(trainerHunt(trainer, "pikachu", 10, 100) == TRAINER_SUCCESS);
	                           //ash has 300 pokeCoins and 1 pokemon:
	                           //pikachu-hp=100,cp=10,candy_cp=0, level=1
	ASSERT_TEST(trainerAddItem(trainer, "candy", 50));//200 pokeCoins left.
	ASSERT_TEST(trainerAddItem(trainer, "candy", 40));//120 pokeCoins left.

	                           //ash has 2 candies: values:40,50/
	ASSERT_TEST(trainerPokemonTrain(trainer, 1) == TRAINER_SUCCESS);
	                           //pikachu cp=60 ,candy_cp=50
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(pokemon_cp == 60);
	ASSERT_TEST(trainerPokemonTrain(trainer, 1) == TRAINER_SUCCESS);
	                           //pikachu cp=100 ,candy_cp=90
	ASSERT_TEST(trainerPokemonTrain(trainer, 1) ==
			TRAINER_NO_AVAILABLE_ITEM_FOUND);
	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(pokemon_cp == 100);
	ASSERT_TEST(pokemon_level == 1);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 1, "pikachu+", 10) ==
			TRAINER_SUCCESS);//should have 10cp+90candy_cp=100
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 2, "pikachu++", 10) ==
				TRAINER_SUCCESS);

	ASSERT_TEST(trainerGetParameters(trainer, &trainers_xp, 1,
			&pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(strcmp(pokemon_species, "pikachu+") == 0);
	ASSERT_TEST(pokemon_cp == 100);
	//120 pokeCoins left.
	//no items

	ASSERT_TEST(trainerAddItem(trainer, "candy", 10));
	//100 pokeCoins
	//1 candy: value=10


	ASSERT_TEST(trainerPokemonTrain(NULL, 1) ==
			TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerPokemonTrain(trainer,2)==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerPokemonTrain(trainer,0)==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerPokemonTrain(trainer,1)==
			TRAINER_SUCCESS);
	ASSERT_TEST(trainerPokemonTrain(trainer,1)==
			TRAINER_NO_AVAILABLE_ITEM_FOUND);
	//pikachu+ has 110 cp and 100candy_cp
	ASSERT_TEST(trainerEvolvePokemon(NULL, 1, 1, "pikachu++", 10) ==
			TRAINER_NULL_ARGUMENT);

	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, -1, "pikachu++", 10) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 0, "pikachu++", 10) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 1, NULL, 10) ==
			TRAINER_NULL_ARGUMENT);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 1, "pikachu++", -1) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 1, 1, "pikachu++", 0) ==
			TRAINER_INVALID_ARGUMENT);
	ASSERT_TEST(trainerEvolvePokemon(trainer, -1, 1, "pikachu++", 10) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 0, 1, "pikachu++", 10) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);
	ASSERT_TEST(trainerEvolvePokemon(trainer, 2, 1, "pikachu++", 10) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);

	ASSERT_TEST(trainerSetParameters(trainer, 20, 1, 0, 1) ==
			TRAINER_SUCCESS);//now pokemon is dead
	ASSERT_TEST(trainerRemovePokemonIfDead(trainer, 1) ==	TRAINER_SUCCESS);
	ASSERT_TEST(trainerRemovePokemonIfDead(trainer, 1 ) ==
			TRAINER_POKEMON_DOES_NOT_EXIST);//been removed

	trainerDestroy(trainer);

	return true;
}


static bool testTrainerCopy(){


	TrainerResult result;
	double trainers_xp;
	char* pokemon_species;
	int pokemon_cp;
	double pokemon_hp;
	int pokemon_level;

	Trainer trainer = trainerCreate("ash", "in_the_ground", 200, &result);
	ASSERT_TEST(trainer != NULL);
	ASSERT_TEST(result == TRAINER_SUCCESS);

	ASSERT_TEST(trainerHunt(trainer, "pikachu", 1, 10) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerHunt(trainer, "charizard", 1, 10) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerHunt(trainer, "mew", 1, 10) == TRAINER_SUCCESS);

	ASSERT_TEST(trainerAddItem(trainer, "potion",10) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerAddItem(trainer, "potion",10) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerAddItem(trainer, "potion",10) == TRAINER_SUCCESS);

	ASSERT_TEST(trainerAddItem(trainer, "candy",10) == TRAINER_SUCCESS);
	ASSERT_TEST(trainerAddItem(trainer, "candy",10) == TRAINER_SUCCESS);

				//original trainer have:
	            //                      3 pokemons: id=1 "pikachu"
	            //                                  id=2 "charizard"
	            //                                  id=3 "mew"
	            //                      3 potions of value 10
	            //                      2 candies value 10

	Trainer trainer_copy = trainerCopy(trainer);
	ASSERT_TEST(trainer_copy != NULL);
	ASSERT_TEST(result == TRAINER_SUCCESS);

	ASSERT_TEST(trainerGetParameters(trainer_copy,
			&trainers_xp, 1, &pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(strcmp(pokemon_species, "pikachu") == 0);

	ASSERT_TEST(trainerGetParameters(trainer_copy,
			&trainers_xp, 2, &pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(strcmp(pokemon_species, "charizard") == 0);

	ASSERT_TEST(trainerGetParameters(trainer_copy,
			&trainers_xp, 3, &pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_SUCCESS);
	ASSERT_TEST(strcmp(pokemon_species, "mew") == 0);

	ASSERT_TEST(trainerGetParameters(trainer_copy,
			&trainers_xp, 4, &pokemon_species, &pokemon_cp, &pokemon_hp,
			&pokemon_level) == TRAINER_POKEMON_DOES_NOT_EXIST);

	ASSERT_TEST(trainerSetParameters(trainer_copy, 1, 1, 10, 1) ==
			TRAINER_SUCCESS);
	//now try to heal. 3 times should work and 4th time fail
	for(int i=1; i<=3; i++){
		ASSERT_TEST(trainerPokemonHeal(trainer_copy, 1) == TRAINER_SUCCESS);

	}
	ASSERT_TEST(trainerPokemonHeal(trainer_copy, 1) ==
			TRAINER_NO_AVAILABLE_ITEM_FOUND);
	trainerDestroy(trainer);
	trainerDestroy(trainer_copy);

	return true;
}


int main(){
	RUN_TEST(testTrainerCreateDestroy);
	RUN_TEST(testTrinerGetLocationMove);
	RUN_TEST(testTrainerHuntGetSetParameters);
	RUN_TEST(testTrainerHuntPokeCoinsGainAddItemHeal);
	RUN_TEST(testTrainerPokemonTrainEvolveOrKill);

	RUN_TEST(testTrainerCopy);

	return 0;
}
