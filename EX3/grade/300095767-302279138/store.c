#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "store.h"

//=============================================================================
//								MACROS
//=============================================================================


#define MAX_TYPE_LEN 10
#define NEGATIVE_NUM -1
#define POSITIVE_NUM 1


//=============================================================================
//								STRUCT
//=============================================================================

//store struct is a stock
struct store_t {
	Stock stock;
};


// translate stockResult to StoreResult
static StoreResult stockToStoreResult(StockResult result) {
	switch (result){

	case STOCK_OUT_OF_MEM : return STORE_OUT_OF_MEM;
	case STOCK_NULL_ARGUMENT : return STOCK_NULL_ARGUMENT;
	case STOCK_INVALID_ARGUMENT : return STORE_INVALID_ARGUMENT;
	case STOCK_ITEM_OUT_OF_STOCK : return STORE_ITEM_OUT_OF_STOCK;
	default : return STORE_SUCCESS;
	}
}


//=============================================================================
//					  		HEADER FUNCTION
//=============================================================================


Store storeCreate() {

	Store store = malloc( sizeof(*store) );
	if (store == NULL)	return NULL;

	store->stock = stockCreate();
	if (store->stock == NULL) {
		free(store);
		return NULL;
	}

	return store;
}
//-----------------------------------------------------------------------------
void storeDestroy(Store store) {

	if (store != NULL) {
		stockDestroy(store->stock);
	}
	free(store);
}
//-----------------------------------------------------------------------------
int storeGetSize(Store store) {

	if (store == NULL)	return NEGATIVE_NUM;

	return (stockGetSize(store->stock));
}
//-----------------------------------------------------------------------------
StoreResult storeAddItem(Store store,const char* type,int value,int quantity) {

	if (store == NULL || type == NULL)   		return STOCK_NULL_ARGUMENT;
	StockResult result = stockAddItem(store->stock,type,value,quantity);

	return stockToStoreResult(result);
}
//-----------------------------------------------------------------------------
StoreResult storeRemoveItem(Store store,const char* type, int value) {

	if (store == NULL || type == NULL ) 	return STORE_NULL_ARGUMENT;

	StockResult result = stockRemoveItem(store->stock,type,value);
	return stockToStoreResult(result);
}
//-----------------------------------------------------------------------------
StoreResult storeShowStock(Store store,FILE* output_file) {

	assert(output_file != NULL);
	if (store == NULL)	return STORE_NULL_ARGUMENT;

	StockResult result = stockShowInfo(store->stock,output_file,true);
	return stockToStoreResult(result);
}
//-----------------------------------------------------------------------------
