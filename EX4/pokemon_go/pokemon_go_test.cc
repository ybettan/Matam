#include "test_utils.h"
#include "pokemon_go.h"

#include <fstream>
#include <sstream>


using std::ifstream;
using std::istringstream;
using std::ostringstream;
using namespace mtm::pokemongo;


#define CREATE_AND_CONECT_WORLD		\
							World* world = new World(); \
							SetUpWorld(world); \
							world->Connect("holmes_place", "aroma", NORTH, SOUTH);\
							world->Connect("panorama", "shufersal", NORTH, SOUTH);\
							world->Connect("shufersal", "aroma", EAST, WEST);\
							world->Connect("panorama", "holmes_place", EAST, WEST)

#define	DELETE_WORLD	/*delete world - inseted into pokemonGo destructor*/

#define EQUAL_TRAINERS(location,oss,index,str_trainer)	\
									ostringstream oss; \
									oss << *(pg.GetTrainersIn(location)[index]);\
									ASSERT_EQUAL(oss.str(),str_trainer)


void SetUpWorld(World* world) {

	ifstream world_in("world.in");
	string line;

	while (std::getline(world_in, line)) {
	   istringstream line_stream(line);
	   line_stream >> *world;
   }
}


static bool GetScoreTest() {

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	pg.AddTrainer("ash",RED,"shufersal");
	ASSERT_EQUAL(pg.GetScore(RED),1);
	pg.MoveTrainer("ash",SOUTH);
	ASSERT_EQUAL(pg.GetScore(RED),11);

	pg.AddTrainer("brook",BLUE,"aroma");
	ASSERT_EQUAL(pg.GetScore(BLUE),1);
	pg.MoveTrainer("brook",SOUTH);
	ASSERT_EQUAL(pg.GetScore(BLUE),11);
	pg.MoveTrainer("brook",WEST);
	ASSERT_EQUAL(pg.GetScore(BLUE),14);
	ASSERT_EQUAL(pg.GetScore(RED),0);

	pg.MoveTrainer("brook",NORTH);
	ASSERT_EQUAL(pg.GetScore(BLUE),4);
	ASSERT_EQUAL(pg.GetScore(RED),10);

	DELETE_WORLD;
	return true;
}

static bool AddTrainerTest() {

	Trainer trainer100("lilipad",RED);

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	ASSERT_THROW(PokemonGoInvalidArgsException,pg.AddTrainer("",RED,"shufersal"));
	ASSERT_NO_THROW(pg.AddTrainer("ash",RED,"shufersal"));
	ASSERT_THROW(PokemonGoTrainerNameAlreadyUsedExcpetion,
											pg.AddTrainer("ash",BLUE,"panorama"));
	ASSERT_THROW(PokemonGoLocationNotFoundException,
											pg.AddTrainer("brook",BLUE,"lalalend"));

	ASSERT_THROW(PokemonGoLocationNotFoundException,pg.GetTrainersIn("lalalend"));
	// working try will be tested in GetTrainersIn function test

	DELETE_WORLD;
	return true;
}

static bool whereIsTest() {

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion,pg.WhereIs("ash"));
	pg.AddTrainer("ash",BLUE,"panorama");
	ASSERT_EQUAL(pg.WhereIs("ash"),"panorama");

	pg.AddTrainer("brook",BLUE,"panorama");
	ASSERT_EQUAL(pg.WhereIs("brook"),"panorama");

	pg.AddTrainer("misty",YELLOW,"shufersal");
	ASSERT_EQUAL(pg.WhereIs("misty"),"shufersal");

	DELETE_WORLD;
	return true;
}


static bool GetTrainersInTest() {

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	ASSERT_THROW(PokemonGoLocationNotFoundException,pg.GetTrainersIn("lalalend"));

	pg.AddTrainer("ash",RED,"shufersal");
	EQUAL_TRAINERS("shufersal",oss1,0,"ash (1) RED\n");

	pg.AddTrainer("brook",BLUE,"shufersal");
	EQUAL_TRAINERS("shufersal",oss2,0,"ash (1) RED\n");
	EQUAL_TRAINERS("shufersal",oss3,1,"brook (1) BLUE\n");

	pg.AddTrainer("misty",YELLOW,"panorama");
	EQUAL_TRAINERS("panorama",oss4,0,"misty (1) YELLOW\n");

	DELETE_WORLD;
	return true;
}

static bool MoveTrainerTest() {

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion,pg.MoveTrainer("ash",NORTH));
	pg.AddTrainer("ash",RED,"aroma");
	ASSERT_THROW(PokemonGoReachedDeadEndException,pg.MoveTrainer("ash",EAST));

	pg.MoveTrainer("ash",WEST);
	ASSERT_EQUAL(pg.WhereIs("ash"),"shufersal");
	pg.MoveTrainer("ash",SOUTH);
	ASSERT_EQUAL(pg.WhereIs("ash"),"panorama");
	pg.MoveTrainer("ash",EAST);
	ASSERT_EQUAL(pg.WhereIs("ash"),"holmes_place");
	pg.MoveTrainer("ash",NORTH);
	ASSERT_EQUAL(pg.WhereIs("ash"),"aroma");

	DELETE_WORLD;
	return true;
}

static bool PokemonGoTest() {

	CREATE_AND_CONECT_WORLD;
	PokemonGo pg(world);

	ASSERT_EQUAL(pg.GetScore(RED),0);
	ASSERT_EQUAL(pg.GetScore(BLUE),0);
	ASSERT_EQUAL(pg.GetScore(YELLOW),0);

	pg.AddTrainer("Ash", YELLOW, "aroma");
	pg.MoveTrainer("Ash", SOUTH);
	pg.AddTrainer("Clemont", RED, "aroma");

	ASSERT_EQUAL(pg.GetScore(YELLOW), 11);
	ASSERT_EQUAL(pg.GetScore(RED), 1);

	pg.MoveTrainer("Clemont", SOUTH);

	ASSERT_EQUAL(pg.GetScore(YELLOW), 0);
	ASSERT_EQUAL(pg.GetScore(RED), 14);

	const vector<Trainer*>& trainers_in_holmes = pg.GetTrainersIn("holmes_place");
	const vector<Trainer*>& trainers_in_aroma = pg.GetTrainersIn("aroma");

	ASSERT_TRUE(trainers_in_aroma.empty());
	ASSERT_EQUAL(trainers_in_holmes.size(), 2);

	ostringstream output;
	output << *trainers_in_holmes[0];
	output << *trainers_in_holmes[1];

	ASSERT_EQUAL(output.str(),	"Ash (1) YELLOW\n"
							   "pikachu(1/2.5/97)\n"
							   "Clemont (2) RED\n"
							   "charmander(1/3/97.5) NORMAL BUG FAIRY ICE GHOST\n");
	return true;
}


//int main() {
//
//	RUN_TEST(GetScoreTest);
//	RUN_TEST(AddTrainerTest);
//	RUN_TEST(whereIsTest);
//	RUN_TEST(GetTrainersInTest);
//	RUN_TEST(MoveTrainerTest);
//	RUN_TEST(PokemonGoTest);
//
//}







