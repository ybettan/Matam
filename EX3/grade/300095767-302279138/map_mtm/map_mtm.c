#include "../map.h"

#include <stdlib.h>
#include <assert.h>

#define ILLEGAL_SIZE -1
#define EQUAL 0
#define EMPTY_MAP 0


typedef enum{
	INVALID_STATE,
	VALID_STATE,
}IteratorState;

struct Map_t{
	struct node_t* node;
	struct node_t* iterator;
	IteratorState iterator_state;
	int size;
	copyMapKeyElements copyKey;
	copyMapDataElements copyData;
	freeMapKeyElements freeKey;
	freeMapDataElements freeData;
	compareMapKeyElements compareKey;
};

typedef struct node_t{
	MapKeyElement key;
	MapDataElement data;
	struct node_t* next;
}*Node;

//-----------------------------------------------------------------------------
//                      STATIC FUNCTION
//-----------------------------------------------------------------------------


/** creates new node with specific MapKeyElement and MapDataElement and
 *  returns it. if allocation failed returns NULL. */

static Node nodeCreate(Map map, MapKeyElement key, MapDataElement data){
	assert(key && data && map);
	Node node = malloc(sizeof(*node));
	if(!node) return NULL;
	node->key = map->copyKey(key);;
	if(!node->key){
		free(node);
		return NULL;
	}
	node->data = map->copyData(data);
	if(!node->data){
		map->freeKey(node->key);
		free(node);
		return NULL;
	}
	return node;
}


/** free key and data of a node using the freeMapKeyElements and
 * freeMapDataElements functions. */

static void nodeDestroy(Map map, Node node){
	assert(map && node);
	if(node->key){
		map->freeKey(node->key);
	}
	if(node->data){
		map->freeData(node->data);
	}
	free(node);
}


/** gets map source and map to copy.  by iterating over source node, the
 *  function creates nodes with the same key and data  corresponding to source
 *  values. if all allocation succeeded it return true. otherwise false. */

static bool MapCopyDataAndMoves(Map map,Map map_copy){
	assert(map && map_copy);
	if (!map->node) return true;
	map_copy->node = nodeCreate(map, map->node->key, map->node->data);
	if(!map_copy->node) return false;
	map_copy->size++;
	map_copy->iterator = map_copy->node;
	for(map->iterator = map->node; map->iterator->next != NULL;
			map->iterator = map->iterator->next){
		map_copy->iterator->next = nodeCreate(map, map->iterator->next->key,
				map->iterator->next->data);
		if(!map_copy->iterator->next) return false;
		map_copy->size++;
		map_copy->iterator = map_copy->iterator->next;
	}
	map_copy->iterator->next = NULL;
	return true;
}


/** the function return the closest node that its key is smaller then or equal
 *  to the key being searched. it returns null if all the keys in the map are
 *  greater than the searched key or if the map is empty. using the internal
 *   iterator in order to iterate over the map. */

static Node getClosestNode(Map map, MapKeyElement keyElement){
	assert(map && keyElement);
	if(!map->node || map->compareKey(map->node->key, keyElement) > EQUAL)
		return NULL;
	for(map->iterator = map->node; map->iterator->next != NULL;
				map->iterator = map->iterator->next){
		if((map->compareKey(map->iterator->key,keyElement) <= EQUAL) &&
				 map->compareKey(map->iterator->next->key,keyElement) > EQUAL){
			 break;
		}
	}
	return map->iterator;
}


/** given a key and data, the function creates new node and puts it after the
 *  closest node given as an argument (null indicates that node should be put
 *   at the beginning). if allocation fail returns false, otherwise true.  */

static bool isPutSuccessful(Map map, MapKeyElement key, MapDataElement data,
		Node closest_node){
	assert(map && key && data);
	Node temp_node;
	Node node_to_put = nodeCreate(map, key, data);
	if(!node_to_put) return false;
	if(!closest_node){
		temp_node = map->node;
		map->node = node_to_put;
	} else {
		temp_node = closest_node->next;
		closest_node->next = node_to_put;
	}
	node_to_put->next = temp_node;
	map->size++;
	return true;
}


/** removes a node from the map, and directs node_before->next to
 * node_to remove->next. null in second argument indicates that node to remove
 *  is the first ordered node.  */

static void removeNode(Map map, Node node_before, Node node_to_remove){
	assert(map && node_to_remove);
	Node temp_node = node_to_remove->next;
	if(!node_before){
		map->node = temp_node;
	} else {
		node_before->next = temp_node;
	}
	nodeDestroy(map, node_to_remove);
	map->size--;
}


/** returns the node before the node delivered as an argument.
 * returns null if node given is the first. */

static Node getNodeBefore(Map map, Node node){
	assert(map && node);
	if (map->node == node) return NULL;
	map->iterator = map->node;
	while(map->iterator->next != node){
		map->iterator = map->iterator->next;
	}
	return map->iterator;
}


