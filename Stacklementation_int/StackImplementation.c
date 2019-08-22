#include<stdio.h>
#include<stdlib.h>
#include "StackImplementation.h"

#define FIRST_STACK_ELEMENT 0
#define DEFAUL_CONST -1

//FIXME : uncomment and delete from "StackImplementation.h"
//STACK is a pointer to the struct stack_t
//next_index is the index of the next element to be place/remove
//max_capacity is the size of the stack
//the stack is build by an array of integers
/*struct stack_t {
  int next_index;
  int max_capacity;
  int* arr;
};
*/
//create a stack of with size of size_of_stack
//-----Errors:-----
//Null if malloc failed or size of stack is not positive
Stack StackCreate (int size_of_stack){
  Stack stack = malloc(sizeof(*stack));
  if ((stack == NULL) || (size_of_stack <= 0))	return NULL;
  stack->arr = malloc(size_of_stack*sizeof(int));
  if (stack->arr == NULL){
    free(stack); //FIXME at this stage the first malloc succeed - fix to assert
    return NULL;
  }
  stack->max_capacity = size_of_stack;
  stack->next_index = FIRST_STACK_ELEMENT;
return stack;
}

//free the memory allocated for stack
void StackDestroy (Stack* stack_ptr){
  if (*stack_ptr != NULL){
	  free((*stack_ptr)->arr);
	  (*stack_ptr)->arr = NULL;
  }
  free(*stack_ptr);
  *stack_ptr = NULL;
}

//inset num into the stack
//-----Errors-----:
//"InvaliStackAddress" for invalid stack
//"StackIsFull" for a full stack
//"ActionSuccess" otherwise
StackResult Push(Stack* stack_ptr , int num){
  if (*stack_ptr == NULL)	return InvalidStackAddress;
  if ((*stack_ptr)->next_index == (*stack_ptr)->max_capacity) 	return StackIsFull;
  ((*stack_ptr)->arr)[(*stack_ptr)->next_index] = num;
  (*stack_ptr)->next_index = (*stack_ptr)->next_index + 1;
return ActionSuccess;
}

//return into 'top' the top of stack
//-----Errors:-----
//"InvalidStackAddress" for an invalid stack
//"StackIsEmpty" for an empty stack
//"ActionSuccess" otherwise
StackResult Top(Stack stack , int* top_ptr){
  if (stack == NULL)	return InvalidStackAddress;
  if (stack->next_index == FIRST_STACK_ELEMENT) 	return StackIsEmpty;
  *top_ptr = (stack->arr)[stack->next_index - 1];
return ActionSuccess;
}

//pop the first element of stack into pop
//-----Errors-----
//"InvalidStackAddress" for an invalid stack
//"StackIsEmpty" for an empty stack
//"ActionSuccess" otherwise
StackResult Pop(Stack* stack_ptr , int* pop_ptr){
  if ((*stack_ptr) == NULL)	return InvalidStackAddress;
  if ((*stack_ptr)->next_index == FIRST_STACK_ELEMENT) 	return StackIsEmpty;
  *pop_ptr = ((*stack_ptr)->arr)[(*stack_ptr)->next_index - 1];
  (*stack_ptr)->next_index = (*stack_ptr)->next_index - 1;
return ActionSuccess;
}

//print the content of stack
void StackPrintElements(Stack stack){
	if (stack->next_index == FIRST_STACK_ELEMENT){ //if stack is empty
		printf("'stack is empty'\n");
		return;
	}
	int index = stack->next_index - 1;
	while (index >= FIRST_STACK_ELEMENT){
		printf("|  %d  |\n",(stack->arr)[index]);
		index = index -1;
	}
}



