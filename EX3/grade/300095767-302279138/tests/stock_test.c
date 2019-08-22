#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../print_utils.h"
#include "../yoni_aux_macros.h"
#include "../stock.h"

//-----------------------------------------------------------------------------
static bool stockCreateTest() {

	bool result = true;

	Stock stock = stockCreate();
	TEST_DIFFERENT( (result),(stock),(NULL) );
	stockDestroy(stock);

	return result;
}
//-----------------------------------------------------------------------------
static int stockGetSizeTest(){

	bool result = true;

	TEST_EQUALS((result),(stockGetSize(NULL)),(-1));

	Stock stock = stockCreate();
	TEST_EQUALS((result),(stockGetSize(stock)),(0));
	stockAddItem(stock,"potion",50,10);
	TEST_EQUALS((result),(stockGetSize(stock)),(1));
	stockAddItem(stock,"potion",50,10);
	TEST_EQUALS((result),(stockGetSize(stock)),(1));
	stockAddItem(stock,"candy",50,10);
	TEST_EQUALS((result),(stockGetSize(stock)),(2));
	stockDestroy(stock);

	return result;
}
//-----------------------------------------------------------------------------
static bool stockAddItemTest() {

	bool result = true;
	Stock stock = stockCreate();

	StockResult score = stockAddItem(NULL,"potion",20,5);
	TEST_EQUALS((result),(score),(STOCK_NULL_ARGUMENT));
	score = stockAddItem(stock,"itzik",20,5);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));
	score = stockAddItem(stock,"potion",0,5);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));
	score = stockAddItem(stock,"potion",20,-3);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));

	TEST_EQUALS(result,stockGetSize(stock),0);
	score = stockAddItem(stock,"potion",20,5);
	TEST_EQUALS((result),(score),(STOCK_SUCCESS));
	TEST_EQUALS((result),(stockGetSize(stock)),(1));
	stockAddItem(stock,"potion",20,5);
	TEST_EQUALS(result,stockGetSize(stock),1);
	stockAddItem(stock,"candy",20,5);
	TEST_EQUALS(result,stockGetSize(stock),2);

	stockDestroy(stock);
	return result;
}
//-----------------------------------------------------------------------------
static bool stockCopyTest() {

	bool result = true;

	TEST_EQUALS((result),(stockCopy(NULL)),(NULL));

	Stock stock = stockCreate();
	stockAddItem(stock,"potion",50,7);
	stockAddItem(stock,"potion",30,4);
	stockAddItem(stock,"candy",100,1);
	stockAddItem(stock,"candy",90,3);
	stockRemoveItem(stock,"potion",50);
	stockRemoveItem(stock,"candy",100);
	Stock stock_copy = stockCopy(stock);
	TEST_EQUALS((result),(stockGetSize(stock)),stockGetSize(stock_copy));

	stockDestroy(stock);
	stockDestroy(stock_copy);
	return result;
}
//-----------------------------------------------------------------------------
static bool stockRemoveItemTest() {

	bool result = true;

	StockResult score = stockRemoveItem(NULL,"potion",20);
	TEST_EQUALS((result),(score),(STOCK_NULL_ARGUMENT));
	Stock stock = stockCreate();
	score = stockRemoveItem(stock,"itzik",20);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));
	score = stockRemoveItem(stock,"potion",0);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));
	score = stockRemoveItem(stock,"potion",-10);
	TEST_EQUALS((result),(score),(STOCK_INVALID_ARGUMENT));
	score = stockRemoveItem(stock,"potion",20);
	TEST_EQUALS((result),(score),(STOCK_ITEM_OUT_OF_STOCK));

	stockAddItem(stock,"potion",20,1);
	score = stockRemoveItem(stock,"potion",20);
	TEST_EQUALS((result),(score),(STOCK_SUCCESS));
	score = stockRemoveItem(stock,"potion",20);
	TEST_EQUALS((result),(score),(STOCK_ITEM_OUT_OF_STOCK));
	TEST_EQUALS((result),(stockGetSize(stock)),(0));
	stockAddItem(stock,"potion",20,2);
	score = stockRemoveItem(stock,"potion",20);
	TEST_EQUALS((result),(score),(STOCK_SUCCESS));
	TEST_EQUALS((result),(stockGetSize(stock)),(1));

	stockDestroy(stock);
	return result;
}
//-----------------------------------------------------------------------------
static bool stockShowInfoTest() {

	bool result = true;

	StockResult score = stockShowInfo(NULL,stdout,true);
	TEST_EQUALS(result,score,STOCK_NULL_ARGUMENT);

	/* the working options where tested manually on files including
	 * STOCK_SUCCESS return value
	 * */

	return result;
}
//-----------------------------------------------------------------------------
static bool stockFindBestPotionTest() {

	bool result = true;
	int value_found = -10;
	Stock stock = stockCreate();


	TEST_EQUALS((result),(stockFindBestPotion(NULL,30,&value_found))
												,(STOCK_NULL_ARGUMENT));
	TEST_EQUALS((result),(stockFindBestPotion(stock,30,NULL))
												,(STOCK_NULL_ARGUMENT));
	TEST_EQUALS((result),(stockFindBestPotion(stock,0,&value_found))
												,(STOCK_INVALID_ARGUMENT));
	TEST_EQUALS((result),(stockFindBestPotion(stock,-100,&value_found))
												,(STOCK_INVALID_ARGUMENT));
	stockAddItem(stock,"candy",30,5);
	stockAddItem(stock,"candy",20,5);
	TEST_EQUALS((result),(stockFindBestPotion(stock,30,&value_found))
											,(STOCK_NO_AVAILABLE_ITEM_FOUND));
	TEST_EQUALS((result),(stockFindBestPotion(stock,30,NULL))
											,(STOCK_NULL_ARGUMENT));
	// STOCK_INVALID_ARGUMENT is stronger than STOCK_NO_AVAILABLE_ITEM_FOUND
	stockAddItem(stock,"potion",50,5);
	TEST_EQUALS((result),(stockFindBestPotion(stock,50,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(50));
	stockAddItem(stock,"potion",70,5);
	TEST_EQUALS((result),(stockFindBestPotion(stock,60,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(70));
	TEST_EQUALS((result),(stockFindBestPotion(stock,100,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(70));

	stockDestroy(stock);
	return result;
}
//-----------------------------------------------------------------------------
static bool stockGetMostValuableCandyTest() {

	bool result = true;
	int value_found = -10;
	Stock stock = stockCreate();

	TEST_EQUALS((result),(stockFindMostValuableCandy(NULL,&value_found))
												,(STOCK_NULL_ARGUMENT));
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,NULL))
												,(STOCK_NULL_ARGUMENT));
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
											,(STOCK_NO_AVAILABLE_ITEM_FOUND));
	stockAddItem(stock,"potion",30,5);
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
											,(STOCK_NO_AVAILABLE_ITEM_FOUND));
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,NULL))
											,(STOCK_NULL_ARGUMENT));
	// STOCK_INVALID_ARGUMENT is stronger than STOCK_NO_AVAILABLE_ITEM_FOUND

	stockAddItem(stock,"candy",30,5);
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(30));
	stockAddItem(stock,"candy",66,2);
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(66));
	stockAddItem(stock,"candy",10,5);
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(66));
	stockRemoveItem(stock,"candy",66);
	stockRemoveItem(stock,"candy",66);
	TEST_EQUALS((result),(stockFindMostValuableCandy(stock,&value_found))
														,(STOCK_SUCCESS));
	TEST_EQUALS((result),(value_found),(30));


	stockDestroy(stock);
	return result;
}
//-----------------------------------------------------------------------------

int main() {

	RUN_TEST(stockCreateTest);		//stockDestroy is tested with valgrind
	RUN_TEST(stockGetSizeTest);
	RUN_TEST(stockAddItemTest);
	RUN_TEST(stockCopyTest);
	RUN_TEST(stockRemoveItemTest);
	RUN_TEST(stockShowInfoTest);
	RUN_TEST(stockFindBestPotionTest);
	RUN_TEST(stockGetMostValuableCandyTest);

}
