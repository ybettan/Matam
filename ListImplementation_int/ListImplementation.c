#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "ListImplementation.h"

//LIST is a pointer to the struct list_t
//data is the data of the element
//next is a pointer to the next element of the list
struct list_element_t {
  int data;
  struct list_element_t* next;
};

//create an empty list of integers and return a pointer to the first element
List ListCreate(){
  List list = NULL;
  return list;
}

//release the memory alloceted for list and initiallize list to point to NULL
//return "InvalidList" for a non allocated list
//return "ActionSuccess" otherwise
ListResults ListDestroy(List** list_ptr_ptr){
	if (*list_ptr_ptr == NULL)	return InvalidList;
    List index = *(*list_ptr_ptr);
    while (index != NULL){
    List tmp = index;
	index = index->next;
	free (tmp);
    }
    *list_ptr_ptr = NULL;
    return ActionSuccess;
}

//insert num at the head of list
//-----Errors:-----
//return "FailToAllocateMemory" if faild to allocate memory for an element
//return "ActionSuccess" otherewise
ListResults ListInsert(List* list_ptr,int num){
  List new_list_element_ptr  = malloc(sizeof(*new_list_element_ptr));
  if (new_list_element_ptr == NULL)	return FailToAllocateMemory;
  new_list_element_ptr->data = num;
  new_list_element_ptr->next = *list_ptr;
  *list_ptr = new_list_element_ptr;
  return ActionSuccess;
}

//update "result" to be TRUE if the list is empty or FALSE otherwise
//---------Errors:--------
//return "InvalidList" for a non created list
//return "InvalidList" for a non created list
//return "ActionSuccess" otherwise
ListResults ListIsEmpty(List* list_ptr , bool* result){
  if (list_ptr == NULL)		return InvalidList;
  if (*list_ptr == NULL){
	  *result = true;
  }else{
	  *result = false;
  }
  return ActionSuccess;
}

//update "head" to be the data stored in the first element of list
//-----Errors:------
//return "InvalidList" for a non created list
//return "TheListIsEmpty" for an empty list
//return "ActionSuccess" otherwise
ListResults ListHead(List* list_ptr,int* head_ptr){
	if (list_ptr == NULL)		return InvalidList;
	if (*list_ptr == NULL)		return TheListIsEmpty;
	*head_ptr = (*list_ptr)->data;
	return ActionSuccess;
}

/*update list_next_ptr to point to the next element of list or NULL if list is
  the last element*/
//-----Errors:------
//return "TheListIsEmpty" for an empty list
//return "ActionSuccess" otherwise
ListResults ListNextElement(List* list_ptr , List* list_next_ptr){
	if (list_ptr == NULL)	    return InvalidList;
	if (*list_ptr == NULL)		return TheListIsEmpty;
	*list_next_ptr = (*list_ptr)->next;
	return ActionSuccess;
}

//printing all the list elements in order
//------Errors:-------
//return "InvalidList" for a list that wasn't created
//return "ActionSuccess" otherwise
ListResults ListPrintElements(List* list_ptr){
  if (list_ptr == NULL)		return InvalidList;
  List index = *list_ptr;
  while (index != NULL){
      printf(" --> %d",index->data);
      index = index->next;
  }
  printf(" --> NULL\n");
  return ActionSuccess;
}

