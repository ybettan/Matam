#include <stdlib.h>

#include "../map.h"
#include "../test_utilities.h"

#define INT_A 65
#define INT_B 66
#define INT_Z 90

/** Function to be used for copying an int as a key to the map */

static MapKeyElement copyKeyInt(MapKeyElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*)n;
	return copy;
}

/** Function to be used for copying a char as a data to the map */

static MapDataElement copyDataChar(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	char* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(char*)n;
	return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */

static void freeInt(MapKeyElement n) {
	free(n);
}

/** Function to be used by the map for freeing elements */

static void freeChar(MapDataElement n) {
	free(n);
}

/** Function to be used by the map for comparing elements */

static int compareInts(MapKeyElement n1, MapKeyElement n2) {
	int* int_1 = (int*)n1;
	int* int_2 = (int*)n2;
	if (*int_1 > *int_2) {
    return 1;
	} else if (*int_2 > *int_1) {
    return -1;
	} else {
		return 0;
	}
}


bool testMapCreateDestroy() {
	Map map1 = mapCreate(NULL, copyDataChar, freeInt, freeChar, compareInts);
	ASSERT_TEST(map1 == NULL);
	Map map2 = mapCreate(copyKeyInt, NULL, freeInt, freeChar, compareInts);
	ASSERT_TEST(map2 == NULL);
	Map map3 = mapCreate(copyKeyInt, copyDataChar, NULL, freeChar,
			compareInts);
	ASSERT_TEST(map3 == NULL);
	Map map4 = mapCreate(copyKeyInt, copyDataChar, freeInt, NULL,
			compareInts);
	ASSERT_TEST(map4 == NULL);
	Map map5 = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, NULL);
	ASSERT_TEST(map5 == NULL);

	Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar,
			compareInts);
	ASSERT_TEST(map != NULL);
	ASSERT_TEST(mapGetSize(map) == 0);
	ASSERT_TEST(mapGetFirst(map) == NULL);
	ASSERT_TEST(mapGetNext(map) == NULL);
	mapDestroy(map);
	return true;

}

bool testMapPutRemoveFirstSize(){//a check for putting a pair of an
	                             //existing key and a data is on
	                             //"testMapPutContainsGet"
	Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar,
			compareInts);
	ASSERT_TEST(mapGetSize(map) == 0);
	ASSERT_TEST(mapGetSize(NULL) == -1);
	ASSERT_TEST(mapGetFirst(map) == NULL);
	ASSERT_TEST(mapGetFirst(NULL) == NULL);

	int counter = 0;
	char letter = (char) counter;
	int not_in_map = INT_Z+1;
	int* element_before_put;
	int* element_after_put;
	int* element_before_remove;
	int* element_after_remove;

	ASSERT_TEST(mapPut(NULL,&counter,&letter) == MAP_NULL_ARGUMENT);
	ASSERT_TEST(mapPut(map,NULL,&letter) == MAP_NULL_ARGUMENT);
	ASSERT_TEST(mapPut(map,&counter,NULL) == MAP_NULL_ARGUMENT);

	for(int i = INT_Z; i >= INT_A; i--){//inserts from (int)'Z' till (int)'A'
		                                //so in each iteration mapGetFirst is
		                                //different.
		counter++;
		char j = (char)i;
		if(mapGetSize(map) != 0){
			element_before_put = (int*)mapGetFirst(map);
			ASSERT_TEST(*element_before_put == i+1);
		}
		ASSERT_TEST(mapGetSize(map) != counter);
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
		ASSERT_TEST(mapGetNext(map) == NULL);//iterator not available
		ASSERT_TEST(mapGetSize(map) == counter);
		element_after_put = (int*)mapGetFirst(map);
		ASSERT_TEST(*element_after_put == i);
	}

	for(int i = INT_A; i <= INT_Z; i++){//removes from (int)'A' till (int) 'Z'
		                                //so in each iteration mapGetFirst is
		                                //different.

		element_before_remove = (int*)mapGetFirst(map);
		ASSERT_TEST(*element_before_remove == i);
		ASSERT_TEST(mapGetSize(map) == INT_Z - (*element_before_remove) +1);
		ASSERT_TEST(*element_before_remove == i);
		ASSERT_TEST(mapRemove(NULL,&not_in_map) == MAP_NULL_ARGUMENT);
		ASSERT_TEST(mapRemove(map,NULL) == MAP_NULL_ARGUMENT);
		ASSERT_TEST(mapRemove(map,&not_in_map) == MAP_ITEM_DOES_NOT_EXIST);
		ASSERT_TEST(mapRemove(map,&i) == MAP_SUCCESS);
		ASSERT_TEST(mapGetNext(map) == NULL);//iterator not available


		element_after_remove = (int*)mapGetFirst(map);

		if(mapGetSize(map) != 0){
			ASSERT_TEST(*element_after_remove == i+1);
		} else {
			ASSERT_TEST(element_after_remove == NULL);
		}
	}


	counter = 0;
	ASSERT_TEST(mapGetSize(map) == 0);



	for(int i = INT_A; i <= INT_Z; i++){//inserts from (int)'A' till (int)'Z'
			                            //so in each iteration mapGetFirst is
			                            //same.
		counter++;
		char j = (char)i;
		if(mapGetSize(map) != 0){
			element_before_put = (int*)mapGetFirst(map);
			ASSERT_TEST(*element_before_put == INT_A);
		}
		ASSERT_TEST(mapGetSize(map) != counter);
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
		ASSERT_TEST(mapGetSize(map) == counter);
		element_after_put = (int*)mapGetFirst(map);
		ASSERT_TEST(*element_after_put == INT_A);
	}

	for(int i = INT_Z; i >= INT_A; i--){//removes from (int)'Z' till (int) 'A'
                                        //so in each iteration mapGetFirst is
                                        //same.

		element_before_remove = (int*)mapGetFirst(map);
		ASSERT_TEST(*element_before_remove == INT_A);
		ASSERT_TEST(mapGetSize(map) == i - (*element_before_remove) +1);
		ASSERT_TEST(mapRemove(NULL,&not_in_map) == MAP_NULL_ARGUMENT);
		ASSERT_TEST(mapRemove(map,NULL) == MAP_NULL_ARGUMENT);
		ASSERT_TEST(mapRemove(map,&not_in_map) == MAP_ITEM_DOES_NOT_EXIST);
		ASSERT_TEST(mapRemove(map,&i) == MAP_SUCCESS);

		element_after_remove = (int*)mapGetFirst(map);

		if(mapGetSize(map) != 0){
			ASSERT_TEST(*element_before_remove == *element_after_remove);
		} else {
			ASSERT_TEST(element_after_remove == NULL);
		}
	}
	mapDestroy(map);
	return true;
}



