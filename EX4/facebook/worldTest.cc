/*
 * worldTest.cc
 *
 *  Created on: Jan 14, 2017
 *      Author: Isacov
 */
#include "world.h"
#include "exceptions.h"
#include "test_utils.h"
#include "pokemon_go.h"
#include "pokemon.h"
#include "trainer.h"
#include "item.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

using namespace mtm::pokemongo;

//static bool trainerTest() {
//	string s = "p";
//	Pokemon a(s, 5, 2);
//	string ss = "pik";
//	Pokemon b(ss, 6, 1);
////	cout << b << endl;
//	string sss = "pikapi";
//	Pokemon d(sss, 20, 1);
////	cout << d << endl;
//	string k = "ash kacham";
//	Team blue = BLUE;
//	Trainer ash(k, blue);
//	ash.TryToCatch(a);
//	ash.TryToCatch(b);
//	ash.TryToCatch(d);
//	Item* p = new Potion(1);
//	Item* c = new Candy(6);
//	ash.addItem(c);
//	ash.addItem(p);
//	ash.useItem(a);
//////	cout << ash;
//	delete p;
//	delete c;
//	return true;
//}

//static bool Starbuckstest() {
//	std::list<Pokemon> list;
//	Pokemon pokemon1("Dor", 1, 1);
//	Pokemon pokemon2("Ariel", 1, 1);
//	list.push_back(pokemon1);
//	list.push_back(pokemon2);
//	string name1("MatamSpring");
//	// 2nd c'tor
//	Starbucks starbucks1(list, name1);
//	ASSERT_TRUE(!starbucks1.isListEmpty());
//	ASSERT_TRUE(
//			!starbucks1.isStringEmpty()
//					&& !name1.compare(starbucks1.getName()));
//	string name2("Empty");
//	// 1st c'tor
//	Starbucks starbucks2(name2);
//	ASSERT_TRUE(starbucks2.isListEmpty());
//	ASSERT_TRUE(
//			!starbucks2.isStringEmpty()
//					&& !name2.compare(starbucks2.getName()));
//	// add pokemon
//	starbucks2.addPokemon(pokemon2);
//	ASSERT_TRUE(!starbucks2.isListEmpty());
//	// Copy c'tor
//	Starbucks starbucks3(starbucks2); // should check that "ariel" is in there
//	ASSERT_TRUE(!starbucks3.isStringEmpty());
//	ASSERT_TRUE(!(starbucks2.getName()).compare(starbucks3.getName()));
//	// operator =
//	starbucks3 = starbucks1; // should check that now "dor" is before "ariel"
//	ASSERT_TRUE(!starbucks3.isStringEmpty());
//	ASSERT_TRUE((starbucks2.getName()).compare(starbucks3.getName()));
//	ASSERT_TRUE(!(starbucks1.getName()).compare(starbucks3.getName()));
////	// remove pokemon
////	starbucks2.removePokemon(pokemon2);
////	ASSERT_TRUE((starbucks2.getPokemons()).empty());
////	starbucks1.removePokemon(pokemon1); // now 1 has ariel and 2 has dor and then ariel
//	// Arrive
//	Trainer trainer1("Kimchi", RED);
//	ASSERT_THROW(LocationTrainerNotFoundException, starbucks1.Leave(trainer1));
//	starbucks1.Arrive(trainer1); // dor is caught by kimchi
//	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
//			starbucks1.Arrive(trainer1));
//	starbucks1.Leave(trainer1);
//	ASSERT_THROW(LocationTrainerNotFoundException, starbucks1.Leave(trainer1));
//	starbucks1.Arrive(trainer1); // ariel is caught by kimchi
//	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
//			starbucks1.Arrive(trainer1));
//	starbucks1.Leave(trainer1);
//	ASSERT_THROW(LocationTrainerNotFoundException, starbucks1.Leave(trainer1));
//	ASSERT_TRUE(starbucks1.isListEmpty());
//	return true;
//}

