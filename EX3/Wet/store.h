#ifndef STORE_STORE_H_
#define STORE_STORE_H_

#include <stdio.h>
#include <stdbool.h>

#include "stock.h"

typedef struct store_t* Store;

typedef enum{
	 STORE_OUT_OF_MEM,
	 STORE_NULL_ARGUMENT,
	 STORE_INVALID_ARGUMENT,
	 STORE_ITEM_OUT_OF_STOCK,
	 STORE_SUCCESS
}StoreResult;


/* creates new empty store.
 * ----return-----
 * return a pointer to struct store
 * NULL if any allocation failed.
 */
Store storeCreate();

/* free all elemnts kept in store and free store's memory.
 * if given NULL do nothing.
 */
void storeDestroy(Store store);

/* returns the number of elements in store
 * ----Return-----
 * an integer >= 0
 * -1 if store is NULL
 */
int storeGetSize(Store store);	//good function for debugging

/* given item with type "candy" or "potion" with value and quantity,
 * tries adding to store the amount specified.
 * ----return-----
 * STORE_OUT_OF_MEM - if any allocation for items failed.
 * STORE_NULL_ARGUMENT - if store or type are NULL.
 * STORE_INVALID_ARGUMENT- if type isn't "potion" or "candy",
 * value <=0 or quantity<=0.
 * STORE_SUCCESS - if the items stored successfully.
 */
StoreResult storeAddItem(Store store,const char* type,int value,int quantity);

/* given type "candy" or "potion" store try to sell singel item with the given
 * value.
 * ----return-----
 * STORE_NULL_ARGUMENT - if store or type are NULL.
 * STORE_INVALID_ARGUMENT - if type isn't "potion" or "candy" or
 * 																if value <= 0 .
 * STORE_ITEM_OUT_OF_STOCK - if store has no item as given.
 * STORE_SUCCESS - if the selling was successful.
 */
StoreResult storeRemoveItem(Store store, const char* type, int value);

/* given an output file (a file or stdout) for printing, the function prints
 * all its capacity as follows:
 * 		- all the candies in increasing order
 *		- all the potions in increasing order.
 * assuming with assert that output_file != NULL
 * -----Return------
 * STORE_NULL_ARGUMENT if store is NULL
 * STORE_SUCCESS if succeed
 */
StoreResult storeShowStock(Store store,FILE* output_file);


#endif /* STORE_STORE_H_ */
