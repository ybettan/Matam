#include "test_utils.h"
#include "location_extentions.h"

#include <string>

using std::ostringstream;
using std::string;
using std::cout;
using std::endl;
using namespace mtm::pokemongo;


//-----------------------------------------------------------------------------------
static bool gymScoreTest() {

	Gym gym1;
	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);     // FIXME:uncomment
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	Trainer trainer1("ash",RED);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);

	gym1.Arrive(trainer1);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),10);

	gym1.Leave(trainer1);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);

	Trainer trainer2("brook",BLUE);
	Trainer trainer3("james",BLUE);

	gym1.Arrive(trainer2);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),10);
	gym1.Arrive(trainer3);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),10);

	Pokemon pokemon("charmander",50,1);
	trainer1.TryToCatch(pokemon);
	gym1.Arrive(trainer1);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),10);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);

	Trainer trainer4("ok",RED);
	gym1.Arrive(trainer4);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),10);
	gym1.Leave(trainer1);
	ASSERT_EQUAL(Gym::GetTeamScore(RED),10);

	return true;
}
//-----------------------------------------------------------------------------------
static bool gymArriveTest() {

	Gym gym1;
	const vector<Trainer*>& vec = gym1.GetTrainers();
	ASSERT_EQUAL(vec.size(),0);
	Trainer trainer1("ash",RED);

	gym1.Arrive(trainer1);
	ASSERT_EQUAL(vec.size(),1);

	ostringstream o1;
	o1 << *gym1.getLeader();
	ASSERT_EQUAL(o1.str(),"ash (1) RED\n");

	Trainer trainer2("brook",RED);
	Pokemon pokemon1("charmander",30,1);
	ASSERT_TRUE(trainer2.TryToCatch(pokemon1));
	gym1.Arrive(trainer2);
	ASSERT_EQUAL(vec.size(),2);
	ostringstream o2;
	o2 << *gym1.getLeader();
	ASSERT_EQUAL(o2.str(),"ash (1) RED\n");

	ASSERT_THROW(LocationTrainerAlreadyInLocationException,gym1.Arrive(trainer2));

	Pokemon pokemon2("squirtel",35,1);
	Trainer trainer3("garey",BLUE);
	ASSERT_TRUE(trainer3.TryToCatch(pokemon2));
	gym1.Arrive(trainer3);
	ASSERT_EQUAL(vec.size(),3);
	ostringstream o3;
	o3 << *gym1.getLeader();
	ASSERT_EQUAL(o3.str(),"garey (2) BLUE\n"
						  "squirtel(1/35/100) NORMAL\n");

	Trainer trainer4("week",RED);
	Pokemon pokemon3("electrod",1,1);
	ASSERT_TRUE(trainer4.TryToCatch(pokemon3));
	gym1.Arrive(trainer4);
	ASSERT_EQUAL(vec.size(),4);
	ostringstream o4;
	o4 << *gym1.getLeader();
	ASSERT_EQUAL(o4.str(),"garey (3) BLUE\n"
						  "squirtel(1/35/99) NORMAL\n");

	return true;
}
//-----------------------------------------------------------------------------------
static bool gymLeaveTest() {

	Gym gym1;
	Trainer strongest_red1("ash",RED);
	Trainer strongest_blue1("brook",BLUE);
	Trainer strongest_yellow1("misty",YELLOW);
	Trainer medium_red1("med_ash",RED);
	Trainer medium_blue1("med_brook",BLUE);
	Trainer medium_yellow1("med_misty",YELLOW);
	Trainer weak_red1("weak_ash",RED);
	Trainer waek_blue1("weak_brook",BLUE);
	Trainer weak_yellow1("weak_misty",YELLOW);
	Pokemon strong_red("charizard",80,1);
	Pokemon med_red("charmander",20,1);
	Pokemon strong_blue("blastoise",70,1);
	Pokemon med_blue("squirtel",15,1);
	Pokemon strong_yellow("vinozaur",60,1);
	Pokemon med_yellow("bulbsaur",10,1);

	ASSERT_TRUE(strongest_red1.TryToCatch(strong_red));
	ASSERT_TRUE(strongest_blue1.TryToCatch(strong_blue));
	ASSERT_TRUE(strongest_yellow1.TryToCatch(strong_yellow));
	ASSERT_TRUE(medium_red1.TryToCatch(med_red));
	ASSERT_TRUE(medium_blue1.TryToCatch(med_blue));
	ASSERT_TRUE(medium_yellow1.TryToCatch(med_yellow));

	gym1.Arrive(strongest_red1);
	ostringstream o10;
	o10 << *gym1.getLeader();
	ASSERT_EQUAL(o10.str(),"ash (1) RED\n"
						  "charizard(1/80/100) NORMAL ROCK BUG\n");
	gym1.Arrive(strongest_blue1);
	ostringstream o11;
	o11 << *gym1.getLeader();
	ASSERT_EQUAL(o11.str(),"ash (2) RED\n"
						  "charizard(1/80/30) NORMAL ROCK BUG\n");
	gym1.Arrive(strongest_yellow1);
	ostringstream o12;
	o12 << *gym1.getLeader();
	ASSERT_EQUAL(o12.str(),"ash (3) RED\n");
	gym1.Arrive(medium_red1);
	ostringstream o13;
	o13 << *gym1.getLeader();
	ASSERT_EQUAL(o13.str(),"ash (3) RED\n");
	gym1.Arrive(medium_blue1);
	ostringstream o14;
	o14 << *gym1.getLeader();
	ASSERT_EQUAL(o14.str(),"med_brook (3) BLUE\n"
							"squirtel(1/15/100) NORMAL\n");
	gym1.Arrive(medium_yellow1);
	ostringstream o15;
	o15 << *gym1.getLeader();
	ASSERT_EQUAL(o15.str(),"med_brook (4) BLUE\n"
							"squirtel(1/15/90) NORMAL\n");
	gym1.Arrive(weak_red1);
	ostringstream o16;
	o16 << *gym1.getLeader();
	ASSERT_EQUAL(o16.str(),"med_brook (5) BLUE\n"
							"squirtel(1/15/90) NORMAL\n");
	gym1.Arrive(waek_blue1);
	ostringstream o17;
	o17 << *gym1.getLeader();
	ASSERT_EQUAL(o17.str(),"med_brook (5) BLUE\n"
							"squirtel(1/15/90) NORMAL\n");
	gym1.Arrive(weak_yellow1);
	ostringstream o18;
	o18 << *gym1.getLeader();
	ASSERT_EQUAL(o18.str(),"med_brook (6) BLUE\n"
							"squirtel(1/15/90) NORMAL\n");
	ASSERT_EQUAL(gym1.GetTrainers().size(),9);

	gym1.Leave(weak_yellow1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),8);
	ostringstream o2;
	o2 << *gym1.getLeader();
	ASSERT_EQUAL(o2.str(),"med_brook (6) BLUE\n"
						"squirtel(1/15/90) NORMAL\n");

	gym1.Leave(medium_blue1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),7);
	ostringstream o3;
	o3 << *gym1.getLeader();
	ASSERT_EQUAL(o3.str(),"brook (1) BLUE\n"
						"blastoise(1/70/20) NORMAL WATER ICE GHOST\n");

	gym1.Leave(strongest_blue1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),6);
	ostringstream o4;
	o4 << *gym1.getLeader();
	ASSERT_EQUAL(o4.str(),"weak_brook (1) BLUE\n");

	gym1.Leave(waek_blue1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),5);
	ostringstream o5;
	o5 << *gym1.getLeader();
	ASSERT_EQUAL(o5.str(),"misty (2) YELLOW\n"
			 "vinozaur(1/60/0) NORMAL ROCK BUG FAIRY GROUND WATER ICE GHOST\n");

	gym1.Leave(strongest_yellow1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),4);
	ostringstream o6;
	o6 << *gym1.getLeader();
	ASSERT_EQUAL(o6.str(),"med_misty (1) YELLOW\n"
						  "bulbsaur(1/10/85)\n");

	gym1.Leave(medium_yellow1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),3);
	ostringstream o7;
	o7 << *gym1.getLeader();
	ASSERT_EQUAL(o7.str(),"med_ash (1) RED\n"
			"charmander(1/20/40) NORMAL BUG FAIRY ICE GHOST\n");

	gym1.Leave(medium_red1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),2);
	ostringstream o8;
	o8 << *gym1.getLeader();
	ASSERT_EQUAL(o8.str(),"ash (2) RED\n");

	gym1.Leave(strongest_red1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),1);
	ostringstream o9;
	o9 << *gym1.getLeader();
	ASSERT_EQUAL(o9.str(),"weak_ash (1) RED\n");

	gym1.Leave(weak_red1);
	ASSERT_EQUAL(gym1.GetTrainers().size(),0);

	ASSERT_EQUAL(gym1.getLeader(),NULL);


	return true;
}
//-----------------------------------------------------------------------------------
static bool starbucksCtorTest() {

	ASSERT_THROW(PokemonInvalidArgsException,
									Starbucks("STARBUCKS star1 charmander 0 5"));
	ASSERT_THROW(PokemonInvalidArgsException,
									Starbucks("STARBUCKS star2 charmander -5 5"));
	ASSERT_THROW(PokemonInvalidArgsException,
									Starbucks("STARBUCKS star3 charmander 30 0"));
	ASSERT_THROW(PokemonInvalidArgsException,
									Starbucks("STARBUCKS star4 charmander 30 -5"));
	Starbucks starbucks1("STARBUCKS star1");
	ASSERT_EQUAL(starbucks1.getPokemonsNum(),0);
	Starbucks starbucks2("STARBUCKS star2 charmander 30 5");
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),1);
	Starbucks starbucks3("STARBUCKS star3 charmander 30 5 bulbasaur 60 10");
	ASSERT_EQUAL(starbucks3.getPokemonsNum(),2);
	return true;
}