//static bool Pokestoptest() {
//	std::list<Item*> list;
//	Item* potion1 = new Potion(1);
//	Item* candy2 = new Candy(1);
//	list.push_back(potion1);
//	list.push_back(candy2);
//	string name1("MatamSpring");
//	// 2nd c'tor
//	Pokestop pokestop1(list, name1);
//	ASSERT_TRUE(!pokestop1.isListEmpty());
//	ASSERT_TRUE(
//			!pokestop1.isStringEmpty() && !name1.compare(pokestop1.getName()));
//	string name2("Empty");
//	// 1st c'tor
//	Pokestop pokestop2(name2);
//	ASSERT_TRUE(pokestop2.isListEmpty());
//	ASSERT_TRUE(
//			!pokestop2.isStringEmpty() && !name2.compare(pokestop2.getName()));
//	// add pokemon
//	pokestop2.addItem(candy2);
//	ASSERT_TRUE(!pokestop2.isListEmpty());
//	// Copy c'tor
//	Pokestop pokestop3(pokestop2); // should check that candy2 is in there
//	ASSERT_TRUE(!pokestop3.isStringEmpty());
//	ASSERT_TRUE(!(pokestop2.getName()).compare(pokestop3.getName()));
//	// operator =
//	pokestop3 = pokestop1; // should check that now potion1 is before candy2
//	ASSERT_TRUE(!pokestop3.isStringEmpty());
//	ASSERT_TRUE((pokestop2.getName()).compare(pokestop3.getName()));
//	ASSERT_TRUE(!(pokestop1.getName()).compare(pokestop3.getName()));
//	// Arrive
//	Trainer trainer1("Kimchi", RED);
//	ASSERT_THROW(LocationTrainerNotFoundException, pokestop1.Leave(trainer1));
//	pokestop1.Arrive(trainer1); // potion1 is taken by kimchi
//	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
//			pokestop1.Arrive(trainer1));
//	pokestop1.Leave(trainer1);
//	ASSERT_THROW(LocationTrainerNotFoundException, pokestop1.Leave(trainer1));
//	pokestop1.Arrive(trainer1); // candy2 is caught by kimchi
//	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
//			pokestop1.Arrive(trainer1));
//	pokestop1.Leave(trainer1);
//	ASSERT_THROW(LocationTrainerNotFoundException, pokestop1.Leave(trainer1));
//	ASSERT_TRUE(pokestop1.isListEmpty());
//	delete candy2;
//	delete potion1;
//	return true;
//}

//static bool Gymtest() {
//	string s = "p";
//	Pokemon a(s, 5, 2);
//	string ss = "pik";
//	Pokemon b(ss, 6, 1);
//	string k = "ash kacham";
//	Team blue = BLUE;
//	Trainer ash(k, blue);
//	ash.TryToCatch(b);
//	Gym brooks("brook's gym");
//	Gym dors("dor's gym", ash);
//	Trainer mysti("mysti", RED);
//	brooks.Arrive(mysti);
//	Pokemon c(ss, 15, 1);
//	mysti.TryToCatch(b);
//	mysti.TryToCatch(c);
//	ASSERT_TRUE(brooks.getLeader() == mysti);
//	ASSERT_TRUE(brooks.getName() == "brook's gym");
//	Team red = RED;
//	ASSERT_TRUE(brooks.getTeam() == red);
//	ASSERT_TRUE(mysti.GetStrongestPokemon() == c);
//	dors.Leave(ash);
//	dors.Arrive(mysti);
//	ash.TryToCatch(c);
//	dors.Arrive(ash);
//	ASSERT_TRUE(dors.GetTrainers().size() == 2);
//	ASSERT_FALSE(dors.getLeader().getName() == "mysti");
//	ASSERT_NO_THROW(dors.GetTrainers());
//	return true;
//}

//bool InputOperatortest() {
//	World* world = new World();
//	ifstream world_in("InputWorld.txt");
//	string line;
//	int i = 0;
//	vector<string> vec;
//	while (std::getline(world_in, line)) {
//		i++;
////		cout << i << endl;
//		istringstream line_stream(line);
//		if (i < 13 || i == 16 || i == 32 || i == 45 || i == 70) {
//			ASSERT_NO_THROW(line_stream >> *world);
//		} else if (i < 45) {
//			ASSERT_THROW(WorldInvalidInputLineException, line_stream >> *world);
//		} else {
//			ASSERT_THROW(WorldLocationNameAlreadyUsed, line_stream >> *world);
//		}
//	}
//	delete world;
//	return true;
//}

