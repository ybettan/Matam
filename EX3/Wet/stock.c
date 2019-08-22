#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define NDEBUG
#include <assert.h>

#include "print_utils.h"
#include "map.h"
#include "stock.h"


//=============================================================================
//                              MACROS
//=============================================================================


#define MAX_TYPE_LEN 10
#define NEGATIVE_NUM -1
#define POSITIVE_NUM 1


//=============================================================================
//                              STRUCT
//=============================================================================


//stock struct is a map containing items

typedef struct item_t {
    char* type;
    int value;
} *Item ;

struct stock_t {
    Map map;
};


//=============================================================================
//                      STATIC ITEM CREATE/DESTROY
//=============================================================================


/* create a new item with "type" as type and "value" as value
 * -----Return------
 * a pointer to item
 * NULL if malloc failed
 */

static Item itemCreate(const char* type,int value) {

    assert (strcmp(type,"potion") == 0   ||   strcmp(type,"candy") == 0);
    assert(value > 0);

    Item item = malloc( sizeof(*item) );
    if (item == NULL)   return NULL;

    item->type = malloc( sizeof(*item->type) * MAX_TYPE_LEN );
    if (item->type == NULL) {
        free(item);
        return NULL;
    }

    strcpy(item->type,type);
    item->value = value;

    return item;
}


//release the memory allocated for item

static void itemDestroy(Item item) {

    if (item != NULL) {
        free(item->type);
    }
    free(item);
}


//=============================================================================
//              CMP,COPY,DESTROY FUNCTIONS FOR MAP USE
//=============================================================================


/*create a copy of data_element
 * -----return-----
 * its poiter or NULL if malloc failed
 */

static MapDataElement intCopyForMap (MapDataElement data_element) {

    int* data_element_copy = malloc( sizeof(*data_element_copy) );
    if (data_element_copy == NULL)  return NULL;

    *data_element_copy = *(int*)data_element;

    return (MapDataElement)data_element_copy;
}


//free the memory allocated for an integer

static void intDestroyForMap(MapDataElement data_element) {

    free( (int*)data_element );
}


/*create a copy of key_element
 * -----return-----
 * its poiter or NULL if malloc failed
 */

static MapKeyElement itemCopyForMap(MapKeyElement key_element) {

    assert(key_element != NULL);

    Item key_element_copy = itemCreate(  ((Item)key_element)->type,
                                         ((Item)key_element)->value  );

    if ( key_element_copy == NULL )     return NULL;

    strcpy( key_element_copy->type , ((Item)key_element)->type );
    key_element_copy->value = ((Item)key_element)->value;

    return (MapKeyElement)key_element_copy;
}


//free the memory allocated for an item

static void itemDestroyForMap(MapKeyElement key_element) {

    itemDestroy( (Item)key_element );
}


/*
* Type of function used by the map to identify equal key elements.
* This function should return:
*       1 if the first element is greater;
*       0 if they're equal;
*      -1 if the second element is greater.
*
* meaning of greater:
*   - "potion" is greater than "candy"
*   - greater value according greater integer
*/

static int itemCmpForMap(MapKeyElement item1,MapKeyElement item2) {

	 assert(item1 != NULL   &&   item2 != NULL);

	 int type_cmp = strcmp( ((Item)item1)->type, ((Item)item2)->type );
	 if(type_cmp) {
		 return type_cmp > 0 ? POSITIVE_NUM : NEGATIVE_NUM;
	 }
	 int value_cmp = ((Item)item1)->value - ((Item)item2)->value;
	 if (!value_cmp) return 0;
	 return value_cmp > 0 ? POSITIVE_NUM : NEGATIVE_NUM;
}

//=============================================================================
//                          HEADER FUNCTION
//=============================================================================


//the function pointers can never be NULL (also according to compiller)