static bool starbucksArriveTest() {

	Starbucks starbucks1("STARBUCKS star1 charmander 30 5 bulbasaur 60 10");
	ASSERT_EQUAL(starbucks1.getPokemonsNum(),2);
	Trainer trainer1("ash",RED);
	starbucks1.Arrive(trainer1);
	ASSERT_EQUAL(starbucks1.getPokemonsNum(),2);
	ostringstream str_out1;
	str_out1 << trainer1;
	ASSERT_EQUAL(str_out1.str(),"ash (1) RED\n");
	ASSERT_EQUAL(*((starbucks1.GetTrainers())[0]),trainer1);

	Starbucks starbucks2("STARBUCKS star2 pidgy 30 1 pikachu 60 1");
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),2);
	Trainer trainer2("brook",BLUE);
	starbucks2.Arrive(trainer2);
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),1);
	ostringstream str_out2;
	str_out2 << trainer2;
	ASSERT_EQUAL(str_out2.str(),"brook (1) BLUE\n"
								"pidgy(1/30/100) NORMAL\n");
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[0]),trainer2);

	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
													starbucks2.Arrive(trainer2));

	Trainer trainer3("misty",YELLOW);
	starbucks2.Arrive(trainer3);
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),0);
	ostringstream str_out3;
	str_out3 << trainer3;
	ASSERT_EQUAL(str_out3.str(),"misty (1) YELLOW\n"
								"pikachu(1/60/100)\n");
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[0]),trainer2);
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[1]),trainer3);

	Trainer trainer4("jessy",YELLOW);
	starbucks2.Arrive(trainer4);
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),0);
	ostringstream str_out4;
	str_out4 << trainer4;
	ASSERT_EQUAL(str_out4.str(),"jessy (1) YELLOW\n");
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[0]),trainer2);
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[1]),trainer3);
	ASSERT_EQUAL(*((starbucks2.GetTrainers())[2]),trainer4);

	return true;
}

