#include "test_utils.h"
#include "pokemon.h"
#include "trainer.h"

using std::string;
using std::cout;
using std::endl;
using std::list;

using namespace mtm::pokemongo;

//namespace mtm {
//namespace pokemongo {

using mtm::pokemongo::Trainer;

static bool GetTeamScoreTest() {

	Pokemon strong("charmander",50,1);
	Pokemon week("squirtel",10,1);

	ASSERT_EQUAL(Trainer::GetTeamScore(RED),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(YELLOW),0);
	Trainer trainer1("ash",RED);
	ASSERT_EQUAL(Trainer::GetTeamScore(RED),1);
	Trainer trainer2("brook",BLUE);
	ASSERT_EQUAL(Trainer::GetTeamScore(BLUE),1);

	trainer1.TryToCatch(strong);
	trainer2.TryToCatch(week);
	trainer1.attack(trainer2);
	ASSERT_EQUAL(Trainer::GetTeamScore(RED),4);
	trainer1.attack(trainer2);
	ASSERT_EQUAL(Trainer::GetTeamScore(RED),7);


	return true;
}


static bool cTorTest() {

	ASSERT_THROW(TrainerInvalidArgsException,Trainer("",RED));
	ASSERT_NO_THROW(Trainer("ash",RED));

	return true;
}

static bool getPokemonsNumTest() {

	Pokemon pokemon1("charmander",10,1);
	Pokemon pokemon2("bulbasaur",10,1);
	Pokemon pokemon3("electrobuz",10,1);

	Trainer trainer("ash",YELLOW);

	trainer.TryToCatch(pokemon1);
	trainer.TryToCatch(pokemon2);
	trainer.TryToCatch(pokemon3);

	return true;
}

static bool tryToCatchTest() {

	Pokemon pokemon1("charmander",30.4,10);
	Pokemon pokemon2("bulbasaur",70,1);
	Pokemon pokemon3("electrobuz",70,1);
	Pokemon pokemon4("squirtel",70,1);
	Trainer trainer("ash",RED);

	ASSERT_FALSE(trainer.TryToCatch(pokemon1));
	ASSERT_TRUE(trainer.TryToCatch(pokemon2));
	ASSERT_TRUE(trainer.TryToCatch(pokemon3));
	ASSERT_TRUE(trainer.TryToCatch(pokemon4));

	return true;
}

static bool tryToTakeTest() {

	ASSERT_THROW(ItemInvalidArgsException,Item(POTION,0));
	ASSERT_THROW(ItemInvalidArgsException,Item(POTION,-3));
	ASSERT_THROW(ItemInvalidArgsException,Item(CANDY,0));
	ASSERT_THROW(ItemInvalidArgsException,Item(CANDY,-100));
	ASSERT_NO_THROW(Item(CANDY,100));

	Item item1(POTION,4);
	Item item2(CANDY,1);
	Item item3(POTION,1);
	Item item4(CANDY,1);

	Trainer trainer("ash",RED);

	ASSERT_FALSE(trainer.TryToTake(item1));
	ASSERT_TRUE(trainer.TryToTake(item2));
	ASSERT_TRUE(trainer.TryToTake(item3));
	ASSERT_TRUE(trainer.TryToTake(item4));


	return true;
}

static bool getTeamTest() {

	Trainer trainer1("ash",RED);
	Trainer trainer2("brook",BLUE);
	Trainer trainer3("ash",YELLOW);

	ASSERT_TRUE(trainer1.GetTeam() == RED);
	ASSERT_TRUE(trainer2.GetTeam() == BLUE);
	ASSERT_TRUE(trainer3.GetTeam() == YELLOW);

	ASSERT_FALSE(trainer1.GetTeam() == BLUE);
	ASSERT_FALSE(trainer2.GetTeam() == YELLOW);
	ASSERT_FALSE(trainer3.GetTeam() == RED);


	return true;
}