Stock stockCreate() {

    Stock stock = malloc( sizeof(*stock) );
    if (stock == NULL)  return NULL;

    Map map =           mapCreate(      itemCopyForMap,
                                        intCopyForMap,
                                        itemDestroyForMap,
                                        intDestroyForMap,
                                        itemCmpForMap       );
    if (map == NULL) {
        free(stock);
        return NULL;
    }
    stock->map = map;

    return stock;
}
//-----------------------------------------------------------------------------
void stockDestroy(Stock stock) {

    if (stock != NULL) {
        mapDestroy(stock->map);
    }
    free(stock);
}
//-----------------------------------------------------------------------------
int stockGetSize(Stock stock) {

    if (stock == NULL)  return NEGATIVE_NUM;

    return mapGetSize(stock->map);
}
//-----------------------------------------------------------------------------
StockResult stockAddItem(Stock stock,const char* type,int value,int quantity) {

    if ( stock == NULL  || type == NULL ) return STOCK_NULL_ARGUMENT;
    if(
            value <= 0     ||
            quantity <= 0  ||
            (strcmp(type,"potion") != 0  &&  strcmp(type,"candy") != 0) ) {
        return STOCK_INVALID_ARGUMENT;
    }
    Item item = itemCreate(type,value);
    if (item == NULL)   return STOCK_OUT_OF_MEM;

    bool is_in_stock = mapContains(stock->map,item);         //if false it is
    MapResult result;                                       //because not exist
    if (! is_in_stock ) {
        result =
            mapPut(stock->map,(MapKeyElement)item,(MapDataElement)&quantity);
    }else{
        int new_quantity = *(int*)mapGet(stock->map,item) + quantity;
        result =
        mapPut(stock->map,(MapKeyElement)item,(MapDataElement)&new_quantity);
    }
    itemDestroy(item);      // the map take a copy

    if (result == MAP_OUT_OF_MEMORY) {
        return STOCK_OUT_OF_MEM;
    }else{
        assert(result == MAP_SUCCESS);
        return STOCK_SUCCESS;
    }
}
//-----------------------------------------------------------------------------
StockResult stockRemoveItem(Stock stock,const char* type, int value) {
    if (stock == NULL  || type == NULL) return STOCK_NULL_ARGUMENT;
    if (      value <= 0     ||
            (strcmp(type,"potion") != 0  &&  strcmp(type,"candy") != 0) ) {
        return STOCK_INVALID_ARGUMENT;
    }
    Item item = itemCreate(type,value);
    if (item == NULL)   return STOCK_OUT_OF_MEM;
    if (! mapContains(stock->map,item) ) {
        itemDestroy(item);
        return STOCK_ITEM_OUT_OF_STOCK;
    }
    int new_quantity = *(int*)mapGet(stock->map,item) - 1;
    MapResult result1 =
        mapPut(stock->map,(MapKeyElement)item,(MapDataElement)&new_quantity);
    if ( *(int*)mapGet(stock->map,item) <= 0 ) {

        #ifndef NDEBUG
        MapResult result2 =
        #endif
        mapRemove(stock->map,item);
        assert(result2 == MAP_SUCCESS);
    }
    itemDestroy(item);
    if (result1 == MAP_OUT_OF_MEMORY) {
        return STOCK_OUT_OF_MEM;
    }else{
        assert(result1 == MAP_SUCCESS);
        return STOCK_SUCCESS;
    }
}
//-----------------------------------------------------------------------------
Stock stockCopy(Stock stock){

	if (stock == NULL) return NULL;

	Stock stock_copy = malloc(sizeof(*stock_copy));
	if (stock_copy == NULL)	return NULL;

	stock_copy->map = mapCopy(stock->map);
	if(stock_copy->map == NULL) {
		free(stock_copy);
		return NULL;
	}
	return stock_copy;

}
//-----------------------------------------------------------------------------
StockResult stockShowInfo(Stock stock,FILE* output_file,bool print_header) {

    assert(output_file != NULL);
    if (stock == NULL)  return STOCK_NULL_ARGUMENT;

    if(print_header){
    	mtmPrintStockHeader(output_file);
    }
    int quantity;

    MAP_FOREACH(Item,item,stock->map) {

        quantity = *(int*)mapGet(stock->map,(MapKeyElement)item);
        mtmPrintItem(output_file,item->type,item->value,quantity);
    }
    return STOCK_SUCCESS;
}
//-----------------------------------------------------------------------------
StockResult stockFindBestPotion(Stock stock,int value_required,
                                                    int* value_found) {

    if (stock == NULL  ||  value_found == NULL) return STOCK_NULL_ARGUMENT;
    if ( value_required <= 0) {
        return STOCK_INVALID_ARGUMENT;
    }
    int current_value = NEGATIVE_NUM;

    MAP_FOREACH(Item,item,stock->map) {

        if (strcmp(item->type,"potion") == 0) {
        	current_value = item->value;
            if (current_value >= value_required) {
                *value_found = current_value;
                return STOCK_SUCCESS;
            }
        }
    }
    *value_found = current_value;

    return *value_found > 0 ? STOCK_SUCCESS : STOCK_NO_AVAILABLE_ITEM_FOUND;
}
//-----------------------------------------------------------------------------
StockResult stockFindMostValuableCandy(Stock stock, int* value_found) {

    if (stock == NULL  ||  value_found == NULL ) {
        return STOCK_NULL_ARGUMENT;
    }
    *value_found = NEGATIVE_NUM;

    MAP_FOREACH(Item,item,stock->map) {

        if (strcmp(item->type,"candy") == 0) {
            *value_found = item->value;
        }
    }
    return *value_found > 0 ? STOCK_SUCCESS : STOCK_NO_AVAILABLE_ITEM_FOUND;
}
//-----------------------------------------------------------------------------