static bool starbucksGetPokemonsNumTest() {

	Starbucks starbucks1("STARBUCKS star1");
	ASSERT_EQUAL(starbucks1.getPokemonsNum(),0);
	Starbucks starbucks2("STARBUCKS star2 charmander 30 5");
	ASSERT_EQUAL(starbucks2.getPokemonsNum(),1);
	Starbucks starbucks3("STARBUCKS star3 charmander 30 5 bulbasaur 60 10");
	ASSERT_EQUAL(starbucks3.getPokemonsNum(),2);
	Starbucks starbucks4("STARBUCKS star4 charmander 30 5 bulbasaur 60 10 "
			"pidgy 10 10 electrobuze 50 50 diglette 20 20");
	ASSERT_EQUAL(starbucks4.getPokemonsNum(),5);

	return true;
}
//-----------------------------------------------------------------------------------
static bool pokestopCtorTest() {

	ASSERT_THROW(ItemInvalidArgsException,Pokestop("POKESTOP coffeeTree itzik 5"));
	ASSERT_THROW(ItemInvalidArgsException,Pokestop("POKESTOP coffeeTree POTION 0"));
	ASSERT_THROW(ItemInvalidArgsException,Pokestop("POKESTOP coffeeTree POTION -3"));

	Pokestop pokestop1("POKESTOP lalalend ");
	ASSERT_EQUAL(pokestop1.getItemsNum(),0);
	Pokestop pokestop2("POKESTOP coffeeTree POTION 1");
	ASSERT_EQUAL(pokestop2.getItemsNum(),1);
	Pokestop pokestop3("POKESTOP shufersal CANDY 6 POTION 12");
	ASSERT_EQUAL(pokestop3.getItemsNum(),2);
	Pokestop pokestop4("POKESTOP rami_levi POTION 4 CANDY 7 CANDY 5 POTION 16");
	ASSERT_EQUAL(pokestop4.getItemsNum(),4);

	return true;
}