static bool IsAllyTest() {

	Trainer trainer1("ash",RED);
	Trainer trainer2("brook",BLUE);
	Trainer trainer3("ash",YELLOW);
	Trainer trainer4("jessy",RED);
	Trainer trainer5("dr ok",BLUE);
	Trainer trainer6("gary",YELLOW);

	ASSERT_TRUE(trainer1.IsAlly(trainer4));
	ASSERT_TRUE(trainer2.IsAlly(trainer5));
	ASSERT_TRUE(trainer3.IsAlly(trainer6));
	ASSERT_TRUE(trainer1.IsAlly(trainer1));

	ASSERT_FALSE(trainer1.IsAlly(trainer2));
	ASSERT_FALSE(trainer1.IsAlly(trainer3));
	ASSERT_FALSE(trainer2.IsAlly(trainer4));
	ASSERT_FALSE(trainer2.IsAlly(trainer6));


	return true;
}


static bool getStrongestPokemonTest() {

	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);
	Pokemon pokemon4("pidgy",40,1);
	Pokemon pokemon5("squirtel",40,1);

	Trainer ash("ash",RED);
	Trainer ash_const("ash",RED);

	ASSERT_THROW(TrainerNoPokemonsFoundException,ash.GetStrongestPokemon());
	ASSERT_TRUE(ash.TryToCatch(pokemon1));
	ASSERT_NO_THROW(ash.GetStrongestPokemon() == pokemon1);
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon1);
	ASSERT_THROW(TrainerNoPokemonsFoundException,ash_const.GetStrongestPokemon());
	ASSERT_NO_THROW(ash_const.TryToCatch(pokemon1));
	ASSERT_TRUE(ash_const.GetStrongestPokemon() == pokemon1);

	ASSERT_TRUE(ash.TryToCatch(pokemon2));
	ASSERT_TRUE(ash_const.TryToCatch(pokemon2));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon1);
	ASSERT_TRUE(ash_const.GetStrongestPokemon() == pokemon1);
	ASSERT_TRUE(ash.TryToCatch(pokemon3));
	ASSERT_TRUE(ash_const.TryToCatch(pokemon3));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon3);
	ASSERT_TRUE(ash_const.GetStrongestPokemon() == pokemon3);
	ASSERT_TRUE(ash.TryToCatch(pokemon4));
	ASSERT_TRUE(ash_const.TryToCatch(pokemon4));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon4);
	ASSERT_TRUE(ash_const.GetStrongestPokemon() == pokemon4);
	ASSERT_TRUE(ash.TryToCatch(pokemon5));
	ASSERT_TRUE(ash_const.TryToCatch(pokemon5));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon4);
	ASSERT_TRUE(ash_const.GetStrongestPokemon() == pokemon4);

	return true;
}

static bool killStrongestPokemonTest() {

	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);
	Pokemon pokemon4("pidgy",40,1);
	Pokemon pokemon5("squirtel",40,1);

	Trainer ash("ash",RED);

	ASSERT_THROW(TrainerNoPokemonsFoundException,ash.KillStrongestPokemon());
	ASSERT_NO_THROW(ash.TryToCatch(pokemon1));
	ash.KillStrongestPokemon();

	ASSERT_TRUE(ash.TryToCatch(pokemon1));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon1);
	ASSERT_TRUE(ash.TryToCatch(pokemon2));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon1);
	ASSERT_TRUE(ash.TryToCatch(pokemon3));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon3);
	ASSERT_TRUE(ash.TryToCatch(pokemon4));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon4);
	ASSERT_TRUE(ash.TryToCatch(pokemon5));
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon4);

	ash.KillStrongestPokemon();
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon5);
	ash.KillStrongestPokemon();
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon3);
	ash.KillStrongestPokemon();
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon1);
	ash.KillStrongestPokemon();
	ASSERT_TRUE(ash.GetStrongestPokemon() == pokemon2);

	return true;
}


static bool operatorEqualTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_TRUE(trainer1 == trainer2);

	trainer1.TryToCatch(pokemon1);
	ASSERT_FALSE(trainer1 == trainer2);

	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 == trainer2);

	trainer1.TryToCatch(pokemon2);

	ASSERT_TRUE(trainer1 == trainer2);
	trainer1.TryToCatch(pokemon3);
	ASSERT_FALSE(trainer1 == trainer2);

	return true;
}

static bool operatorDiffTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_FALSE(trainer1 != trainer2);
	trainer1.TryToCatch(pokemon1);
	ASSERT_TRUE(trainer1 != trainer2);
	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_TRUE(trainer1 != trainer2);
	trainer1.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 != trainer2);
	trainer1.TryToCatch(pokemon3);
	ASSERT_TRUE(trainer1 != trainer2);

	return true;
}