bool testMapPutContainsGet() {
	Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar,
			compareInts);
	int one_int = INT_A;
	char one_char = (char)one_int;
	int two_int = INT_B;
	char two_char = (char)two_int;
	char* a;
	int i,j;

/** tests MapPut's replacement of same key . */
	ASSERT_TEST(mapPut(map, &one_int, &one_char) == MAP_SUCCESS);
	ASSERT_TEST(mapPut(map, &one_int, &one_char) == MAP_SUCCESS);
	ASSERT_TEST(mapPut(map, &one_int, &two_char) == MAP_SUCCESS);
	ASSERT_TEST(mapPut(map, &one_int, &one_char) == MAP_SUCCESS);

	ASSERT_TEST(mapGetSize(map) == 1);
	a = (char*)mapGet(map, &one_int);
	ASSERT_TEST(*a == 'A');

	ASSERT_TEST(mapPut(map, &one_int, &two_char) == MAP_SUCCESS);
	ASSERT_TEST(mapGetSize(map) == 1);
	a = (char*)mapGet(map, &one_int);
	ASSERT_TEST(*a == 'B');


	ASSERT_TEST(mapGet(NULL, &one_int) == NULL);
	ASSERT_TEST(mapGet(map, NULL) == NULL);
	ASSERT_TEST(mapGet(map, &two_int) == NULL);

	ASSERT_TEST(mapPut(map, &one_int, &two_char) == MAP_SUCCESS);
	ASSERT_TEST(mapGetSize(map) == 1);
	a = (char*)mapGet(map, &one_int);
	ASSERT_TEST(*a != 'A');
	ASSERT_TEST(*a == 'B');

	ASSERT_TEST(mapPut(map, &one_int, &one_char) == MAP_SUCCESS);
	ASSERT_TEST(mapPut(map, &two_int, &two_char) == MAP_SUCCESS);
	ASSERT_TEST(mapGetSize(map) == 2);
	int* x = (int*)mapGetFirst(map);
	ASSERT_TEST(*x == one_int);
	a = (char*)mapGet(map, &one_int);
	x = (int*)mapGetNext(map);//tests iterator still available after mapGet.
	ASSERT_TEST(*x == two_int);
	x = (int*)mapGetFirst(map);
	ASSERT_TEST(*x == one_int);
	ASSERT_TEST(mapContains(NULL, x) == false);
	ASSERT_TEST(mapContains(map, NULL) == false);
	bool y = mapContains(map, x);
	ASSERT_TEST(y == true);
	ASSERT_TEST(mapGetNext(map) == NULL);//iterator not available after mapContains


	mapDestroy(map);


