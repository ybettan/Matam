#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


typedef  struct node_t* Node;
struct node_t {
	int n;
	Node next;
};



//===================================================================================
//declaring type Operatore
typedef int (*Operator)(int,int);
//-----------------------------------------------------------------------------------
/* create a new list , initiallize all the data to 0
 * ----Errors:------
 * assumes that malloc don't failes
 */
Node listCreate(int size) {

	Node list = NULL;

	for (int i = 0 ; i < size ; i++) {
		Node node = malloc( sizeof(*node) );
		assert(node != NULL);

		node->n = 0;
		node->next = list;
		list = node;
	}
	return list;
}
//-----------------------------------------------------------------------------------
//returning the size of a given list
int getListSize(const Node const list) {

	Node list_running = list;
	int counter = 0 ;

	while (list_running != NULL) {
		counter++;
		list_running = list_running->next;
	}
	return counter;
}
//-----------------------------------------------------------------------------------
//return "true" if 2 list have the same size or "false" otherwise
bool areEqualSize(const Node const list1,const Node const list2) {

	if (getListSize(list1) == getListSize(list2)) {
		return true;
	}else{
		return false;
	}
}
//-----------------------------------------------------------------------------------
//main function - workes as described in exercise
Node listOperator(const Node const list1,const Node const list2,Operator operator) {

	if (!areEqualSize(list1,list2)  ||  !(list1 && list2 && operator))	return NULL;

	Node list = listCreate( (getListSize(list1)) );
	Node list1_running = list1 , list2_running = list2 , list_running = list;

	while (list1_running != NULL   &&   list2_running != NULL) {
		list_running->n = operator(list1_running->n,list2_running->n);
		list1_running = list1_running->next;
		list2_running = list2_running->next;
		list_running = list_running->next;
	}
	return list;
}
//===================================================================================

// release all the memory allocated for list
void listDestroy (Node list) {

	while (list != NULL) {
		Node temp = list->next;
		free(list);
		list = temp;
	}
}
//-----------------------------------------------------------------------------------

//a binary operator that returns max(a,b)
int maxOperator(int a , int b) {
	if (a > b)
		return a;
	else
		return b;
}
//-----------------------------------------------------------------------------------

/* the function workes as described in exercise
 * giving R and L is equvivalent to array size
 */

Node arrayOfListsOperators(Node* arr,int L,int R) {

	if (L >= R)		return arr[L];

	int M = (L + R) / 2;

	Node left_list = arrayOfListsOperators(arr,L,M);
	Node right_list = arrayOfListsOperators(arr,M + 1,R);

	Node result_list = listOperator(left_list,right_list,maxOperator);
	listDestroy(left_list);
	listDestroy(right_list);

	return result_list;
}

//===================================================================================












int plusOperator(int a , int b) {
	return a + b;
}
int subOperator(int a , int b) {
	return a - b;
}
int mulOperator(int a , int b) {
	return a * b;
}
int divOperator(int a , int b) {
	return b / a;
}



#define LIST_SIZE 8
#define ARR_SIZE 5




void printList(const Node const list) {

	Node list_running = list;
	while (list_running != NULL) {
		printf(" %d --->",list_running->n);
		list_running = list_running->next;
	}
	printf(" NULL\n");
}
void printArrOfList(Node* arr) {

	for (int i = 0 ; i < ARR_SIZE ; i++) {
		Node list_running = arr[i];
		while (list_running != NULL) {
			printf(" %d --->",list_running->n);
			list_running = list_running->next;
		}
		printf(" NULL\n");
	}
}

int main() {

	Node list1 = listCreate(6);
	Node list2 = listCreate(7);

	printf("\n");
	if (listOperator(list1,list2,mulOperator) == NULL)
		printf("function with 2 different sized list test  :  SUCCESS\n");
	else
		printf("function with 2 different sized list test  :  FAILED\n");
	listDestroy(list1);
	listDestroy(list2);
	printf("------------------------------------------------------------------------\n");
//-----------------------------------------------------------------------------------

	list1 = listCreate(6);
	list2 = listCreate(6);
	Node list1_running = list1 , list2_running = list2;

	for (int i = 6 ; i >0 ; i--) {
		list1_running->n = rand() % 10;
		list2_running->n = rand() % 10;

		list1_running = list1_running->next;
		list2_running = list2_running->next;
	}

	printList(list1);
	printList(list2);
	printf("\n\n");

	Node list_result = listOperator(list1,list2,mulOperator);
	printList(list_result);

	listDestroy(list1);
	listDestroy(list2);
	listDestroy(list_result);
	printf("------------------------------------------------------------------------\n");
//-----------------------------------------------------------------------------------



	Node arr[ARR_SIZE];
	for (int i = 0 ; i < ARR_SIZE ; i++) {
		arr[i] = listCreate(LIST_SIZE);
	}
	for (int i = 0 ; i < ARR_SIZE ; i++) {
		Node list_running = arr[i];
		while (list_running != NULL) {
			list_running->n = rand() % 20;
			list_running = list_running->next;
		}
	}
	printArrOfList(arr);
	printf("\n\n");

	Node result_list = arrayOfListsOperators(arr,0,ARR_SIZE - 1);
	printList(result_list);

	listDestroy(result_list);
}