static bool operatorSmallerTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_FALSE(trainer1 < trainer2);
	trainer1.TryToCatch(pokemon1);
	ASSERT_FALSE(trainer1 < trainer2);
	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_TRUE(trainer1 < trainer2);
	trainer1.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 < trainer2);
	trainer2.TryToCatch(pokemon3);
	ASSERT_TRUE(trainer1 < trainer2);

	return true;
}


static bool operatorGratherTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_FALSE(trainer1 > trainer2);
	trainer1.TryToCatch(pokemon1);
	ASSERT_TRUE(trainer1 > trainer2);
	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 > trainer2);
	trainer1.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 > trainer2);
	trainer1.TryToCatch(pokemon3);
	ASSERT_TRUE(trainer1 > trainer2);

	return true;
}


static bool operatorSmallerOrEqualTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_TRUE(trainer1 <= trainer2);
	trainer1.TryToCatch(pokemon1);
	ASSERT_FALSE(trainer1 <= trainer2);
	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_TRUE(trainer1 <= trainer2);
	trainer1.TryToCatch(pokemon2);
	ASSERT_TRUE(trainer1 <= trainer2);
	trainer2.TryToCatch(pokemon3);
	ASSERT_TRUE(trainer1 <= trainer2);

	return true;
}


static bool operatorGratherOrEqualTest() {

	Trainer trainer1("Ash",RED) , trainer2("brook",BLUE);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);

	ASSERT_TRUE(trainer1 >= trainer2);
	trainer1.TryToCatch(pokemon1);
	ASSERT_TRUE(trainer1 >= trainer2);
	trainer1.KillStrongestPokemon();
	trainer2.TryToCatch(pokemon2);
	ASSERT_FALSE(trainer1 >= trainer2);
	trainer1.TryToCatch(pokemon2);
	ASSERT_TRUE(trainer1 >= trainer2);
	trainer1.TryToCatch(pokemon3);
	ASSERT_TRUE(trainer1 >= trainer2);

	return true;
}


static bool operatorOutputTest() {

	Trainer trainer("Ash",RED);
	Pokemon pokemon1("charmander",20,1);
	Pokemon pokemon2("bulbasaur",20,1);
	Pokemon pokemon3("squirtel",30,1);
	std::ostringstream str_out1;
	str_out1 << trainer;

	ASSERT_EQUAL(str_out1.str(),"Ash (1) RED\n");

	trainer.TryToCatch(pokemon1);
	trainer.TryToCatch(pokemon2);
	trainer.TryToCatch(pokemon3);


	std::ostringstream str_out2;
	str_out2 << trainer;
	ASSERT_EQUAL(str_out2.str(),
				"Ash (1) RED\n"
				"charmander(1/20/100) NORMAL BUG FAIRY ICE GHOST\n"
				"bulbasaur(1/20/100)\n"
				"squirtel(1/30/100) NORMAL\n");

	return true;
}

