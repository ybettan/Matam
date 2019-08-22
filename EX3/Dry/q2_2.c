#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//===================================================================================
//type definition
typedef void* Element;
typedef int (*CmpFunction)(Element,Element);
//-----------------------------------------------------------------------------------
//when given 2 sorted sub-arrays of an array , the function sort the total array
void merge(Element* arr,int L,int M,int R,CmpFunction cmpFunction) {

	int i = L , j = M + 1 , index = 0;
	Element temp_arr[R - L + 1];

	while (i <= M   &&   j <= R) {
		if ( cmpFunction(arr[i],arr[j]) < 0) {
			temp_arr[index++] = arr[i++];
		}else{
			temp_arr[index++] = arr[j++];
		}
		assert(temp_arr[index - 1] != NULL);
	}
	while ( i <= M ) {
		temp_arr[index++] = arr[i++];
		assert(temp_arr[index - 1] != NULL);
	}
	while ( j <= R ) {
		temp_arr[index++] = arr[j++];
		assert(temp_arr[index - 1] != NULL);
	}
	for (int i = L , k = 0 ; i <= R ; i++) {
		arr[i] = temp_arr[k++];
		assert(temp_arr[index - 1] != NULL);
	}
}
//-----------------------------------------------------------------------------------
//by divide and conccer we devide the array recursivley to 2 sub-arrays and merge them
void genericMergeSort(Element* arr,int L,int R,CmpFunction cmpFunction) {

	if (L >= R)	return;

	int M = (L + R) / 2;

	genericMergeSort(arr,L,M,cmpFunction);
	genericMergeSort(arr,M + 1,R,cmpFunction);

	merge(arr,L,M,R,cmpFunction);
}
//===================================================================================

int intCmp(Element a,Element b) {

	return *(int*)a - *(int*)b;
}

int strCmp(Element a,Element b) {

	return strcmp((char*)a,(char*)b);
}


int main() {

	char* arr[] = { "bbb","ccc","hhh","aaa","ddd","eee","ggg","fff","bbb"};

	for (int i = 0 ; i < 9 ; i ++)
		printf("%s  ",arr[i]);
	printf("\n");




	genericMergeSort((Element*)arr,0,8,strCmp);

	for (int i = 0 ; i < 9 ; i ++)
		printf("%s  ",arr[i]);
	printf("\n");

}



