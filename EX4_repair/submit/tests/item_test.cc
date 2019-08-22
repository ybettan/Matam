#include "test_utils.h"
#include "item.h"
#include "exceptions.h"

using std::string;
using std::cout;
using std::endl;
using mtm::pokemongo::Item;

using namespace mtm::pokemongo;



static bool cTorTest() {

	ASSERT_THROW(ItemInvalidArgsException,Item(POTION,-3));
	ASSERT_NO_THROW(Item(POTION,7));

	return true;
}

static bool getDataTest() {

	Item item1(CANDY,6);
	ASSERT_TRUE(item1.getType() == CANDY);
	ASSERT_TRUE(item1.getLvl() == 6);

	Item item2(POTION,100);
	ASSERT_TRUE(item2.getType() == POTION);
	ASSERT_TRUE(item2.getLvl() == 100);

	Item item3(CANDY,40);
	ASSERT_TRUE(item3.getType() == CANDY);
	ASSERT_FALSE(item3.getLvl() == 6);

	return true;
}




//int main() {
//
//	RUN_TEST(cTorTest);
//	RUN_TEST(getDataTest);
//}