//static bool WORLDtest() {
//	World* pWorld = new World();
//	ifstream worldIN("InputWorld2.txt");
//	string line = "";
//	while (std::getline(worldIN, line)) {
//		istringstream lineStream(line);
//		lineStream >> *pWorld;
//	} //initialize world
//	delete pWorld;
//	return true;
//}

//static void SetUpWorld(World* world) {
//	ifstream world_in("world.in");
//	string line;
//
//	while (std::getline(world_in, line)) {
//		istringstream line_stream(line);
//		line_stream >> *world;
//	}
//}
//
//static bool TestPokemonGo() {
//	World* world = new World();
//	SetUpWorld(world);
//
//	world->Connect("holmes_place", "aroma", NORTH, SOUTH);
//	world->Connect("panorama", "shufersal", NORTH, SOUTH);
//	world->Connect("shufersal", "aroma", EAST, WEST);
//	world->Connect("panorama", "holmes_place", EAST, WEST);
//
//	PokemonGo pokemon_go(world);
//	pokemon_go.AddTrainer("Ash", YELLOW, "aroma");
//	pokemon_go.MoveTrainer("Ash", SOUTH);
//	pokemon_go.AddTrainer("Clemont", RED, "aroma");
//
//	ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);
//	ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
//
//	pokemon_go.MoveTrainer("Clemont", SOUTH);
//
//	ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);
//	ASSERT_EQUAL(pokemon_go.GetScore(RED), 14);
//
//	const vector<Trainer*>& trainers_in_holmes = pokemon_go.GetTrainersIn(
//			"holmes_place");
//	const vector<Trainer*>& trainers_in_aroma = pokemon_go.GetTrainersIn(
//			"aroma");
//
//	ASSERT_TRUE(trainers_in_aroma.empty());
//	ASSERT_EQUAL(trainers_in_holmes.size(), 2);
//
//	ostringstream output;
//
//	output << *trainers_in_holmes[0];
//	output << *trainers_in_holmes[1];
//
//	ASSERT_EQUAL(output.str(), "Ash (1) YELLOW\n"
//			"pikachu(1/2.5/97)\n"
//			"Clemont (2) RED\n"
//			"charmander(1/3/97.5) NORMAL BUG FAIRY ICE GHOST\n");
//
//	return true;
//}

static void SetUpMegaWorld(World* world) {
	ifstream world_in("megaTest.in");
	string line;

	while (std::getline(world_in, line)) {
		istringstream line_stream(line);
		line_stream >> *world;
	}
}

