#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "map_mtm.h"

/**
 * Evaluates b and continues if b is true.
 * If b is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT(b,map) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s\n",__FILE__,__LINE__,#b); \
                mapDestroy(map);\
                return false; \
        } \
} while (0)

#define ASSERT2(b,map1,map2) do { \
		if (!(b)) {\
			mapDestroy(map1);\
			ASSERT(false,map2);\
		}\
} while (0)

/**
 * Macro used for running a test from the main function
 */
#define TEST(success,test) do { \
        printf("Running "#test"... "); \
        if (test()) { \
                printf("[OK]\n");\
                ++success;\
        } \
} while(0)

/** Function to be used for copying an int to the map */
static MapKeyElement copyInt(MapKeyElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*)n;
	return (MapKeyElement) copy;
}

/** Function to be used for copying a char to the map */
static MapDataElement copyChar(MapDataElement n) {
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

/** Function to be used for copying a char to the map */
static MapDataElement copyString(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	char* copy = malloc(strlen(n)+1);
	if (copy)
		return strcpy(copy,n);
	return copy;
}

/** Function to be used for copying a strings to the map */
static MapDataElement copyIntPtr(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	int** orig = n;
	int** copy = malloc(sizeof(*copy));
	if (!copy)
		return NULL;
	*copy = malloc(sizeof(**copy));
	if (!*copy){
		free(copy);
		return NULL;
	}
	**copy = **orig;

	return copy;
}

/** Function to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
	free(n);
}

/** Function to be used by the map for freeing elements */
static void freeChar(MapDataElement n) {
	free(n);
}

/** Function to be used by the map for freeing elements */
static void freeString(MapDataElement n) {
	free(n);
}

/** Function to be used by the map for freeing elements */
static void freeIntPtr(MapDataElement n) {
	int** orig = n;
	free(*orig);
	free(orig);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
	int* a = n1;
	int* b = n2;
	if (*a < *b) {
    return 1;
	} else if (*b < *a) {
    return -1;
	}
	return 0;
}

/** Function to be used by the map for comparing elements */
static int compareStrings(MapKeyElement n1, MapKeyElement n2) {
	if (strcmp((char*)n1, (char*)n2) > 0) {
    return 1;
	} else if (strcmp((char*)n1, (char*)n2) < 0) {
    return -1;
	}
	return 0;
}

bool testMapCreateDestroy() {
	mapDestroy(NULL);
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	ASSERT(map != NULL,map);
	ASSERT(mapGetSize(map) == 0,map);
	ASSERT(mapGetFirst(map) == NULL,map);
	mapDestroy(map);
	return true;

}

bool testMapAddAndSize() {
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	for (int i=800; i<1000;++i){
		char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 799,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 199,map);
	}
	for (int i=0; i<400;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i + 601,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == 1000,map);
	}
	ASSERT(mapGetSize(NULL) == -1,map);
	mapDestroy(map);
	return true;
}

bool testMapGet() {
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	for (int i=800; i<1000;++i){
		char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 799,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 199,map);
	}
	for (int i=0; i<400;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i + 601,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == 1000,map);
	}
	for (int i=1; i<1000;++i){
		char j = (char) i;
		++j;
		char* getVal = (char*)mapGet(map,&i);
		ASSERT(getVal != NULL,map);
		char val = *getVal;
		ASSERT(val == j,map);

		getVal = (char*)mapGet(map,&i);
		ASSERT(getVal != NULL,map);
		val = *getVal;
		ASSERT(val == j,map);
	}
	int i = 1000;
	ASSERT(mapGet(map,&i) == NULL,map);
	mapDestroy(map);
	return true;
}

bool testMapReplace(){
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	char j = 1;
	int key = 0;
	ASSERT(mapPut(map,&key,&j) == MAP_SUCCESS,map);
	for (int i=1; i<1000;++i){
		j = (char) i;
		key = 0;
		++j;
		ASSERT(mapPut(map,&key,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == 1,map);
		char* getVal = (char*)mapGet(map,&key);
		ASSERT(getVal != NULL,map);
		char val = *getVal;
		ASSERT(val == j,map);
	}
	mapDestroy(map);
	return true;
}

bool testIterator() {
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);

	ASSERT(mapGetFirst(NULL) == NULL, map);
	ASSERT(mapGetFirst(map) == NULL, map);

  for (int i=800; i<1000;++i){
		char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 799,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i - 199,map);
	}
	for (int i=0; i<400;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == i + 601,map);
	}
	for (int i=400; i<800;++i) {
    char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
		ASSERT(mapGetSize(map) == 1000,map);
	}

  int i = 1000;
	MAP_FOREACH(int*,iter,map) {
		ASSERT(*(int*)iter == --i,map);
	}

	mapDestroy(map);
	return true;
}


