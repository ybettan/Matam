#include<stdio.h>
#include "stackImplementation.h"


int main(){

//###########StackCreate
	printf("-------StackCreate test--------\n");
	Stack stack = StackCreate(-3);
	if (stack == NULL)
		printf("SUCCEED\n");
	else
 	  printf("FAILD\n");

	stack = StackCreate(3);
	printf("stack address should be a valid pointer and its value is : %p\n",stack);
	printf("stack fields are : \n");
	printf("next_index : %d\nmax_capacity : %d\narr address : %p\n",stack->next_index,stack->max_capacity,stack->arr);

//##########StackDestroy
	printf("-------StackDestroy test--------\n");
	Stack* stack_ptr = &stack;
	StackDestroy(stack_ptr);
	printf("stack address should be NULL and its value is : %p\n",stack);
	printf("stack.arr address should be NULL and was tested in StackDestroy func due to access to allocated memory otherwise (is working well)\n");

//############StackPush
	printf("--------StackPush test----------\n");
	//at this stage stack == NULL FIXME : fix to assert
	StackResult func_resault = Push(stack_ptr,4);
	if (func_resault == InvalidStackAddress)
	  printf("SUCCEED\n");
	else
	  printf("FAILD\n");

	stack = StackCreate(2);
	func_resault = Push(stack_ptr,5);
	if (func_resault == ActionSuccess)
	  printf("SUCCEED\n");
	else
	  printf("FAILD\n");

	printf("we had insert 5 so top should be 5 and its value is : %d\n",(stack->arr)[stack->next_index - 1]);
	Push(stack_ptr,20);
	printf("we insert also 20 so now stack is : \n|  %d  |\n|  %d   |\n ------\n",(stack->arr)[stack->next_index-1],(stack->arr)[stack->next_index-2]);

	func_resault = Push(stack_ptr,12);
	if (func_resault == StackIsFull)
	  printf("SUCCEED\n");
	else
	  printf("FAILD\n");

//############StackTop
	printf("--------StackTop test---------\n");
	StackDestroy(stack_ptr);
	// at this stage stack point to NULL FIXME : fix to assert
	int top;
	int* top_ptr = &top;
	func_resault = Top(stack,top_ptr);
	if (func_resault == InvalidStackAddress)
      printf("SUCCEED\n");
    else
      printf("FAILD\n");

	stack = StackCreate(3);
	func_resault = Top(stack,top_ptr);
	if (func_resault == StackIsEmpty)
      printf("SUCCEED\n");
    else
      printf("FAILD\n");

	Push(stack_ptr,30);
	Push(stack_ptr,40);
	Top(stack,top_ptr);
	printf("we pushed 30 and then 40 so top is : %d\n",top);
	Top(stack,top_ptr);
	printf("top stay is stack so top remains : %d\n",top);

//###########StackPop
	printf("--------StackPop test---------\n");
	printf("stack is : \n|  %d  |\n|  %d  |\n ------\n",(stack->arr)[stack->next_index-1],(stack->arr)[stack->next_index-2]);
	int pop;
	int* pop_ptr = &pop;
	func_resault = Pop(stack_ptr,pop_ptr);
	if (func_resault == ActionSuccess)
      printf("SUCCEED\n");
    else
      printf("FAILD\n");

	printf("pop should be 40 and its value is : %d\n",pop);
	Top(stack,top_ptr);
	printf("top should be now 30 and its value is : %d\n",top);
	Pop(stack_ptr,pop_ptr);
	printf("now pop should be 30 and its value is : %d\n",pop);
	//at this stage stack is empty FIXME : fix to assert
	func_resault = Pop(stack_ptr,pop_ptr);
	if (func_resault == StackIsEmpty)
      printf("SUCCEED\n");
    else
      printf("FAILD\n");

//############StackPrintElements
	printf("--------StackPrintElements test---------\n");
	printf("try to print an empty stack will gives us :\n");
	StackPrintElements(stack);
	Push(stack_ptr,10);
	printf("we pushed 10 so now stack is : \n");
	StackPrintElements(stack);
	Push(stack_ptr,11);
	printf("we also pushed 11 so now stack is : \n");
	StackPrintElements(stack);
	Push(stack_ptr,12);
	printf("we also pushed 12 so now stack is : \n");
	StackPrintElements(stack);
}