static bool pokestopArriveTest() {

	Pokestop pokestop1("POKESTOP pok1 POTION 5 CANDY 5");
	ASSERT_EQUAL(pokestop1.getItemsNum(),2);
	Trainer trainer1("ash",RED);
	pokestop1.Arrive(trainer1);
	ASSERT_EQUAL(pokestop1.getItemsNum(),2);
	ASSERT_EQUAL(*((pokestop1.GetTrainers())[0]),trainer1);

	Pokestop pokestop2("POKESTOP pok2 POTION 1 CANDY 1");
	ASSERT_EQUAL(pokestop2.getItemsNum(),2);
	Trainer trainer2("brook",BLUE);
	pokestop2.Arrive(trainer2);
	ASSERT_EQUAL(pokestop2.getItemsNum(),1);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[0]),trainer2);

	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
													pokestop2.Arrive(trainer2));

	Trainer trainer3("misty",YELLOW);
	pokestop2.Arrive(trainer3);
	ASSERT_EQUAL(pokestop2.getItemsNum(),0);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[0]),trainer2);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[1]),trainer3);

	Trainer trainer4("jessy",YELLOW);
	pokestop2.Arrive(trainer4);
	ASSERT_EQUAL(pokestop2.getItemsNum(),0);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[0]),trainer2);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[1]),trainer3);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[2]),trainer4);

	Pokestop pokestop3("POKESTOP pok3 POTION 5 CANDY 4 POTION 3 CANDY 1 POTION 2");
	ASSERT_EQUAL(pokestop3.getItemsNum(),5);
	Trainer trainer5("gary",RED);
	pokestop3.Arrive(trainer5);
	ASSERT_EQUAL(pokestop3.getItemsNum(),4);
	ASSERT_EQUAL(*((pokestop2.GetTrainers())[0]),trainer5);

	return true;
}


static bool pokestopGetItemsNumTest() {

	Pokestop pokestop1("POKESTOP pok1");
	ASSERT_EQUAL(pokestop1.getItemsNum(),0);
	Pokestop pokestop2("POKESTOP pok2 POTION 1");
	ASSERT_EQUAL(pokestop2.getItemsNum(),1);
	Pokestop pokestop3("POKESTOP pok3 CANDY 6 POTION 12");
	ASSERT_EQUAL(pokestop3.getItemsNum(),2);
	Pokestop pokestop4("POKESTOP pok4 POTION 4 CANDY 7 CANDY 5 POTION 16");
	ASSERT_EQUAL(pokestop4.getItemsNum(),4);

	return true;
}

static bool gymInitiallizeTeamsScoreTest() {

	Gym::InitiallizeTeamsScore();

	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	Trainer trainer("ash",RED);
	Gym gym;

	gym.Arrive(trainer);

	ASSERT_NOT_EQUAL(Gym::GetTeamScore(RED),0);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	Gym::InitiallizeTeamsScore();

	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	return true;
}
//-----------------------------------------------------------------------------------



//int main() {
//
//	RUN_TEST(gymScoreTest);		//diff names just to be more visible when testing
//	RUN_TEST(gymArriveTest);
//	RUN_TEST(gymLeaveTest);
//	RUN_TEST(gymInitiallizeTeamsScoreTest);
//	RUN_TEST(starbucksCtorTest);
//	RUN_TEST(starbucksArriveTest);
//	RUN_TEST(starbucksGetPokemonsNumTest);
//	RUN_TEST(pokestopCtorTest);
//	RUN_TEST(pokestopArriveTest);
//	RUN_TEST(pokestopGetItemsNumTest);
//	return 0;
//}