static bool attackTest() {

	Item item_lvl1(CANDY,1);
	Item item_lvl2(CANDY,2);
	Item item_lvl3(POTION,3);

	/* equal trainers(with and without) - winner descided by team
	 * no changes to trainers
	 */
	Trainer trainer1("ash",BLUE);
	Trainer trainer2("brook",RED);
	ASSERT_TRUE(trainer1.attack(trainer2));
	ASSERT_FALSE(trainer2.attack(trainer1));
	ASSERT_FALSE(trainer1.TryToTake(item_lvl2));
	Pokemon pokemon1("charmander",20,1);
	ASSERT_TRUE(trainer1.TryToCatch(pokemon1));
	ASSERT_TRUE(trainer2.TryToCatch(pokemon1));
	ASSERT_TRUE(trainer1.attack(trainer2));
	ASSERT_FALSE(trainer1.TryToTake(item_lvl2));
	ASSERT_EQUAL(trainer1.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer2.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer1.GetItemsNum(),0);
	ASSERT_EQUAL(trainer2.GetItemsNum(),0);
	trainer2.KillStrongestPokemon();

	/* trainer with pokemon VS trainer without pokemon - automatic win
	 * winner levels up
	 */
	Trainer trainer3("misty",RED);
	Trainer trainer4("jessy",YELLOW);
	Pokemon pokemon3("charmander",20,1);
	ASSERT_TRUE(trainer3.TryToCatch(pokemon3));
	ASSERT_TRUE(trainer3.attack(trainer4));
	ASSERT_TRUE(trainer3.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (none pokemon die) - winner by >
	 * winers levles up
	 */
	Trainer trainer5("misty",RED);
	Trainer trainer6("jessy",YELLOW);
	Pokemon pokemon5("charmander",20,1);
	Pokemon pokemon6("squirtel",10,1);
	ASSERT_TRUE(trainer5.TryToCatch(pokemon5));
	ASSERT_TRUE(trainer6.TryToCatch(pokemon6));
	ASSERT_TRUE(trainer5.attack(trainer6));
	ASSERT_EQUAL(trainer5.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer6.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer5.GetItemsNum(),0);
	ASSERT_EQUAL(trainer6.GetItemsNum(),0);
	ASSERT_TRUE(trainer5.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (one pokemon die) - winner by >
	 * winers levles up and pokemon is deleted
	 */
	Trainer trainer7("gary",RED);
	Trainer trainer8("james",YELLOW);
	Pokemon pokemon7("charmander",120,1);
	Pokemon pokemon8("squirtel",10,1);
	ASSERT_TRUE(trainer7.TryToCatch(pokemon7));
	ASSERT_TRUE(trainer8.TryToCatch(pokemon8));
	ASSERT_TRUE(trainer7.attack(trainer8));
	ASSERT_EQUAL(trainer7.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer8.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer7.GetItemsNum(),0);
	ASSERT_EQUAL(trainer8.GetItemsNum(),0);
	ASSERT_TRUE(trainer7.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer7.attack(trainer8));
	ASSERT_EQUAL(trainer7.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer8.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer7.GetItemsNum(),1);
	ASSERT_EQUAL(trainer8.GetItemsNum(),0);
	ASSERT_TRUE(trainer7.TryToTake(item_lvl3));

	// test if work when second pokemon dies
	Trainer trainer9("gary",RED);
	Trainer trainer10("james",YELLOW);
	Pokemon pokemon9("squirtel",10,1);
	Pokemon pokemon10("charmander",120,1);
	ASSERT_TRUE(trainer9.TryToCatch(pokemon9));
	ASSERT_TRUE(trainer10.TryToCatch(pokemon10));
	ASSERT_FALSE(trainer9.attack(trainer10));
	ASSERT_EQUAL(trainer9.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer10.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer9.GetItemsNum(),0);
	ASSERT_EQUAL(trainer10.GetItemsNum(),0);
	ASSERT_FALSE(trainer9.TryToTake(item_lvl2));
	ASSERT_FALSE(trainer9.attack(trainer10));
	ASSERT_EQUAL(trainer9.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer10.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer9.GetItemsNum(),0);
	ASSERT_EQUAL(trainer10.GetItemsNum(),0);
	ASSERT_FALSE(trainer9.TryToTake(item_lvl3));

	/* 2 trainers with pokemns (2 pokemons die) - winner by >
	 * winers levles up and pokemons are deleted
	 */
	Trainer trainer11("yoni",RED);
	Trainer trainer12("itzik",YELLOW);
	Pokemon pokemon11("charmander",120,1);
	Pokemon pokemon12("squirtel",110,1);
	ASSERT_TRUE(trainer11.TryToCatch(pokemon11));
	ASSERT_TRUE(trainer12.TryToCatch(pokemon12));
	ASSERT_TRUE(trainer11.attack(trainer12));
	ASSERT_EQUAL(trainer11.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer12.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer11.GetItemsNum(),0);
	ASSERT_EQUAL(trainer12.GetItemsNum(),0);
	ASSERT_TRUE(trainer11.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer11.attack(trainer12));
	ASSERT_EQUAL(trainer11.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer12.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer11.GetItemsNum(),1);
	ASSERT_EQUAL(trainer12.GetItemsNum(),0);
	ASSERT_FALSE(trainer11.TryToTake(item_lvl3));
//---------------------------------------------------------------------------------
	/* equal trainers(with and without pokemons)(first trainer have items)
	 * - winner descided by team
	 * no changes to trainers
	 */
	Trainer trainer13("avi",BLUE);
	Trainer trainer14("nimni",RED);
	ASSERT_TRUE(trainer13.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer13.attack(trainer14));
	ASSERT_FALSE(trainer14.attack(trainer13));
	ASSERT_EQUAL(trainer13.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer14.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer13.GetItemsNum(),1);
	ASSERT_EQUAL(trainer14.GetItemsNum(),0);
	ASSERT_FALSE(trainer13.TryToTake(item_lvl2));
	Pokemon pokemon13("charmander",20,1);
	ASSERT_TRUE(trainer13.TryToCatch(pokemon13));
	ASSERT_TRUE(trainer14.TryToCatch(pokemon13));
	ASSERT_TRUE(trainer13.attack(trainer14));
	ASSERT_EQUAL(trainer13.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer14.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer13.GetItemsNum(),1);
	ASSERT_EQUAL(trainer14.GetItemsNum(),0);
	ASSERT_FALSE(trainer13.TryToTake(item_lvl2));
	ASSERT_EQUAL(trainer13.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer14.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer13.GetItemsNum(),1);
	ASSERT_EQUAL(trainer14.GetItemsNum(),0);

	/* trainer with pokemon VS trainer without pokemon (second trainer have items)
	 * - automatic win
	 * winner levels up
	 */
	Trainer trainer15("ronen",RED);
	Trainer trainer16("mizraki",YELLOW);
	Pokemon pokemon15("charmander",20,1);
	ASSERT_TRUE(trainer15.TryToCatch(pokemon15));
	ASSERT_TRUE(trainer15.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer15.attack(trainer16));
	ASSERT_EQUAL(trainer15.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer16.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer15.GetItemsNum(),1);
	ASSERT_EQUAL(trainer16.GetItemsNum(),0);
	ASSERT_TRUE(trainer15.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (none pokemon die) - one of them have candy
	 * winner by >
	 * winers levles up
	 */
	Trainer trainer17("lolo",RED);
	Trainer trainer18("komoyo",YELLOW);
	Pokemon pokemon17("charmander",20,1);
	Pokemon pokemon18("squirtel",10,1);
	ASSERT_TRUE(trainer17.TryToCatch(pokemon17));
	ASSERT_TRUE(trainer18.TryToCatch(pokemon18));
	ASSERT_TRUE(trainer18.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer17.attack(trainer18));
	ASSERT_EQUAL(trainer17.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer18.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer17.GetItemsNum(),0);
	ASSERT_EQUAL(trainer18.GetItemsNum(),0);
	ASSERT_TRUE(trainer17.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (one pokemon die) - one of them have potion
	 * winner by >
	 * winers levles up and pokemon is deleted
	 */
	Trainer trainer19("loko",RED);
	Trainer trainer20("pormihavana",YELLOW);
	Pokemon pokemon19("charmander",120,1);
	Pokemon pokemon20("squirtel",10,1);
	ASSERT_TRUE(trainer19.TryToCatch(pokemon19));
	ASSERT_TRUE(trainer20.TryToCatch(pokemon20));
	ASSERT_TRUE(trainer19.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer19.attack(trainer20));
	ASSERT_EQUAL(trainer19.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer20.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer19.GetItemsNum(),0);
	ASSERT_EQUAL(trainer20.GetItemsNum(),0);
	ASSERT_TRUE(trainer19.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer19.attack(trainer20));
	ASSERT_EQUAL(trainer19.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer20.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer19.GetItemsNum(),1);
	ASSERT_EQUAL(trainer20.GetItemsNum(),0);
	ASSERT_TRUE(trainer19.TryToTake(item_lvl3));

	/* 2 trainers with pokemns (2 pokemons die) - one of them have potion
	 * winner by >
	 * winers levles up and pokemons are deleted
	 */
	Trainer trainer21("mimi",RED);
	Trainer trainer22("messica",YELLOW);
	Pokemon pokemon21("charmander",120,1);
	Pokemon pokemon22("squirtel",110,1);
	ASSERT_TRUE(trainer21.TryToCatch(pokemon21));
	ASSERT_TRUE(trainer22.TryToCatch(pokemon22));
	ASSERT_TRUE(trainer21.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer21.attack(trainer22));
	ASSERT_EQUAL(trainer21.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer22.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer21.GetItemsNum(),0);
	ASSERT_EQUAL(trainer22.GetItemsNum(),0);
	ASSERT_TRUE(trainer21.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer21.attack(trainer22));
	ASSERT_EQUAL(trainer21.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer22.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer21.GetItemsNum(),1);
	ASSERT_EQUAL(trainer22.GetItemsNum(),0);
	ASSERT_FALSE(trainer21.TryToTake(item_lvl3));
//---------------------------------------------------------------------------------
	/* equal trainers(with and without pokemons)(both trainer have items)
	 * - winner descided by team
	 * no changes to trainers
	 */
	Trainer trainer23("roni",BLUE);
	Trainer trainer24("superstar",RED);
	ASSERT_TRUE(trainer23.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer24.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer23.attack(trainer24));
	ASSERT_FALSE(trainer24.attack(trainer23));
	ASSERT_EQUAL(trainer23.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer24.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer23.GetItemsNum(),1);
	ASSERT_EQUAL(trainer24.GetItemsNum(),1);
	ASSERT_FALSE(trainer23.TryToTake(item_lvl2));
	Pokemon pokemon23("charmander",20,1);
	ASSERT_TRUE(trainer23.TryToCatch(pokemon23));
	ASSERT_TRUE(trainer24.TryToCatch(pokemon23));
	ASSERT_TRUE(trainer23.attack(trainer24));
	ASSERT_EQUAL(trainer23.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer24.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer23.GetItemsNum(),1);
	ASSERT_EQUAL(trainer24.GetItemsNum(),1);
	ASSERT_FALSE(trainer23.TryToTake(item_lvl2));
	ASSERT_EQUAL(trainer23.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer24.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer23.GetItemsNum(),1);
	ASSERT_EQUAL(trainer24.GetItemsNum(),1);

	/* trainer with pokemon VS trainer without pokemon (both trainer have items)
	 * - automatic win
	 * winner levels up
	 */
	Trainer trainer25("momo",RED);
	Trainer trainer26("hapara",YELLOW);
	Pokemon pokemon25("charmander",20,1);
	ASSERT_TRUE(trainer25.TryToCatch(pokemon25));
	ASSERT_TRUE(trainer25.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer26.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer25.attack(trainer26));
	ASSERT_EQUAL(trainer25.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer26.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer25.GetItemsNum(),1);
	ASSERT_EQUAL(trainer26.GetItemsNum(),1);
	ASSERT_TRUE(trainer25.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (none pokemon die) - both of them have candy
	 * winner by >
	 * winers levles up
	 */
	Trainer trainer27("aviv",RED);
	Trainer trainer28("tefen",YELLOW);
	Pokemon pokemon27("charmander",20,1);
	Pokemon pokemon28("squirtel",10,1);
	ASSERT_TRUE(trainer27.TryToCatch(pokemon27));
	ASSERT_TRUE(trainer28.TryToCatch(pokemon28));
	ASSERT_TRUE(trainer27.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer28.TryToTake(item_lvl1));

	ASSERT_TRUE(trainer27.attack(trainer28));
	ASSERT_EQUAL(trainer27.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer28.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer27.GetItemsNum(),0);
	ASSERT_EQUAL(trainer28.GetItemsNum(),0);
	ASSERT_TRUE(trainer27.TryToTake(item_lvl2));

	/* 2 trainers with pokemns (one pokemon die) - both of them have potion
	 * winner by >
	 * winers levles up and pokemon is deleted
	 */
	Trainer trainer29("kiki",RED);
	Trainer trainer30("miraje",YELLOW);
	Pokemon pokemon29("charmander",120,1);
	Pokemon pokemon30("squirtel",10,1);
	ASSERT_TRUE(trainer29.TryToCatch(pokemon29));
	ASSERT_TRUE(trainer30.TryToCatch(pokemon30));
	ASSERT_TRUE(trainer29.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer30.TryToTake(item_lvl1));

	ASSERT_TRUE(trainer29.attack(trainer30));
	ASSERT_EQUAL(trainer29.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer30.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer29.GetItemsNum(),0);
	ASSERT_EQUAL(trainer30.GetItemsNum(),0);
	ASSERT_TRUE(trainer29.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer29.attack(trainer30));
	ASSERT_EQUAL(trainer29.GetPokemonsNum(),1);
	ASSERT_EQUAL(trainer30.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer29.GetItemsNum(),1);
	ASSERT_EQUAL(trainer30.GetItemsNum(),0);
	ASSERT_TRUE(trainer29.TryToTake(item_lvl3));

	/* 2 trainers with pokemns (2 pokemons die) - both of them have potion
	 * winner by >
	 * winers levles up and pokemons are deleted
	 */
	Trainer trainer31("rima",RED);
	Trainer trainer32("malhutit",YELLOW);
	Pokemon pokemon31("charmander",120,1);
	Pokemon pokemon32("squirtel",110,1);
	ASSERT_TRUE(trainer31.TryToCatch(pokemon31));
	ASSERT_TRUE(trainer32.TryToCatch(pokemon32));
	ASSERT_TRUE(trainer31.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer32.TryToTake(item_lvl1));
	ASSERT_TRUE(trainer31.attack(trainer32));
	ASSERT_EQUAL(trainer21.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer32.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer31.GetItemsNum(),0);
	ASSERT_EQUAL(trainer32.GetItemsNum(),0);
	ASSERT_TRUE(trainer31.TryToTake(item_lvl2));
	ASSERT_TRUE(trainer31.attack(trainer32));
	ASSERT_EQUAL(trainer31.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer32.GetPokemonsNum(),0);
	ASSERT_EQUAL(trainer31.GetItemsNum(),1);
	ASSERT_EQUAL(trainer32.GetItemsNum(),0);
	ASSERT_FALSE(trainer31.TryToTake(item_lvl3));

	return true;
}



static bool GetNameTest() {

	Trainer trainer1("Ash",RED);
	ASSERT_EQUAL(trainer1.GetName(),"Ash");

	Trainer trainer2("brook",BLUE);
	ASSERT_EQUAL(trainer2.GetName(),"brook");

	Trainer trainer3("misty",YELLOW);
	ASSERT_EQUAL(trainer3.GetName(),"misty");

	Trainer trainer4("jammes",BLUE);
	ASSERT_EQUAL(trainer4.GetName(),"jammes");

	Trainer trainer5("jessie",YELLOW);
	ASSERT_EQUAL(trainer5.GetName(),"jessie");


	return true;
}

static bool InitiallizeTeamsScoreTest() {

	Trainer::InitiallizeTeamsScore();

	ASSERT_EQUAL(Trainer::GetTeamScore(RED),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(YELLOW),0);

	Trainer trainer1("ash",RED);
	Trainer trainer2("brook",BLUE);

	ASSERT_NOT_EQUAL(Trainer::GetTeamScore(RED),0);
	ASSERT_NOT_EQUAL(Trainer::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(YELLOW),0);

	Trainer::InitiallizeTeamsScore();

	ASSERT_EQUAL(Trainer::GetTeamScore(RED),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Trainer::GetTeamScore(YELLOW),0);

	return true;
}

//}  // namespace mtm
//}  // namespace pokemongo

//int main() {
//
//	RUN_TEST(GetTeamScoreTest);
//	RUN_TEST(cTorTest);
//	RUN_TEST(getPokemonsNumTest);
//	RUN_TEST(tryToCatchTest);
//	RUN_TEST(tryToTakeTest);
//	RUN_TEST(getTeamTest);
//	RUN_TEST(IsAllyTest);
//	RUN_TEST(getStrongestPokemonTest);
//	RUN_TEST(killStrongestPokemonTest);
//	RUN_TEST(operatorEqualTest);
//	RUN_TEST(operatorDiffTest);
//	RUN_TEST(operatorSmallerTest);
//	RUN_TEST(operatorGratherTest);
//	RUN_TEST(operatorSmallerOrEqualTest);
//	RUN_TEST(operatorGratherOrEqualTest);
//	RUN_TEST(operatorOutputTest);
//	RUN_TEST(attackTest);
//	RUN_TEST(GetNameTest);
//	RUN_TEST(InitiallizeTeamsScoreTest);
//
//	return 0;
//}


