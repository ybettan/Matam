#include "test_utils.h"
#include "world.h"
#include <fstream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using namespace mtm::pokemongo;




static bool operatorInputTest() {

	World world;

	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	istringstream iss1("NANA banana");
	ASSERT_THROW(WorldInvalidInputLineException,iss1 >> world);
	istringstream iss2("POKESTOP banana tita 30");
	ASSERT_THROW(WorldInvalidInputLineException,iss2 >> world);
	istringstream iss3("POKESTOP banana POTION 0");
	ASSERT_THROW(WorldInvalidInputLineException,iss3 >> world);
	istringstream iss4("POKESTOP banana POTION -6");
	ASSERT_THROW(WorldInvalidInputLineException,iss4 >> world);
	istringstream iss5("STARBUCKS banana charmander 30 0");
	ASSERT_THROW(WorldInvalidInputLineException,iss5 >> world);
	istringstream iss6("POKESTOP banana charmander 30 -6");
	ASSERT_THROW(WorldInvalidInputLineException,iss6 >> world);
	istringstream iss7("STARBUCKS banana charmander 0 10");
	ASSERT_THROW(WorldInvalidInputLineException,iss7 >> world);
	istringstream iss8("POKESTOP banana charmander -1 10");
	ASSERT_THROW(WorldInvalidInputLineException,iss8 >> world);
	istringstream iss9("POKESTOP pokestop1 ");
	ASSERT_NO_THROW(iss9 >> world);
	istringstream iss10("POKESTOP pokestop1 CANDY 3");
	ASSERT_THROW(WorldLocationNameAlreadyUsed,iss10 >> world);
	istringstream iss11("STARBUCKS starbucks1 charmander 20 1");
	ASSERT_NO_THROW(iss11 >> world);
	istringstream iss12("STARBUCKS starbucks1 bulbasaur 30 2");
	ASSERT_THROW(WorldLocationNameAlreadyUsed,iss12 >> world);
	istringstream iss13("GYM gym1");
	ASSERT_NO_THROW(iss13 >> world);
	istringstream iss15("GYM gym2");
	ASSERT_NO_THROW(iss15 >> world);
	istringstream iss16("GYM gym1");
	ASSERT_THROW(WorldLocationNameAlreadyUsed,iss16 >> world);

	return true;
}

static bool InsertTest() {

	World world;

	ASSERT_EQUAL(Gym::GetTeamScore(RED),0);
	ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
	ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

	world.Insert("library");
	ASSERT_TRUE((&world)->Contains("library"));

	(&world)->Insert("library2");
	ASSERT_TRUE((&world)->Contains("library2"));

	world.Insert("superGym",new Gym());
	ASSERT_TRUE((&world)->Contains("superGym"));

	world.Insert("fishbach",new Pokestop("POKESTOP fishbach CANDY 5"));
	ASSERT_TRUE((&world)->Contains("fishbach"));

	ASSERT_THROW(WorldLocationNameAlreadyUsed,world.Insert("library"));

	return true;
}


static bool GetLocationsTest() {

		World world;

		ASSERT_EQUAL(Gym::GetTeamScore(RED),0);
		ASSERT_EQUAL(Gym::GetTeamScore(BLUE),0);
		ASSERT_EQUAL(Gym::GetTeamScore(YELLOW),0);

		ASSERT_EQUAL(world.GetLocations().size(),0);
		world.Insert("place1",new Gym());
		ASSERT_EQUAL(world.GetLocations().size(),1);
		world.Insert("place2",new Pokestop("POKESTOP poki1"));
		ASSERT_EQUAL(world.GetLocations().size(),2);
		world.Insert("place3",new Starbucks("STARBUCKS starki1"));
		ASSERT_EQUAL(world.GetLocations().size(),3);

	return true;
}


//int main() {
//
//		RUN_TEST(operatorInputTest);
//		RUN_TEST(InsertTest);
//		RUN_TEST(GetLocationsTest);
//
//
//}