bool testRemoveContains() {
	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	int key = 29;
	ASSERT(mapContains(map,&key) == false,map);
	ASSERT(mapRemove(NULL,&key) == MAP_NULL_ARGUMENT,map);
	ASSERT(mapRemove(map,NULL) == MAP_NULL_ARGUMENT,map);
	ASSERT(mapRemove(map,&key) == MAP_ITEM_DOES_NOT_EXIST,map);

	for (int i=1; i<1000;++i){
		char j = (char) i;
		++j;
		ASSERT(mapPut(map,&i,&j) == MAP_SUCCESS,map);
	}

	for (int i=10; i<1000; i*=2){
		int size = mapGetSize(map);
		ASSERT(size > 0, map);
		ASSERT(mapContains(map,&i) == true,map);
		ASSERT(mapRemove(map,&i) == MAP_SUCCESS,map);
		ASSERT(mapContains(map,&i) == false,map);
		ASSERT(mapRemove(map,&i) == MAP_ITEM_DOES_NOT_EXIST,map);
		ASSERT(size-1 == mapGetSize(map), map);
	}

	mapDestroy(map);
	return true;
}

bool testCopyClear() {
	Map copy = mapCopy(NULL);
	ASSERT(copy == NULL, copy);

	Map map = mapCreate(copyInt, copyChar, freeInt, freeChar, compareInts);
	copy = mapCopy(map);
	ASSERT(copy != NULL, map);
	ASSERT2(mapGetSize(map) == 0, map,copy);
	ASSERT2(mapGetSize(copy) == 0, map,copy);

	for (int i=1; i<4;++i){
		char j = (char) i;
		++j;
		ASSERT2(mapPut(map,&i,&j) == MAP_SUCCESS,map,copy);
		ASSERT2(mapGetSize(copy) == 0, map,copy);
		ASSERT2(mapGetSize(map) == i,map,copy);
	}
	mapDestroy(copy);
	ASSERT(mapGetSize(map) == 3, map);

	int key = 2;
	char data = (char) key;
	++data;
	char *getData = (char*)mapGet(map,&key);
	ASSERT(getData != NULL, map);
	char val = *getData;
	ASSERT(val == data,map);

	copy = mapCopy(map);
	ASSERT(copy != NULL, map);
	ASSERT2(mapGetSize(copy) == 3, map,copy);

	ASSERT2(mapRemove(copy, &key) == MAP_SUCCESS, map,copy);
	ASSERT2(mapGetSize(copy) == 2, map,copy);
	ASSERT2(mapGetSize(map) == 3, map,copy);
	getData = (char*)mapGet(map,&key);
	ASSERT2(getData != NULL, map,copy);
	val = *getData;
	ASSERT2(val == data,map,copy);

	ASSERT2(mapClear(map) == MAP_SUCCESS, map,copy);
	ASSERT2(mapGetSize(copy) == 2, map,copy);
	ASSERT2(mapGetSize(map) == 0, map,copy);

	ASSERT2(mapGetFirst(map) == NULL, map,copy);
	int *keyPtr = mapGetFirst(copy);
	ASSERT2(keyPtr != NULL, map,copy);
	key = *keyPtr;
	data = (char) key;
	++data;
	getData = (char*)mapGet(copy,&key);
	ASSERT2(getData != NULL, map, copy);
	val = *getData;
	ASSERT2(val == data,map, copy);

	mapDestroy(copy);
	mapDestroy(map);

	return true;
}

bool testGenerality() {
	Map map = mapCreate(copyString, copyIntPtr, freeString, freeIntPtr, compareStrings);
	char* strings[4] = {"we", "will", "test", "you"};
	int lengths[4] = {2,4,4,3};
	int* pointers[4];
	for (int i=0; i<4; ++i)
		pointers[i] = lengths+i;

	for (int i=0; i<4;++i)
		ASSERT(mapPut(map,strings[i],pointers+i) == MAP_SUCCESS,map);

	int order[4] = {3,1,0,2};
	for (int i=0; i<4;++i){
		char* key = strings[order[i]];
		int** getDataPtr = mapGet(map,key);
		ASSERT(getDataPtr != NULL && *getDataPtr != NULL, map);
		int data = **getDataPtr;
		ASSERT(data == lengths[order[i]],map);
	}

	mapDestroy(map);
	return true;
}

int main(int argc, char *argv[]) {
	char* c = argv[1];
	int successful = 0;
	switch ( *c ) {
	case '0': {
		TEST(successful,testMapCreateDestroy);
		TEST(successful,testMapAddAndSize);
		TEST(successful,testMapGet);
		TEST(successful,testMapReplace);
		TEST(successful,testIterator);
		TEST(successful,testRemoveContains);
		TEST(successful,testCopyClear);
		TEST(successful,testGenerality);
		break;
	}
	case '1':
		TEST(successful,testMapCreateDestroy);
		break;
	case '2':
		TEST(successful,testMapAddAndSize);
		break;
	case '3':
		TEST(successful,testMapGet);
		break;
	case '4':
		TEST(successful,testMapReplace);
		break;
	case '5':	
		TEST(successful,testIterator);
		break;
	case '6':
		TEST(successful,testRemoveContains);
		break;
	case '7':
		TEST(successful,testCopyClear);
		break;
	case '8':
		TEST(successful,testGenerality);
		break;
	}
	return 0;
}