/** tests mixing ordered keys insert and MapPut's replacement of same key. */

	Map crazy_map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar,
			compareInts);

	for(i = 3; i != 0 ; i = (i+3)%100){
		j = (char) i;
		ASSERT_TEST(mapPut(crazy_map,&i,&j) == MAP_SUCCESS);
	}
	ASSERT_TEST(mapGetSize(crazy_map) == 99);

	int counter = 1;
	MAP_FOREACH(int*,iterator,crazy_map){
		ASSERT_TEST(*iterator == counter);
		ASSERT_TEST(*(char*)mapGet(crazy_map, iterator) == (char)counter);
		counter++;
	}




	for(i = 3; i != 0; i = (i+3)%100){
		j = (char) i+100;
		one_char = (char) i; //same as before
		ASSERT_TEST(mapPut(crazy_map, &i, &j) == MAP_SUCCESS);
		ASSERT_TEST(*(char*)mapGet(crazy_map, &i) != one_char);
	}

	MAP_FOREACH(int*,iterator,crazy_map){
		int* checker = copyKeyInt(iterator);
		bool before = mapContains(crazy_map, checker);
		ASSERT_TEST(mapRemove(crazy_map, iterator) == MAP_SUCCESS);
		bool after = mapContains(crazy_map, checker);
		ASSERT_TEST(before != after);
		freeInt(checker);
	}



	mapDestroy(crazy_map);

return true;
}


bool testMapRemoveCopyClear() {

	Map map1 = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, compareInts);
	for(int i = 9; i != 0 ; i = (i+9)%100){
		char j = (char) i;
		ASSERT_TEST(mapPut(map1, &i, &j) == MAP_SUCCESS);
	}
	ASSERT_TEST(mapCopy(NULL) == NULL);
	ASSERT_TEST(mapGetFirst(map1) != NULL);//sets iterator
	Map map1_copy = mapCopy(map1);
	ASSERT_TEST(mapGetNext(map1) == NULL);
	ASSERT_TEST(map1_copy != NULL);
	ASSERT_TEST(mapGetNext(map1_copy) == NULL);



	for(int i=1; i<=99; i++){
		int* a = (int*)mapGetFirst(map1);
		int* b = (int*)mapGetFirst(map1_copy);
		ASSERT_TEST(*a == *b);
		ASSERT_TEST(mapRemove(map1, &i) == MAP_SUCCESS);
		ASSERT_TEST(mapRemove(map1_copy, &i) == MAP_SUCCESS);
	}
	ASSERT_TEST(mapGetSize(map1) == mapGetSize(map1_copy));
	ASSERT_TEST(mapGetSize(map1) == 0);

	for(int i = 9; i != 0 ; i = (i+9)%100){
		char j = (char) i;
		ASSERT_TEST(mapPut(map1, &i, &j) == MAP_SUCCESS);
	}
	ASSERT_TEST(mapGetSize(map1) == 99);
	ASSERT_TEST(mapClear(NULL) == MAP_NULL_ARGUMENT);
	ASSERT_TEST(mapClear(map1) == MAP_SUCCESS);
	ASSERT_TEST(mapGetSize(map1) == 0);
	ASSERT_TEST(mapGetNext(map1) == NULL);
	ASSERT_TEST(mapGetFirst(map1) == NULL);
	ASSERT_TEST(mapGetSize(map1) == mapGetSize(map1_copy));

	Map map2 = mapCopy(map1);
	ASSERT_TEST(map2 !=NULL);
	mapDestroy(map1);
	mapDestroy(map1_copy);
	mapDestroy(map2);
	return true;
}


bool testMapNextIterator() {
	Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar,
			compareInts);

	for (int i = 1; i < 400; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}

	for (int i = 800; i >= 400; --i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}

	for (int i = 801; i < 1000; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}

	int i = 1;
	MAP_FOREACH(MapKeyElement,iter,map) {
		ASSERT_TEST(*(int*)iter == i++);
	}


	ASSERT_TEST(mapClear(map) == MAP_SUCCESS);
	for(i = 3; i != 0 ; i = (i+3)%100){
		char j = (char) i;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	ASSERT_TEST(mapGetSize(map) == 99);

	while(mapGetSize(map) >= 3){
		mapRemove(map, mapGetFirst(map));/////////////////what about iterator?
		ASSERT_TEST(mapGetNext(map)== NULL);
		mapGetFirst(map);
		int* next = (int*)mapGetNext(map);
		int* first = (int*)mapGetFirst(map);
		ASSERT_TEST(*next - *first == 1);
	}
	mapDestroy(map);
	return true;
}


int main() {

	RUN_TEST(testMapCreateDestroy);
	RUN_TEST(testMapPutRemoveFirstSize);
	RUN_TEST(testMapPutContainsGet);
	RUN_TEST(testMapRemoveCopyClear);
	RUN_TEST(testMapNextIterator);

	return 0;
}

