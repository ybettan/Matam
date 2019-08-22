#ifndef LISTIMPLEMENTATION_H_
#define LISTIMPLEMENTATION_H_

#include<stdbool.h>

//possible results for function List functions
typedef enum ListResults{
  FailToAllocateMemory,
  ActionSuccess,
  TheListIsEmpty,
  InvalidList
}ListResults;

//ListElement is struct that contain an element of list
//LIST is a pointer to the first element in the struct
typedef struct list_element_t ListElement;
typedef ListElement* List;

//create an empty list of integers and return a pointer to the first element
//--------Errors:--------
//NULL if faild to allocate memory
List ListCreate();

//release the memory alloceted for list and initiallize list to point to NULL
//-----Errors:-------
//return "InvalidList" for a non allocated list
//return "ActionSuccess" otherwise
ListResults ListDestroy(List** list_ptr_ptr);

//insert num at the head of list
//-----Errors:-----
//return "FailToAllocateMemory" if faild to allocate memory for an element
//return "ActionSuccess" otherewise
ListResults ListInsert(List* list_ptr,int num);

//update "result" to be TRUE if the list is empty or FALSE otherwise
//---------Errors:--------
//return "InvalidList" for a non created list
//return "ActionSuccess" otherwise
ListResults ListIsEmpty(List* list_ptr,bool* result);

//update "head" to be the data stored in the first element of list
//-----Errors:------
//return "InvalidList" for a non created list
//return "TheListIsEmpty" for an empty list
//return "ActionSuccess" otherwise
ListResults ListHead(List* list_ptr,int* head_ptr);

//update list_next_ptr to point to the next element of list or NULL if list is
//the last element
//-----Errors:------
//return "InvalidList" for a non created list
//return "TheListIsEmpty" for an empty list
//return "ActionSuccess" otherwise
ListResults ListNextElement(List* list_ptr , List* list_next_ptr);

//printing all the list elements in order
//------Errors:-------
//return "InvalidList" for a list that wasn't created
//return "ActionSuccess" otherwise
ListResults ListPrintElements(List* list_ptr);

#endif /* LISTIMPLEMENTATION_H_ */
