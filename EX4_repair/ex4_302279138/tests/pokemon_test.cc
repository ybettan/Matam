#include "test_utils.h"
#include "pokemon.h"

using std::string;
using std::cout;
using std::endl;
using mtm::pokemongo::Pokemon;
using std::set;
using namespace mtm::pokemongo;



static bool cTorTest() {

	set<PokemonType> pokemon_types = Pokemon::GetDefaultTypes("charmander");

	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("",pokemon_types,10,10));
	ASSERT_THROW(PokemonInvalidArgsException,
								Pokemon("charmander",pokemon_types,0,10));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",-3,10));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",10,0));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",10,-5));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("",10,10));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",0,10));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",-17,10));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",10,0));
	ASSERT_THROW(PokemonInvalidArgsException,Pokemon("charmander",10,-1));


	return true;
}


static bool operatorEqualTest() {

	Pokemon pokemon1("charmander",30.4,10);
	Pokemon pokemon2(pokemon1);
	Pokemon pokemon3("bulbasaur",25,10);
	Pokemon pokemon4("bulbasaur",10,25);
	Pokemon pokemon5("charmander",10,25);
	Pokemon pokemon6("bulbasaur",10,20);
	Pokemon pokemon7("bulbasaur",15,25);

	ASSERT_TRUE(pokemon1 == pokemon1);
	ASSERT_TRUE(pokemon1 == pokemon2);
	ASSERT_TRUE(pokemon3 == pokemon4);
	ASSERT_FALSE(pokemon4 == pokemon5);
	ASSERT_FALSE(pokemon4 == pokemon6);
	ASSERT_FALSE(pokemon4 == pokemon7);

	return true;
}

static bool operatorDiffTest() {

	Pokemon pokemon1("charmander",30.4,10);
	Pokemon pokemon2(pokemon1);
	Pokemon pokemon3("bulbasaur",25,10);
	Pokemon pokemon4("bulbasaur",10,25);
	Pokemon pokemon5("charmander",10,25);
	Pokemon pokemon6("bulbasaur",10,20);
	Pokemon pokemon7("bulbasaur",15,25);

	ASSERT_FALSE(pokemon1 != pokemon2);
	ASSERT_FALSE(pokemon3 != pokemon4);
	ASSERT_TRUE(pokemon4 != pokemon5);
	ASSERT_TRUE(pokemon4 != pokemon6);
	ASSERT_TRUE(pokemon4 != pokemon7);

	return true;
}

static bool operatorSmallerTest() {

	Pokemon pokemon1("charmander",10,100);
	Pokemon pokemon2("charmander",10,100);
	Pokemon pokemon3("charmander",9,100);
	Pokemon pokemon4("charmander",10,99);
	Pokemon pokemon5("bulbasaur",10,100);
	Pokemon pokemon6("bulbasaur",100,100);
	Pokemon pokemon7("bulbasaur",11,100);

	ASSERT_FALSE(pokemon1 < pokemon2);
	ASSERT_TRUE(pokemon3 < pokemon1);
	ASSERT_TRUE(pokemon4 < pokemon1);
	ASSERT_TRUE(pokemon5 < pokemon1);
	ASSERT_FALSE(pokemon6 < pokemon1);
	ASSERT_FALSE(pokemon7 < pokemon1);

	return true;
}

static bool operatorGratherTest() {

	Pokemon pokemon1("charmander",10,100);
	Pokemon pokemon2("charmander",10,100);
	Pokemon pokemon3("charmander",9,100);
	Pokemon pokemon4("charmander",10,99);
	Pokemon pokemon5("bulbasaur",10,100);
	Pokemon pokemon6("bulbasaur",100,100);
	Pokemon pokemon7("bulbasaur",11,100);

	ASSERT_FALSE(pokemon1 > pokemon2);
	ASSERT_FALSE(pokemon3 > pokemon1);
	ASSERT_FALSE(pokemon4 > pokemon1);
	ASSERT_FALSE(pokemon5 > pokemon1);
	ASSERT_TRUE(pokemon6 > pokemon1);
	ASSERT_TRUE(pokemon7 > pokemon1);

	return true;
}

