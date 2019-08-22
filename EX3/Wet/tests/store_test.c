#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../yoni_aux_macros.h"
#include "../store.h"

//-----------------------------------------------------------------------------
static bool storeCreateTest() {

	bool result = true;

	Store store = storeCreate();
	TEST_DIFFERENT( (result),(store),(NULL) );
	storeDestroy(store);

	return result;
}
//-----------------------------------------------------------------------------
static bool storeGetSizeTest() {

	bool result = true;

	TEST_EQUALS((result),(storeGetSize(NULL)),(-1));

	Store store = storeCreate();
	TEST_EQUALS((result),(storeGetSize(store)),(0));
	//more test will be execute in storeAddItemTest/storeRemoveItemTest
	storeDestroy(store);

	return result;
}
//-----------------------------------------------------------------------------
static bool storeAddItemTest() {

	bool result = true;
	Store store = storeCreate();

	TEST_EQUALS((result),(storeAddItem(NULL,"potion",20,5)),
												(STORE_NULL_ARGUMENT));
	TEST_EQUALS((result),(storeAddItem(store,"itzik",20,5)),
												(STORE_INVALID_ARGUMENT));
	TEST_EQUALS((result),(storeAddItem(store,"potion",0,5)),
												(STORE_INVALID_ARGUMENT));
	TEST_EQUALS((result),(storeAddItem(store,"potion",20,-7)),
												(STORE_INVALID_ARGUMENT));

	TEST_EQUALS((result),(storeAddItem(store,"potion",20,5)),
												(STORE_SUCCESS));
	TEST_EQUALS((result),(storeGetSize(store)),(1));
	TEST_EQUALS((result),(storeAddItem(store,"potion",20,5)),
												(STORE_SUCCESS));
	TEST_EQUALS((result),(storeGetSize(store)),(1));
	TEST_EQUALS((result),(storeAddItem(store,"candy",20,5)),
												(STORE_SUCCESS));
	TEST_EQUALS((result),(storeGetSize(store)),(2));
	TEST_EQUALS((result),(storeAddItem(store,"potion",30,5)),
												(STORE_SUCCESS));
	TEST_EQUALS((result),(storeGetSize(store)),(3));

	storeDestroy(store);
	return result;
}
//-----------------------------------------------------------------------------
static bool storeRemoveItemTest() {

	bool result = true;
	Store store = storeCreate();

	TEST_EQUALS((result),(storeRemoveItem(NULL,"potion",50)),
												(STORE_NULL_ARGUMENT));
	TEST_EQUALS((result),(storeRemoveItem(store,"rami",50)),
												(STORE_INVALID_ARGUMENT));
	TEST_EQUALS((result),(storeRemoveItem(store,"potion",0)),
												(STORE_INVALID_ARGUMENT));
	TEST_EQUALS((result),(storeRemoveItem(store,"potion",-1000)),
												(STORE_INVALID_ARGUMENT));
	TEST_EQUALS((result),(storeRemoveItem(store,"potion",30)),
												(STORE_ITEM_OUT_OF_STOCK));
	TEST_EQUALS((result),(storeRemoveItem(store,"itzik",30)),
												(STORE_INVALID_ARGUMENT));
	//STORE_INVALID_ARGUMENT is stronger than STORE_ITEM_OUT_OF_STOCK
	storeAddItem(store,"potion",30,1);
	TEST_EQUALS((result),(storeRemoveItem(store,"potion",30)),
												(STORE_SUCCESS));
	TEST_EQUALS((result),(storeRemoveItem(store,"potion",30)),
												(STORE_ITEM_OUT_OF_STOCK));

	storeDestroy(store);
	return result;
}
//-----------------------------------------------------------------------------
static bool storeShowStockTest() {

	bool result = true;
	TEST_EQUALS((result),(storeShowStock(NULL,stdout)),
												(STORE_NULL_ARGUMENT));

	/* the working options where tested manually on files including
	 * STORE_SUCCESS return value
	 * */

	return result;
}
//-----------------------------------------------------------------------------

int main() {

	RUN_TEST(storeCreateTest);		//storeDestroy is tested with valgrind
	RUN_TEST(storeGetSizeTest);
	RUN_TEST(storeAddItemTest);
	RUN_TEST(storeRemoveItemTest);
	RUN_TEST(storeShowStockTest);

}
