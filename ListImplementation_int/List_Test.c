#include<stdio.h>
#include<stdlib.h>
#include "ListImplementation.h"

int main(){

//---------------------------------------------
	List list = NULL;
	List* list_ptr = &list;
	List** list_ptr_ptr = &list_ptr;
//---------------------------------------------

//ListCreate Tests
	printf("------------------ListCreate Test--------------------\n");
	list = ListCreate();
	printf("list should point tu NULL and its value is : %p\n", (void*)list);
	printf("list_ptr should be a valid pointer and its value is : %p\n", (void*)list_ptr);
	printf("the list is : ");
	ListPrintElements(list_ptr);

//ListDestroy Tests
	printf("------------------ListDestroy Test--------------------\n");
	ListResults result = ListDestroy(list_ptr_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	printf("after destroy list ptr should point to NULL and its value is : %p\n",list_ptr);
	result = ListDestroy(list_ptr_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = ListPrintElements(list_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

//ListInsert Tests
	printf("------------------ListInsert Test--------------------\n");
	list = ListCreate();
	list_ptr = &list;                      //FIXME : to be remove after insert it into ListCreate
	result = ListInsert(list_ptr,5);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	printf("we inserted 5 and now list is :   ");
	ListPrintElements(list_ptr);
	ListInsert(list_ptr,7);
	printf("we inserted 7 and now list is :   ");
	ListPrintElements(list_ptr);
	ListInsert(list_ptr,9);
	printf("we inserted 9 and now list is :   ");
	ListPrintElements(list_ptr);

//ListIsEmpty Tests
	printf("------------------ListIsEmpty Test--------------------\n");
	ListDestroy(list_ptr_ptr);
	bool response;
	bool* response_ptr = &response;
	result = ListIsEmpty(list_ptr,response_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	list = ListCreate();
	list_ptr = &list;                      //FIXME : to be remove after insert it into ListCreate
	result = ListIsEmpty(list_ptr,response_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (response == true){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,4);
	ListIsEmpty(list_ptr,response_ptr);
	if (response == false){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

//ListPrintElements Tests
	printf("------------------ListPrintElements Test--------------------\n");
	ListDestroy(list_ptr_ptr);
	result = ListPrintElements(list_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	list = ListCreate();
	list_ptr = &list;                      //FIXME : to be remove after insert it into ListCreate
	printf("[--> NULL] should be printed : ");
	result = ListPrintElements(list_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,17);
	printf("we had inset 17 so the list is :  ");
	result = ListPrintElements(list_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,1100);
	printf("we had inset 1100 so the list is :  ");
	ListPrintElements(list_ptr);
	ListInsert(list_ptr,97);
	printf("we had inset 97 so the list is :  ");
	ListPrintElements(list_ptr);
	ListInsert(list_ptr,410);
	printf("we had inset 410 so the list is :  ");
	ListPrintElements(list_ptr);

//ListHead Tests
	printf("------------------ListPrintHead Test--------------------\n");
	ListDestroy(list_ptr_ptr);
	int head;
	int* head_ptr = &head;
	result = ListHead(list_ptr,head_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	list = ListCreate();
	list_ptr = &list;                      //FIXME : to be remove after insert it into ListCreate
	result = ListHead(list_ptr,head_ptr);
	if (result == TheListIsEmpty){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,88);
	result = ListHead(list_ptr,head_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	printf("we had insert 88   --->   ");
	printf("list.head is : %d\n",head);

	ListInsert(list_ptr,100);
	ListHead(list_ptr,head_ptr);
	printf("we had insert 100   --->   ");
	printf("list.head is : %d\n",head);

	ListInsert(list_ptr,12);
	ListInsert(list_ptr,13);
	ListInsert(list_ptr,15);
	ListHead(list_ptr,head_ptr);
	printf("we had insert 12 then 13 then 15   --->   ");
	printf("list.head is : %d\n",head);

//ListNextElement Tests
	printf("------------------ListNextElement Test--------------------\n");
	ListDestroy(list_ptr_ptr);
	List list_next = NULL;
	List* list_next_ptr = &list_next;
	result = ListNextElement(list_ptr,list_next_ptr);
	if (result == InvalidList){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	list = ListCreate();
	list_ptr = &list;                      //FIXME : to be remove after insert it into ListCreate
	result = ListNextElement(list_ptr,list_next_ptr);
	if (result == TheListIsEmpty){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,50);
	result = ListNextElement(list_ptr,list_next_ptr);
	if (result == ActionSuccess){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (list_next == NULL){
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	ListInsert(list_ptr,40);
	ListInsert(list_ptr,70);
	ListInsert(list_ptr,36);
	printf("the list is :   ");
	ListPrintElements(list_ptr);
	ListNextElement(list_ptr,list_next_ptr);
	ListHead(list_ptr,head_ptr);
	printf("list.head is :   %d\n",head);
	ListHead(list_next_ptr,head_ptr);
	printf("list.next is : %d\n",head);
	List* temp = list_next_ptr;
	ListNextElement(temp,list_next_ptr);
	ListHead(list_next_ptr,head_ptr);
	printf("list.next.next is : %d\n",head);

}
