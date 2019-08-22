
#include <string.h>
#include "aux_macros.h"
#include "pokemon.h"


static bool test_pokemonCreate_Destroy_GetLevel(){
	bool result = true;

	Pokemon pikachu1 = pokemonCreate("", TYPE_NORMAL, 199, 4);
	TEST_EQUALS(result,pikachu1,NULL);
	Pokemon pikachu2 = pokemonCreate("Pikachu",TYPE_NORMAL, 199, 0);
	TEST_EQUALS(result,pikachu2,NULL);
	Pokemon pikachu3 = pokemonCreate("Pikachu", TYPE_NORMAL, 0, 4);
	TEST_EQUALS(result,pikachu3,NULL);
	Pokemon pikachu4 = pokemonCreate("h", TYPE_FIRE, 200, 4);
	TEST_DIFFERENT(result,pikachu4,NULL);
	Pokemon pikachu5= pokemonCreate("h", TYPE_WATER, 399, 4);
	TEST_DIFFERENT(result,pikachu5,NULL);
	Pokemon pikachu6 = pokemonCreate("Pikachu", TYPE_ELECTRIC, -1, 4);
	TEST_EQUALS(result,pikachu6,NULL);
	Pokemon pikachu7 = pokemonCreate("Pikachu", TYPE_ELECTRIC, 9902, 4);
	TEST_EQUALS(result,pikachu7,NULL);
	Pokemon pikachu8 = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result, pikachu8, NULL);
	Pokemon pikachu9 = pokemonCreate("Pikachu", TYPE_WATER, 9901, 4);
	TEST_DIFFERENT(result,pikachu9,NULL);
	Pokemon pikachu10 = pokemonCreate("Pikachu", -3, 20, 20);
	TEST_EQUALS(result,pikachu10,NULL);
	Pokemon fire_pokemon = pokemonCreate("h", TYPE_FIRE, 199, 4);
	TEST_DIFFERENT(result,fire_pokemon,NULL);

	TEST_EQUALS(result,pokemonGetLevel(pikachu4),2);
	TEST_EQUALS(result,pokemonGetLevel(pikachu5),4);
	TEST_EQUALS(result,pokemonGetLevel(pikachu8),1);
	TEST_EQUALS(result,pokemonGetLevel(pikachu9),100);
	TEST_EQUALS(result,pokemonGetLevel(fire_pokemon),2);

	pokemonDestroy(pikachu1);
	pokemonDestroy(pikachu2);
	pokemonDestroy(pikachu3);
	pokemonDestroy(pikachu4);
	pokemonDestroy(pikachu5);
	pokemonDestroy(pikachu6);
	pokemonDestroy(pikachu7);
	pokemonDestroy(pikachu8);
	pokemonDestroy(pikachu9);
	pokemonDestroy(pikachu10);
	pokemonDestroy(fire_pokemon);


	return result;
}

static bool test_pokemonTeachMove_UnteachMove_GetRank(){
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result,pikachu,NULL);
	TEST_EQUALS(result, pokemonGetRank(pikachu), 0);//have no moves--->rank=0.
	pokemonTeachMove(pikachu, "bzzz", TYPE_ELECTRIC, 50, 50);//have one move
	                                                         //out of 4
	TEST_DIFFERENT(result, pokemonGetRank(pikachu), 0);//now rank!=0.

	pokemonTeachMove(pikachu, "azzz", TYPE_ELECTRIC, 50, 50);//have 2 moves
                                                             //out of 4
	pokemonTeachMove(pikachu, "czzz", TYPE_ELECTRIC, 50, 70);//have 3 moves
                                                             //out of 4
	pokemonTeachMove(pikachu, "dzzz", TYPE_ELECTRIC, 50, 70);//have 4 moves
                                                             //out of 4
	pokemonTeachMove(pikachu, "aaaa", TYPE_ELECTRIC, 50, 50);
	TEST_EQUALS(result,pokemonUnteachMove(pikachu, "azzz"),//aaaa replace azzz
			POKEMON_MOVE_DOES_NOT_EXIST);

