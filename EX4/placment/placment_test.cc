#include "placment.h"
#include "test_utils.h"

//=============================================================================
//									USING
//=============================================================================
using namespace mtm::pokemongo;





//=============================================================================
//								PUBLIC METHODS
//=============================================================================




static bool cTorTest() {

	ASSERT_THROW(PlacmentInvalidArgsException,Placment("belinson",NULL));
	Trainer trainer("ash",RED);
	ASSERT_NO_THROW(Placment("belinson",&trainer));

	return true;
}

static bool getLocationNameTest() {

	Trainer trainer("ash",RED);
	Placment placment1("shufersal",&trainer);
	Placment placment2("aaa",&trainer);
	Placment placment3("aab",&trainer);
	Placment placment4("bba",&trainer);
	Placment placment5("bbb",&trainer);
	Placment placment6("cab",&trainer);
	Placment placment7("lilalo",&trainer);

	ASSERT_EQUAL(placment1.GetLocationName(),"shufersal");
	ASSERT_EQUAL(placment2.GetLocationName(),"aaa");
	ASSERT_EQUAL(placment3.GetLocationName(),"aab");
	ASSERT_EQUAL(placment4.GetLocationName(),"bba");
	ASSERT_EQUAL(placment5.GetLocationName(),"bbb");
	ASSERT_EQUAL(placment6.GetLocationName(),"cab");
	ASSERT_EQUAL(placment7.GetLocationName(),"lilalo");

	return true;
}

static bool getTrainerTest() {

	Trainer trainer1("ash",RED);
	Trainer trainer2("ash",BLUE);
	Trainer trainer3("brook",BLUE);
	Trainer trainer4("misty",YELLOW);
	Trainer trainer5("jessie",BLUE);

	Placment placment1("shufersal",&trainer1);
	Placment placment2("shufersal",&trainer2);
	Placment placment3("shufersal",&trainer3);
	Placment placment4("shufersal",&trainer4);
	Placment placment5("shufersal",&trainer5);

	ASSERT_EQUAL(placment1.GetTrainer(),&trainer1);
	ASSERT_EQUAL(placment2.GetTrainer(),&trainer2);
	ASSERT_EQUAL(placment3.GetTrainer(),&trainer3);
	ASSERT_EQUAL(placment4.GetTrainer(),&trainer4);
	ASSERT_EQUAL(placment5.GetTrainer(),&trainer5);




	return true;
}



//
//int main() {
//
//	RUN_TEST(cTorTest);
//	RUN_TEST(getLocationNameTest);
//	RUN_TEST(getTrainerTest);
//
//	return 0;
//}


