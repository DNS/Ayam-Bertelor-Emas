
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


ARRAY_FLOAT *arrayFloat_new () {
	ARRAY_FLOAT *arr;
	arr = malloc(sizeof(ARRAY_FLOAT));
	if (arr == NULL) return NULL;
	arr->blocks = malloc(sizeof(ARRAY_FLOAT_BLOCK));
	arr->blocks[0].elements = malloc(sizeof(float) * ARRAY_BLOCK_CAPACITY);
	arr->element_size = 0;
	arr->block_size = 1;
	arr->capacity = ARRAY_BLOCK_CAPACITY;
	return arr;
}


void arrayFloat_destroy (ARRAY_FLOAT *arr) {
	arrayFloat_clear(arr);
	free(arr->blocks[0].elements);
	free(arr->blocks);
	free(arr);
}

void arrayFloat_add (ARRAY_FLOAT *arr, float val) {
	DWORD curr_index;
	
	if (arr->element_size < arr->capacity) {
		curr_index = ARRAY_BLOCK_CAPACITY - (arr->capacity - arr->element_size);
		
		arr->blocks[arr->block_size-1].elements[curr_index] = val;
		
		//printf("%d\n", *(DWORD*)arr->blocks[0].elements[0]);
		
		arr->element_size++;
	} else {
		arr->blocks = realloc(arr->blocks, (arr->block_size+1) * sizeof(ARRAY_FLOAT_BLOCK));
		arr->blocks[arr->block_size].elements = malloc(sizeof(float) * ARRAY_BLOCK_CAPACITY);
		
		
		arr->blocks[arr->block_size].elements[0] = val;
		
		arr->block_size++;
		arr->element_size++;
		arr->capacity += ARRAY_BLOCK_CAPACITY;
	}
}

DWORD arrayFloat_size (ARRAY_FLOAT *arr) {
	return arr->element_size;
}

float arrayFloat_get (ARRAY_FLOAT *arr, DWORD index) {
	
	//printf("%d\n", *(DWORD*)arr->blocks[0].elements[0]);
	
	DWORD block_index, el_index;
	block_index = index / ARRAY_BLOCK_CAPACITY;
	el_index = index % ARRAY_BLOCK_CAPACITY;
	
	return arr->blocks[block_index].elements[el_index];
}


bool arrayFloat_set (ARRAY_FLOAT *arr, DWORD index, float val) {
	DWORD block_index;
	DWORD el_index;
	
	if (arrayFloat_size(arr)-1 < index)
		return false;
	
	block_index = index / ARRAY_BLOCK_CAPACITY;
	el_index = index % ARRAY_BLOCK_CAPACITY;
	arr->blocks[block_index].elements[el_index] = val;
	
	return true;
}



void arrayFloat_remove (ARRAY_FLOAT *arr, DWORD index) {
	DWORD el_index, size, i, remainder;
	
	size = arrayFloat_size(arr);
	
	// TODO: SLOW, OPTIMIZE IN array_removeRange()
	for (i=index; i<size; i++) {
		if (i < size-1)
			arrayFloat_set(arr, i, arrayFloat_get(arr, i+1));
	}
	
	el_index = index % ARRAY_BLOCK_CAPACITY;
	
	remainder = arr->capacity - arr->element_size;
	//printf("%d\n", t);
	if (el_index == 0 && arr->block_size > 1 && remainder > ARRAY_BLOCK_CAPACITY) {
		arr->block_size--;
		free(arr->blocks[arr->block_size].elements);
		//printf("el_index: %d\nblock_size: %d\n", el_index, arr->block_size);
		arr->blocks = realloc(arr->blocks, arr->block_size * sizeof(ARRAY_FLOAT_BLOCK));
		arr->capacity = ARRAY_BLOCK_CAPACITY * arr->block_size;
	}
	
	
	
	if (arr->element_size != 0)
		arr->element_size--;
	
}