/**the next section tests invalid argument and the order of PokemonResult    */

	TEST_EQUALS(result,	pokemonTeachMove(NULL, "", -1, 0,0)
			,POKEMON_NULL_ARG);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, NULL,-1, 0, 0)
			,POKEMON_NULL_ARG);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "", -1, 0, 0)
			,POKEMON_INVALID_MOVE_NAME);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "bzzz", -1, 0, 0)
			,POKEMON_INVALID_TYPE);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "bzzz", TYPE_ELECTRIC, 0, 0)
			,POKEMON_INVALID_POWER_POINTS);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "bzzz", TYPE_ELECTRIC, 50, 0)
			,POKEMON_INVALID_STRENGTH);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "bzzz", TYPE_FIRE, 50, 50)
			,POKEMON_INCOMPATIBLE_MOVE_TYPE);
	TEST_EQUALS(result,	pokemonTeachMove(pikachu, "bzzz", TYPE_ELECTRIC, 50, 50)
			,POKEMON_MOVE_ALREADY_EXISTS);

//pikachu has "bzzz"-str=50 "aaaa"-str=50 "czzz"-str=70 "dzzz"-str=70
//pikachu exp=20--->level=1. avg_str=60--->rank=61

	TEST_EQUALS(result, pokemonGetRank(pikachu), 61);
	TEST_EQUALS(result, pokemonUnteachMove(NULL, "aaa"), POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, NULL), POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, ""), POKEMON_INVALID_MOVE_NAME);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "aaa"), POKEMON_MOVE_DOES_NOT_EXIST);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "aaaa"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "bzzz"), POKEMON_SUCCESS);

	//pikachu has"czzz"-str=70 "dzzz"-str=70
	//pikachu exp=20--->level=1. avg_str=70--->rank=71

	TEST_EQUALS(result, pokemonGetRank(pikachu), 71);

	pokemonDestroy(pikachu);
	return result;
}

static bool test_pokemonCopy(){
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result,pikachu,NULL);

	pokemonTeachMove(pikachu, "b", TYPE_ELECTRIC, 50, 50);//1 move out
	                                                      //of 4 possible
	pokemonTeachMove(pikachu, "a", TYPE_ELECTRIC, 50, 50);//2 moves out
                                                          //of 4 possible
	pokemonTeachMove(pikachu, "c", TYPE_ELECTRIC, 50, 50);//3 moves out
                                                          //of 4 possible
	pokemonTeachMove(pikachu, "d", TYPE_ELECTRIC, 50, 50);//4 moves out
                                                          //of 4 possible
	pokemonTeachMove(pikachu, "d", TYPE_ELECTRIC, 50, 50);
	TEST_EQUALS(result, pokemonTeachMove(pikachu, "d", TYPE_ELECTRIC, 50, 50)
			,POKEMON_MOVE_ALREADY_EXISTS);

	Pokemon failed_copy = pokemonCopy(NULL);
	TEST_EQUALS(result, failed_copy, NULL);

	/**the next section tests if pokemonCopy managed to copy all the fields
	 * required.                                                              */

	Pokemon pikachu_copy = pokemonCopy(pikachu);

	TEST_DIFFERENT(result, pikachu_copy, NULL);
	TEST_EQUALS(result, pokemonGetLevel(pikachu), pokemonGetLevel(pikachu_copy));
	TEST_EQUALS(result, pokemonGetRank(pikachu), pokemonGetRank(pikachu_copy));

	TEST_EQUALS(result, pokemonUnteachMove(pikachu,"aa"),
			POKEMON_MOVE_DOES_NOT_EXIST);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu,"aa"),
			pokemonUnteachMove(pikachu_copy,"aa"));

	TEST_EQUALS(result, pokemonUnteachMove(pikachu,"a"),
			pokemonUnteachMove(pikachu_copy,"a"));
	TEST_EQUALS(result, pokemonUnteachMove(pikachu,"a"),
			POKEMON_MOVE_DOES_NOT_EXIST);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu_copy,"a"),
			POKEMON_MOVE_DOES_NOT_EXIST);


	TEST_EQUALS(result, pokemonTeachMove(pikachu,"b", TYPE_NORMAL, 50, 50),
			POKEMON_MOVE_ALREADY_EXISTS);
	TEST_EQUALS(result, pokemonTeachMove(pikachu,"b", TYPE_NORMAL, 50, 50),
			pokemonTeachMove(pikachu_copy,"b", TYPE_NORMAL, 50, 50));
	//both already have a move called "b".

	pokemonDestroy(pikachu);
	pokemonDestroy(pikachu_copy);
	return result;
}

