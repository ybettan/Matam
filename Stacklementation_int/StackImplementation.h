/*
 * StackImplementation.h
 *
 *  Created on: 21 срхїз 2016
 *      Author: yoni
 */

#ifndef STACKIMPLEMENTATION_H_
#define STACKIMPLEMENTATION_H_

//ADT stack of integers
typedef struct stack_t* Stack;

//FIXME : to be removed from "StackImplementation.h"
struct stack_t {
  int next_index;
  int max_capacity;
  int* arr;
};

//possible result for stack function
typedef enum StackResult{
  StackIsFull ,
  StackIsEmpty ,
  InvalidStackAddress ,
  ActionSuccess} StackResult;

//create a stack of with size of size_of_stack and return a pointer to the stack
//-----Errors:-----
//Null if failed
Stack StackCreate (int size_of_stack);

//free the memory allocated for stack
void StackDestroy (Stack* stack_ptr);

//inset num into the stack
//-----Errors-----:
//"InvaliStackAddress" if stack is NULL
//"StackIsFull" for a full stack
//"ActionSuccess" otherwise
StackResult Push(Stack* stack_ptr , int num);

//return into 'top' the top of stack
//-----Errors:-----
//"InvalidStackAddress" if stack is NULL
//"StackIsEmpty" for an empty stack
//"ActionSuccess" otherwise
StackResult Top(Stack stack , int* top_result_ptr);

//pop the first element of stack into pop
//-----Errors-----
//"InvalidStackAddress" if stack is NULL
//"StackIsEmpty" for an empty stack
//"ActionSuccess" otherwise
StackResult Pop(Stack* stack_ptr , int* pop);

//print the content of stack
void StackPrintElements(Stack stack);

#endif /* STACKIMPLEMENTATION_H_ */