void arrayFloat_clear (ARRAY_FLOAT *arr) {
	DWORD i, block_size;
	
	block_size = arrayFloat_size(arr);
	
	for (i=1; i<arr->block_size; i++)
		free(arr->blocks[i].elements);
	
	arr->blocks = realloc(arr->blocks, 1 * sizeof(ARRAY_FLOAT_BLOCK));
	arr->element_size = 0;
	arr->block_size = 1;
	arr->capacity = ARRAY_BLOCK_CAPACITY;
}

/* TODO: SLOW, NEED OPTIMIZATION */
bool arrayFloat_trim (ARRAY_FLOAT *arr, DWORD fromIndex, DWORD toIndex) {
	DWORD i, total_remove, removed_block, size, new_element_size;
	DWORD startBlock, endCopyBlock, endBlock, currBlock, currIndex;
	DWORD old_block_size, new_block_size;
	div_t x, z;
	
	size = arrayFloat_size(arr);
	
	if (fromIndex > toIndex || fromIndex < 0 || toIndex > size || arr->element_size <= 0) return false;
	
	
	/*
	// SLOW METHOD
	size = arrayFloat_size(arr);
	total_remove = toIndex - fromIndex + 1;
	removed_block = (toIndex - fromIndex) / ARRAY_BLOCK_CAPACITY;
	
	//printf("%d\n", total_remove);
	for (i=fromIndex; i<fromIndex+total_remove; i++)
		arrayFloat_remove(arr, fromIndex);	// SLOW !!!
	
	arr->element_size -= total_remove;
	arr->block_size -= total_remove / ARRAY_BLOCK_CAPACITY;
	arr->capacity = ARRAY_BLOCK_CAPACITY * arr->block_size;
	*/
	total_remove = toIndex - fromIndex + 1;
	
	for (i=fromIndex; i<size; i++) {
		x = div(i, ARRAY_BLOCK_CAPACITY);
		z = div(i + total_remove, ARRAY_BLOCK_CAPACITY);
		if (i + total_remove < size) {
			arr->blocks[x.quot].elements[x.rem] = arr->blocks[z.quot].elements[z.rem];
		}
		
	}
	
	new_element_size = arr->element_size - total_remove;
	
	if (new_element_size >= 0) {
		arr->element_size -= total_remove;
	}
	
	if (arr->block_size > 1) {
		old_block_size = arr->block_size; 
		arr->block_size -= total_remove / ARRAY_BLOCK_CAPACITY;
		new_block_size = arr->block_size;
		
		for (i=old_block_size-1; i>=new_block_size; i--) {
			free(arr->blocks[i].elements);
		}
		
		arr->blocks = realloc(arr->blocks, arr->block_size * ARRAY_BLOCK_CAPACITY * sizeof(float));
	}
	
	arr->capacity = ARRAY_BLOCK_CAPACITY * arr->block_size;
	
	return true;
}

/* shrink to size */
bool arrayFloat_shrink(ARRAY_FLOAT *arr, DWORD shrink_size) {
	DWORD size, shrink_index;
	size = arrayFloat_size(arr);
	
	if (shrink_size >= size) return false;
	
	arrayFloat_trim(arr, shrink_size, size-1);
	
	return true;
}

void arrayFloat_iterator (ARRAY_FLOAT *arr, void (*callback)(DWORD index, float val)) {
	DWORD i, size;
	size = arrayFloat_size(arr);
	for (i=0; i<size; i++) {
		callback(i, arrayFloat_get(arr, i));
	}
}

float arrayFloat_last (ARRAY_FLOAT *arr) {
	return arrayFloat_get(arr, arr->element_size - 1);
}

DWORD arrayFloat_indexOf (ARRAY_FLOAT *arr, float val) {
	DWORD index, i, size;
	size = arrayFloat_size(arr);
	for (i=0; i<size; i++) {
		if (val == arrayFloat_get(arr, i))
			return i;
	}
	
	return -1;
}

DWORD arrayFloat_lastIndexOf (ARRAY_FLOAT *arr, float val) {
	DWORD index, i, size;
	size = arrayFloat_size(arr);
	for (i=size-1; i>=0; i++) {
		if (val == arrayFloat_get(arr, i))
			return i;
	}
	
	return -1;
}