static bool test_pokemonUseMove_Heal(){
	bool result = true;

	Pokemon water_pokemon = pokemonCreate("fish", TYPE_WATER, 2011, 3);
	pokemonTeachMove(water_pokemon, "aqua_attack", TYPE_WATER, 50, 50);
	pokemonTeachMove(water_pokemon, "beaver_punch", TYPE_NORMAL, 100, 70);
	pokemonTeachMove(water_pokemon, "wave", TYPE_WATER, 4, 90);
	TEST_EQUALS(result,pokemonGetRank(water_pokemon),91);

	Pokemon fire_pokemon = pokemonCreate("h", TYPE_FIRE, 199, 4);
	TEST_DIFFERENT(result,fire_pokemon,NULL);
	pokemonTeachMove(fire_pokemon, "a", TYPE_FIRE, 50, 50);
	pokemonTeachMove(fire_pokemon, "b", TYPE_NORMAL, 50, 61);
	pokemonTeachMove(fire_pokemon, "c", TYPE_FIRE, 50, 72);
	TEST_EQUALS(result,pokemonGetRank(fire_pokemon),63);

	pokemonUseMove(water_pokemon,fire_pokemon, "wave");//-+264
	pokemonUseMove(water_pokemon,fire_pokemon, "wave");//+-272
	pokemonUseMove(water_pokemon,fire_pokemon, "wave");//+-284
	pokemonUseMove(water_pokemon,fire_pokemon, "wave");//+-200
	//now fire_pokemon has 0 hp water_pokemon has exp=3031

	TEST_EQUALS(result, pokemonUseMove(NULL, NULL, ""), POKEMON_NULL_ARG);
	TEST_EQUALS(result,	pokemonUseMove(water_pokemon,fire_pokemon, NULL)
			,POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonUseMove(water_pokemon, fire_pokemon,""),
			POKEMON_INVALID_MOVE_NAME);
	TEST_EQUALS(result, pokemonUseMove(water_pokemon, fire_pokemon,""),
				POKEMON_INVALID_MOVE_NAME);
	TEST_EQUALS(result,	pokemonUseMove(water_pokemon,fire_pokemon, "waves")
			,POKEMON_MOVE_DOES_NOT_EXIST);
	TEST_EQUALS(result,	pokemonUseMove(water_pokemon,fire_pokemon, "wave")
			,POKEMON_NO_POWER_POINTS);//power_points of wave is 0
	TEST_EQUALS(result,	pokemonUseMove(water_pokemon,fire_pokemon,
			"beaver_punch") ,POKEMON_NO_HEALTH_POINTS);
	TEST_EQUALS(result,	pokemonUseMove(fire_pokemon, water_pokemon,
				"a") ,POKEMON_NO_HEALTH_POINTS);

	pokemonHeal(fire_pokemon);
	TEST_EQUALS(result,	pokemonUseMove(fire_pokemon, water_pokemon,
				"a") ,POKEMON_SUCCESS);//now fire_pokemon has health-points?
	pokemonHeal(water_pokemon);
	TEST_EQUALS(result,	pokemonUseMove(water_pokemon,fire_pokemon, "wave")
			,POKEMON_SUCCESS);//now wave has power_points
	TEST_EQUALS(result, pokemonHeal(NULL), POKEMON_NULL_ARG);

	Pokemon grass_pokemon = pokemonCreate("weed", TYPE_GRASS, 9900, 2);
	pokemonTeachMove(grass_pokemon, "ganja_attack", TYPE_GRASS, 10, 500);
	pokemonUseMove(grass_pokemon, water_pokemon,"ganja_attack");
	//water_pokemon have 1310 hp, so will the level of
	//grass_pokemon be 113(9900+1310=11210)? should be 100!
	TEST_EQUALS(result, pokemonGetLevel(grass_pokemon), 100);
	TEST_EQUALS(result, pokemonHeal(NULL), POKEMON_NULL_ARG);

	pokemonDestroy(water_pokemon);
	pokemonDestroy(fire_pokemon);
	pokemonDestroy(grass_pokemon);

	return result;
}

