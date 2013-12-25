
/*
Project : Dynamic Array - Hashed Array Tree
Status  : Complete
Author  : Daniel Sirait <dns@cpan.org>
Copyright   : Copyright (c) 2013 Daniel Sirait
License : Must be distributed only under the terms of "THE BEER-WARE LICENSE"  (Revision 42):
 As long as you retain this notice you
 can do whatever you want with this stuff. If we meet some day, and you think
 this stuff is worth it, you can buy me a beer in return
Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
  ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
  WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include "darray.h"
/*
// TODO: FUTURE, elements data type can be assigned at runtime
#define array_new(arr, datatype) \
	ARRAY *arr;			\
	arr = malloc(sizeof(ARRAY));	\
	arr->blocks = malloc(sizeof(ARRAY_BLOCK));	\
	arr->blocks[0].elements = malloc(sizeof(datatype) * ARRAY_BLOCK_CAPACITY);	\
	arr->element_size = 0;	\
	arr->block_size = 1;	\
	arr->capacity = ARRAY_BLOCK_CAPACITY;
*/

ARRAY *array_new (void) {
	ARRAY *arr;
	arr = malloc(sizeof(ARRAY));
	if (arr == NULL) return NULL;
	arr->blocks = malloc(sizeof(ARRAY_BLOCK));
	arr->blocks[0].elements = malloc(sizeof(float) * ARRAY_BLOCK_CAPACITY);
	arr->element_size = 0;
	arr->block_size = 1;
	arr->capacity = ARRAY_BLOCK_CAPACITY;
	return arr;
}


void array_destroy (ARRAY *arr) {
	array_clear(arr);
	free(arr->blocks[0].elements);
	free(arr->blocks);
	free(arr);
}

void array_add (ARRAY *arr, float val) {
	DWORD curr_index;

	if (arr->element_size < arr->capacity) {
		curr_index = ARRAY_BLOCK_CAPACITY - (arr->capacity - arr->element_size);

		arr->blocks[arr->block_size-1].elements[curr_index] = val;

		//printf("%d\n", *(DWORD*)arr->blocks[0].elements[0]);

		arr->element_size++;
	} else {
		arr->blocks = realloc(arr->blocks, (arr->block_size+1) * sizeof(ARRAY_BLOCK));
		arr->blocks[arr->block_size].elements = malloc(sizeof(float) * ARRAY_BLOCK_CAPACITY);


		arr->blocks[arr->block_size].elements[0] = val;

		arr->block_size++;
		arr->element_size++;
		arr->capacity += ARRAY_BLOCK_CAPACITY;
	}
}

DWORD array_size (ARRAY *arr) {
	return arr->element_size;
}

float array_get (ARRAY *arr, DWORD index) {

	//printf("%d\n", *(DWORD*)arr->blocks[0].elements[0]);

	DWORD block_index, el_index;
	block_index = index / ARRAY_BLOCK_CAPACITY;
	el_index = index % ARRAY_BLOCK_CAPACITY;

	return arr->blocks[block_index].elements[el_index];
}


bool array_set (ARRAY *arr, DWORD index, float val) {
	DWORD block_index;
	DWORD el_index;

	if (array_size(arr)-1 < index)
		return false;

	block_index = index / ARRAY_BLOCK_CAPACITY;
	el_index = index % ARRAY_BLOCK_CAPACITY;
	arr->blocks[block_index].elements[el_index] = val;

	return true;
}



void array_remove (ARRAY *arr, DWORD index) {
	DWORD el_index, size, i, remainder;

	size = array_size(arr);

	// TODO: SLOW, OPTIMIZE IN array_removeRange()
	for (i=index; i<size; i++) {
		if (i < size-1)
			array_set(arr, i, array_get(arr, i+1));
	}

	el_index = index % ARRAY_BLOCK_CAPACITY;

	remainder = arr->capacity - arr->element_size;
	//printf("%d\n", t);
	if (el_index == 0 && arr->block_size > 1 && remainder > ARRAY_BLOCK_CAPACITY) {
		arr->block_size--;
		free(arr->blocks[arr->block_size].elements);
		//printf("el_index: %d\nblock_size: %d\n", el_index, arr->block_size);
		arr->blocks = realloc(arr->blocks, arr->block_size * sizeof(ARRAY_BLOCK));
		arr->capacity = ARRAY_BLOCK_CAPACITY * arr->block_size;
	}



	if (arr->element_size != 0)
		arr->element_size--;

}

void array_clear (ARRAY *arr) {
	DWORD i, block_size;

	block_size = array_size(arr);

	for (i=1; i<arr->block_size; i++)
		free(arr->blocks[i].elements);

	arr->blocks = realloc(arr->blocks, 1 * sizeof(ARRAY_BLOCK));
	arr->element_size = 0;
	arr->block_size = 1;
	arr->capacity = ARRAY_BLOCK_CAPACITY;
}




/* TODO: SLOW, NEED OPTIMIZATION */
bool array_remove_range (ARRAY *arr, DWORD fromIndex, DWORD toIndex) {
	DWORD i, total_remove, removed_block, size;
	DWORD startBlock, endCopyBlock, endBlock, currBlock, currIndex;

	if (fromIndex > toIndex) return false;

	size = array_size(arr);
	total_remove = toIndex - fromIndex + 1;
	removed_block = (toIndex - fromIndex) / ARRAY_BLOCK_CAPACITY;


	//printf("%d\n", total_remove);
	for (i=fromIndex; i<fromIndex+total_remove; i++)
		array_remove(arr, fromIndex);	// SLOW !!!

	arr->element_size -= total_remove;
	arr->block_size -= total_remove / ARRAY_BLOCK_CAPACITY;
	arr->capacity = ARRAY_BLOCK_CAPACITY * arr->block_size;


		/*

	//startBlock = fromIndex / ARRAY_BLOCK_CAPACITY;
	//endCopyBlock = toIndex / ARRAY_BLOCK_CAPACITY;
	endBlock = toIndex / ARRAY_BLOCK_CAPACITY;

	//printf("%d - %d\n", endBlock, removed_block);





	for (i=fromIndex; i<200; i++) {
		currBlock = i / ARRAY_BLOCK_CAPACITY;
		currIndex = i % ARRAY_BLOCK_CAPACITY;



		//if (currBlock+removed_block != endBlock)
			//printf("%d: %d\n", currBlock, currBlock+removed_block);
			arr->blocks[currBlock].elements[currIndex] = arr->blocks[currBlock+endBlock].elements[currIndex];



	}
	*/

	//free(arr->blocks[i+total_remove].elements);
	//arr->blocks = realloc(arr->blocks, (arr->block_size) * sizeof(ARRAY_BLOCK));

	return true;
}

void array_iterate (ARRAY *arr, void (*callback)(DWORD index, float val)) {
	DWORD i, size;
	size = array_size(arr);
	//printf("%d\n", *(DWORD*)arr->blocks[0].elements[0]);
	for (i=0; i<size; i++) {
		//array_get(arr, 0);
		callback(i, array_get(arr, i));
	}
}

float array_last (ARRAY *arr) {
	return array_get(arr, arr->element_size - 1);
}