static bool megaTest() {
	World* world = new World();
	SetUpMegaWorld(world);
	try {
		world->Connect("holmes_place", "aroma", NORTH, NORTH);
		world->Connect("holmes_place", "shufersal", SOUTH, NORTH);
		world->Connect("panorama", "aroma", WEST, SOUTH);
		world->Connect("panorama", "rami_levi", EAST, WEST);
		world->Connect("rami_levi", "aroma", EAST, EAST);
		world->Connect("rami_levi", "shani", NORTH, WEST);
		world->Connect("rami_levi", "technionGYM", SOUTH, EAST);
		world->Connect("shani", "technionGYM", SOUTH, WEST);
		world->Connect("shani", "shufersal", EAST, EAST);
		world->Connect("shufersal", "technionGYM", SOUTH, NORTH);
		world->Connect("shufersal", "aroma", WEST, WEST);
		PokemonGo pokemon_go(world);
		pokemon_go.AddTrainer("snir", BLUE, "panorama");
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("snir", SOUTH));
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("snir", NORTH));
		pokemon_go.AddTrainer("avi", YELLOW, "aroma");
		pokemon_go.MoveTrainer("avi", SOUTH);//avi arrives panorama
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("avi", NORTH));
		pokemon_go.AddTrainer("yosi", BLUE, "panorama");
		pokemon_go.AddTrainer("haim", BLUE, "panorama");
		pokemon_go.AddTrainer("yechezkel", YELLOW, "panorama");
		pokemon_go.AddTrainer("noah", RED, "panorama");
		ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 24);
		ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
		ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 0);
		ASSERT_EQUAL(
				pokemon_go.GetTrainersIn("panorama").size()
						+ pokemon_go.GetTrainersIn("aroma").size(), 6);
		pokemon_go.AddTrainer("rocketTeam", RED, "rami_levi");
		pokemon_go.MoveTrainer("rocketTeam", NORTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "shani");
		pokemon_go.MoveTrainer("rocketTeam", SOUTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "technionGYM");
		pokemon_go.MoveTrainer("rocketTeam", NORTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "shufersal");
		pokemon_go.MoveTrainer("rocketTeam", NORTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "holmes_place");
		pokemon_go.MoveTrainer("rocketTeam", NORTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "aroma");
		pokemon_go.MoveTrainer("avi", EAST);//avi leaves panorama
		ASSERT_TRUE(pokemon_go.WhereIs("avi") == "rami_levi");
		pokemon_go.MoveTrainer("rocketTeam", SOUTH);
		ASSERT_TRUE(pokemon_go.WhereIs("rocketTeam") == "panorama");
		string aroma("aroma");
		world->Remove(aroma);
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("rocketTeam", WEST));
		string technionGYM("technionGYM");
		world->Remove(technionGYM);
		pokemon_go.MoveTrainer("rocketTeam", EAST);
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("rocketTeam", SOUTH));
		pokemon_go.AddTrainer("dor_h'ach", RED, "shani");
		pokemon_go.MoveTrainer("dor_h'ach", WEST);
		ASSERT_EQUAL(pokemon_go.WhereIs("dor_h'ach"), "rami_levi");
		world->Connect("rami_levi", "holmes_place", EAST, NORTH);
		pokemon_go.MoveTrainer("dor_h'ach", EAST);
		world->Connect("panorama", "holmes_place", WEST, EAST);
		ASSERT_EQUAL(pokemon_go.WhereIs("dor_h'ach"), "holmes_place");
		pokemon_go.MoveTrainer("snir", WEST);
		ASSERT_TRUE((*world)["holmes_place"]->GetTrainers().size() == 2);
		ASSERT_TRUE((*world)["panorama"]->GetTrainers().size() == 4);
		ASSERT_THROW(PokemonGoReachedDeadEndException,
				pokemon_go.MoveTrainer("snir", WEST));
		pokemon_go.AddTrainer("tsenzur", RED, "shufersal");
		pokemon_go.AddTrainer("tsilag", YELLOW, "shufersal");
		pokemon_go.AddTrainer("aliza_malek", BLUE, "shufersal");
		for (int i = 0; i < 100; ++i) {
			string name = "blueBallon";
			name += i;
			pokemon_go.AddTrainer(name, BLUE, "shufersal");
		}
		for (int i = 0; i < 100; ++i) {
			string name = "redBallon";
			name += i;
			pokemon_go.AddTrainer(name, RED, "shufersal");
		}
		for (int i = 0; i < 100; ++i) {
			string name = "yellowBallon";
			name += i;
			pokemon_go.AddTrainer(name, YELLOW, "shani");
		}
		ASSERT_EQUAL(pokemon_go.GetScore(RED), 129);
		ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 100);
		ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 114);
		world->Remove("panorama");
//		ASSERT_THROW(WorldLocationNotFoundException , world->Remove("panorama"));
//		ASSERT_THROW(WorldLocationNotFoundException, world->Remove("aroma"));
		world->Remove("shufersal");
		world->Remove("shani");
		world->Remove("holmes_place");
	} catch (...) {
		cout << "caught an unexpected exception" << endl;
	}
	return true;
}

int main() {
//	RUN_TEST(trainerTest);
//	RUN_TEST(Starbuckstest);
//	RUN_TEST(Pokestoptest);
//	RUN_TEST(Gymtest);
//	RUN_TEST(InputOperatortest);
//	RUN_TEST(WORLDtest);
//	RUN_TEST(TestPokemonGo);
//	RUN_TEST(PokemonGogeneralTest);
	RUN_TEST(megaTest);
	return 0;
}