static bool operatorSmallerOrEqualTest() {

	Pokemon pokemon1("charmander",10,100);
	Pokemon pokemon2("charmander",10,100);
	Pokemon pokemon3("charmander",9,100);
	Pokemon pokemon4("charmander",10,99);
	Pokemon pokemon5("bulbasaur",10,100);
	Pokemon pokemon6("bulbasaur",100,100);
	Pokemon pokemon7("bulbasaur",11,100);

	ASSERT_TRUE(pokemon1 <= pokemon2);
	ASSERT_TRUE(pokemon3 <= pokemon1);
	ASSERT_TRUE(pokemon4 <= pokemon1);
	ASSERT_TRUE(pokemon5 <= pokemon1);
	ASSERT_FALSE(pokemon6 <= pokemon1);
	ASSERT_FALSE(pokemon7 <= pokemon1);

	return true;
}


static bool operatorGratherOrEqualTest() {

	Pokemon pokemon1("charmander",10,100);
	Pokemon pokemon2("charmander",10,100);
	Pokemon pokemon3("charmander",9,100);
	Pokemon pokemon4("charmander",10,99);
	Pokemon pokemon5("bulbasaur",10,100);
	Pokemon pokemon6("bulbasaur",100,100);
	Pokemon pokemon7("bulbasaur",11,100);

	ASSERT_TRUE(pokemon1 >= pokemon2);
	ASSERT_FALSE(pokemon3 >= pokemon1);
	ASSERT_FALSE(pokemon4 >= pokemon1);
	ASSERT_FALSE(pokemon5 >= pokemon1);
	ASSERT_TRUE(pokemon6 >= pokemon1);
	ASSERT_TRUE(pokemon7 >= pokemon1);

	return true;
}

static bool operatorOutputTest() {

	set<PokemonType> types_set1;
	types_set1.insert(FIRE);
	types_set1.insert(BUG);
	types_set1.insert(GHOST);
	types_set1.insert(ROCK);
	types_set1.insert(NORMAL);
	types_set1.insert(ELECTRIC);
	Pokemon pokemon1("charmander",types_set1,30.5,10);
	std::ostringstream out_str1;
	out_str1 << pokemon1;
	string str1 = out_str1.str();
	ASSERT_TRUE(str1 ==
			"charmander(10/30.5/100) NORMAL ROCK BUG GHOST ELECTRIC FIRE");

	set<PokemonType> types_set2;
	Pokemon pokemon2("bulbasaur",types_set2,77.7,13);
	std::ostringstream out_str2;
	out_str2 << pokemon2;
	string str2 = out_str2.str();
	ASSERT_TRUE(str2 == "bulbasaur(13/77.7/100)");
	return true;
}

static bool lvlTest() {

	Pokemon pokemon1("charmander",30.5,10);
	ASSERT_TRUE(pokemon1.Level() == 10);
	Pokemon pokemon2("charmander",30.5,6);
	ASSERT_TRUE(pokemon2.Level() == 6);
	Pokemon pokemon3("charmander",30.5,350);
	ASSERT_FALSE(pokemon3.Level() == 355);

	return true;
}


static bool hitTest() {

	Pokemon attacker("charmander",20,2);
	Pokemon victim("bulbasaur",1,1);

	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_TRUE( attacker.Hit(victim) );

	return true;
}

static bool healTest() {

	Pokemon attacker("charmander",20,2);
	Pokemon victim("bulbasaur",1,1);


	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_TRUE( attacker.Hit(victim) );
	victim.Heal();
	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_FALSE( attacker.Hit(victim) );
	ASSERT_TRUE( attacker.Hit(victim) );

	return true;
}

static bool trainTest() {

	Pokemon attacker("charmander",20,2);
	Pokemon victim("bulbasaur",1,1);
//	bool exception_cought = false;

	ASSERT_THROW(PokemonInvalidArgsException,attacker.Train(-3));
	ASSERT_THROW(PokemonInvalidArgsException,attacker.Train(1));

	ASSERT_FALSE( attacker.Hit(victim) );
	victim.Heal();
	ASSERT_NO_THROW(attacker.Train(5));
	ASSERT_TRUE( attacker.Hit(victim) );
	victim.Heal();
	attacker.Train(2);
	ASSERT_TRUE( attacker.Hit(victim) );

	return true;
}



//int main() {
//
//	RUN_TEST(cTorTest);
//	RUN_TEST(operatorEqualTest);
//	RUN_TEST(operatorDiffTest);
//	RUN_TEST(operatorSmallerTest);
//	RUN_TEST(operatorGratherTest);
//	RUN_TEST(operatorSmallerOrEqualTest);
//	RUN_TEST(operatorGratherOrEqualTest);
//	RUN_TEST(operatorOutputTest);
//	RUN_TEST(lvlTest);
//	RUN_TEST(hitTest);
//	RUN_TEST(healTest);
//	RUN_TEST(trainTest);
//
//}
//






