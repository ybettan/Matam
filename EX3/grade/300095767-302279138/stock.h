#ifndef STOCK_H_
#define STOCK_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct stock_t* Stock;

typedef enum{
	 STOCK_OUT_OF_MEM,
	 STOCK_NULL_ARGUMENT,
	 STOCK_INVALID_ARGUMENT,
	 STOCK_ITEM_OUT_OF_STOCK,
	 STOCK_NO_AVAILABLE_ITEM_FOUND,
	 STOCK_SUCCESS
}StockResult;


/* creates new empty stock.
 * ----return-----
 * return a pointer to stock struct
 * NULL if any allocation failed.
 */

Stock stockCreate();


/* free all elemnts kept in stock and free stock's memory.
 * if given NULL do nothing.
 */

void stockDestroy(Stock stock);


/* returns the number of elements in stock
 * ----Return-----
 * an integer >= 0
 * -1 if stock is NULL
 */

int stockGetSize(Stock stock);


/* given item with type "candy" or "potion" with value and quantity,
 * tries adding to stock the amount specified.
 * ----return-----
 * STOCK_OUT_OF_MEM - if any allocation for items failed.
 * STOCK_NULL_ARGUMENT - if stock or type are NULL.
 * STOCK_INVALID_ARGUMENT- if type isn't "potion" or "candy",
 * 													value <=0 or quantity<=0.
 * STOCK_SUCCESS - if the items stored successfully.
 */

StockResult stockAddItem(Stock stock,const char* type,int value,int quantity);


/* creates a new copy of stock
 * -----return-----
 * its pointer
 * NULL if stock == NULL or memory allocation failed
 */

Stock stockCopy(Stock stock);


/* given type "candy" or "potion" stock remove singel item with the given
 * value.
 * ----return-----
 * STOCK_NULL_ARGUMENT - if stock or type are NULL.
 * STOCK_INVALID_ARGUMENT - if type isn't "potion" or "candy" or value <= 0 .
 * STOCK_ITEM_OUT_OF_STOCK - if stock has no item as given.
 * STOCK_SUCCESS - if successful.
 */

StockResult stockRemoveItem(Stock stock, const char* type, int value);


/* given an output file (a file or stdout) for printing, the function prints
 * all its capacity as follows:
 * 		- all the candies in increasing order
 *		- all the potions in increasing order.
 * assuming with assert that output_file != NULL
 * the function will print the header for stock if and only if
 * print_header == true
 * -----Return------
 * STOCK_NULL_ARGUMENT if stock is NULL.
 * STOCK_SUCCESS if succeed.
 */

StockResult stockShowInfo(Stock stock,FILE* output_file,bool print_header);


/* the function recives the required value and returns the minimal "potion"
 * so that value_found >= value_required. if there is no such potion it will
 * return the potion with maximal value .
 *  the found value is written where value_fount point to.
 * ----return-----
 * STOCK_NULL_ARGUMENT - if stock or value_found are NULL.
 * STOCK_INVALID_ARGUMENT - if value_required <= 0
 * STOCK_NO_AVAILABLE_ITEM_FOUND - if stock doesn't contain any potions.
 * STOCK_SUCCESS - if an appropriate potion was found.
 */
StockResult stockFindBestPotion(Stock stock,int value_required,
															int* value_found);

/* the function finds the most valuable candy in the stock. if found it
 * writes the value to a given pointer.
 * ----return-----
 * STOCK_NULL_ARGUMENT - if stock or value_found are NULL.
 * STOCK_NO_AVAILABLE_ITEM_FOUND - if stock doesn't contain any candy.
 * STOCK_SUCCESS - if found most valuable candy.
 */
StockResult stockFindMostValuableCandy(Stock stock, int* value_found);


#endif /* STOCK_H_ */