//-----------------------------------------------------------------------------
//                      HEADER FUNCTION
//-----------------------------------------------------------------------------


Map mapCreate(copyMapKeyElements copyKeyElement,
		copyMapDataElements copyDataElement,
		freeMapKeyElements freeKeyElement,
		freeMapDataElements freeDataElement,
	    compareMapKeyElements compareKeyElements){

	if(!copyKeyElement || !copyDataElement || !freeKeyElement ||
			!freeDataElement || !compareKeyElements) return NULL;
	Map map = malloc(sizeof(*map));
	if(!map) return NULL;
	map->node = NULL;
	map->iterator = NULL;
	map->iterator_state = INVALID_STATE;
	map->size = EMPTY_MAP;
	map->copyKey = copyKeyElement;
	map->copyData = copyDataElement;
	map->freeKey = freeKeyElement;
	map->freeData = freeDataElement;
	map->compareKey = compareKeyElements;
	return map;
}


void mapDestroy(Map map){
	if(!map) return;
	while(map->node){
		Node temp_node = map->node;
		map->node = map->node->next;
		nodeDestroy(map, temp_node);
	}
	free(map);
}


Map mapCopy(Map map){
	if(!map) return NULL;
	map->iterator_state = INVALID_STATE;
	Map map_copy = mapCreate(map->copyKey, map->copyData, map->freeKey,
			map->freeData, map->compareKey);
	if(!map_copy){
		return NULL;
	}
	bool result = MapCopyDataAndMoves(map, map_copy);
	if(!result){
		mapDestroy(map_copy);
		return NULL;
	}
	return map_copy;
}


int mapGetSize(Map map){
	return (map != NULL) ? map->size : ILLEGAL_SIZE;
}


bool mapContains(Map map, MapKeyElement element){
	if(!map) return false;
	map->iterator_state = INVALID_STATE;
	if(!element) return false;
	map->iterator = map->node;
	while(map->iterator){
		if(map->compareKey(map->iterator->key, element) == EQUAL) return true;
		map->iterator= map->iterator->next;
	}
	return false;
}


MapKeyElement mapGetFirst(Map map){//Fixme: what if map!= nul? iterator?
	if(!map || map->size == EMPTY_MAP) return NULL;
	map->iterator_state = VALID_STATE;
	map->iterator = map->node;
	return map->iterator->key;
}


MapKeyElement mapGetNext(Map map){
	if(!map || map->iterator_state == INVALID_STATE) return NULL;
	map->iterator = map->iterator->next;
	if(!map->iterator) {
		map->iterator_state = INVALID_STATE;
		return NULL;
	}
	return map->iterator->key;
}


MapResult mapPut(Map map, MapKeyElement keyElement,MapDataElement dataElement){
	if(!map)return MAP_NULL_ARGUMENT;
	map->iterator_state = INVALID_STATE;
	if(!keyElement || !dataElement) return MAP_NULL_ARGUMENT;
	Node closest_node = getClosestNode(map, keyElement);
	if(closest_node && map->compareKey(closest_node->key,keyElement) == EQUAL){
		MapDataElement new_data = map->copyData(dataElement);
		if(!new_data) return MAP_OUT_OF_MEMORY;
		map->freeData(closest_node->data);
		closest_node->data = new_data;
	} else if (!isPutSuccessful(map, keyElement, dataElement, closest_node)){
		return MAP_OUT_OF_MEMORY;
	}
	return MAP_SUCCESS;
}


MapDataElement mapGet(Map map, MapKeyElement keyElement){
	if(!map || !keyElement || map->size == EMPTY_MAP){
		return NULL;
	}
	Node iterator_copy = map->iterator;
	Node closest_node = getClosestNode(map, keyElement);
	map->iterator = iterator_copy;
	if(closest_node && map->compareKey(closest_node->key, keyElement) == EQUAL)
		return closest_node->data;
	return NULL;
}


MapResult mapRemove(Map map, MapKeyElement keyElement){
	if(!map) return MAP_NULL_ARGUMENT;
	map->iterator_state = INVALID_STATE;
	if(!keyElement) return MAP_NULL_ARGUMENT;
	Node closest_node = getClosestNode(map, keyElement);
	if(!closest_node || map->compareKey(closest_node->key,keyElement) != EQUAL)
		return MAP_ITEM_DOES_NOT_EXIST;
	removeNode(map, getNodeBefore(map, closest_node), closest_node);
	return MAP_SUCCESS;
}


MapResult mapClear(Map map){
	if(!map) return MAP_NULL_ARGUMENT;
	map->iterator_state = INVALID_STATE;
	while(map->node){
		removeNode(map, NULL, map->node);
	}
	return MAP_SUCCESS;
}