static bool test_pokemonEvolve(){
	bool result = true;
	Pokemon water_pokemon = pokemonCreate("fish", TYPE_WATER, 2011, 3);
	TEST_DIFFERENT(result, water_pokemon, NULL);
	Pokemon experienced_pokemon = pokemonCreate("veteran", TYPE_NORMAL, 9901, 3);
	TEST_DIFFERENT(result, experienced_pokemon, NULL);
	TEST_EQUALS(result, pokemonGetLevel(water_pokemon),21);
	TEST_EQUALS(result, pokemonEvolve(NULL,""),POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonEvolve(experienced_pokemon,NULL)
			,POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonEvolve(experienced_pokemon,"")
			,POKEMON_INVALID_NAME);
	TEST_EQUALS(result, pokemonEvolve(experienced_pokemon,"get_bigger")
			,POKEMON_CANNOT_EVOLVE);
	TEST_EQUALS(result, pokemonGetLevel(water_pokemon),21);
	TEST_EQUALS(result, pokemonEvolve(water_pokemon, "fish+"),
			POKEMON_SUCCESS);
	TEST_DIFFERENT(result, pokemonGetLevel(water_pokemon),21);
	TEST_EQUALS(result, pokemonGetLevel(water_pokemon),22);
	TEST_EQUALS(result, pokemonEvolve(water_pokemon, "fish++"),
			POKEMON_SUCCESS);
	TEST_DIFFERENT(result, pokemonGetLevel(water_pokemon),22);
	TEST_EQUALS(result, pokemonGetLevel(water_pokemon),23);
	pokemonDestroy(water_pokemon);
	pokemonDestroy(experienced_pokemon);


	return result;
}

static bool test_leak(){
	bool result = true;
	Pokemon a = pokemonCreate("a", TYPE_NORMAL, 1, 8);
	Pokemon b = pokemonCreate("b", TYPE_NORMAL, 1, 8);
	pokemonTeachMove(a,"1",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"2",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"3",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"4",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"5",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"6",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"7",TYPE_NORMAL,1,1);
	pokemonTeachMove(a,"8",TYPE_NORMAL,1,1);

	pokemonTeachMove(b,"1",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"2",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"3",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"4",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"5",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"6",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"7",TYPE_NORMAL,1,1);
	pokemonTeachMove(b,"8",TYPE_NORMAL,1,1);

	Pokemon aa = pokemonCopy(a);
	Pokemon bb = pokemonCopy(b);

	pokemonDestroy(a);
	pokemonDestroy(b);
	pokemonDestroy(aa);
	pokemonDestroy(bb);
	return result;
}


 static bool testPokemonPrintName() {
#define CONST 5
    bool result = true;

    Pokemon pokemon = pokemonCreate("pikachu",TYPE_ELECTRIC,100,5);
    FILE* file = (void*)CONST;

    PokemonResult score = pokemonPrintName(NULL,file);
    TEST_EQUALS(result,score,POKEMON_NULL_ARG );

    score = pokemonPrintName(pokemon,NULL);
    TEST_EQUALS(result,score,POKEMON_NULL_ARG );

    pokemonDestroy(pokemon);

    return result;
}

static bool testPokemonPrintVoice() {
    bool result = true;

    Pokemon pokemon = pokemonCreate("pikachu",TYPE_ELECTRIC,100,5);

    PokemonResult score = pokemonPrintVoice(NULL,NULL);
    TEST_EQUALS(result,score,POKEMON_NULL_ARG );

    score = pokemonPrintVoice(pokemon,NULL);
    TEST_EQUALS(result,score,POKEMON_NULL_ARG );

    //malloc failure simulation has been check with "White-Box"

    pokemonDestroy(pokemon);

    return result;
}


static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result, pikachu, NULL);
	Pokemon pikachu_copy = pokemonCopy(pikachu);
	TEST_DIFFERENT(result, pikachu_copy, NULL);
	pokemonDestroy(pikachu_copy);
	TEST_EQUALS(result, pokemonTeachMove(
		pikachu, "Thunder", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Thunder"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(pikachu), 1);
	TEST_EQUALS(result, pokemonGetRank(pikachu), 0);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 30, 40);
	TEST_EQUALS(result, pokemonUseMove(
		squirtle, pikachu, "Bubble"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonHeal(pikachu), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonEvolve(squirtle, "Wartortle"), POKEMON_SUCCESS);
	pokemonDestroy(pikachu);
	pokemonDestroy(squirtle);
	return result;
}

int main() {

	RUN_TEST(test_pokemonCreate_Destroy_GetLevel);
	RUN_TEST(test_pokemonTeachMove_UnteachMove_GetRank);
	RUN_TEST(test_pokemonCopy);
	RUN_TEST(test_pokemonUseMove_Heal);
	RUN_TEST(test_pokemonEvolve);
	RUN_TEST(test_leak);
	RUN_TEST(testPokemonPrintName);
    RUN_TEST(testPokemonPrintVoice);
	RUN_TEST(testCombo);

	return 0;
}
