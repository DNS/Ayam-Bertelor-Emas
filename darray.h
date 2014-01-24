
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tchar.h>
#include <windows.h>

#ifndef _DARRAY_H
#define _DARRAY_H


//#define bool BOOL
//#define true TRUE
//#define false FALSE


#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef QWORD
typedef unsigned long long QWORD;
#endif



#define ARRAY_BLOCK_CAPACITY 512	/* default size 8 */

typedef struct _ARRAY_FLOAT_BLOCK {
	float *elements;
} ARRAY_FLOAT_BLOCK;

typedef struct _ARRAY {
	ARRAY_FLOAT_BLOCK *blocks;
	DWORD element_size;
	DWORD block_size;
	DWORD capacity;		/* maximum capacity (ARRAY_BLOCK_CAPACITY * block_size) */
} ARRAY_FLOAT, *PARRAY_FLOAT;

ARRAY_FLOAT *arrayFloat_new ();
void arrayFloat_destroy (ARRAY_FLOAT *arr);
void arrayFloat_add (ARRAY_FLOAT *arr, float val);
DWORD arrayFloat_size (ARRAY_FLOAT *arr);
float arrayFloat_get (ARRAY_FLOAT *arr, DWORD index);
bool arrayFloat_set (ARRAY_FLOAT *arr, DWORD index, float val);
void arrayFloat_remove (ARRAY_FLOAT *arr, DWORD index);
void arrayFloat_clear (ARRAY_FLOAT *arr);
void arrayFloat_iterator (ARRAY_FLOAT *arr, void (*callback)(DWORD index, float val));
bool arrayFloat_trim (ARRAY_FLOAT *arr, DWORD fromIndex, DWORD toIndex);
float arrayFloat_last (ARRAY_FLOAT *arr);
bool arrayFloat_shrink(ARRAY_FLOAT *arr, DWORD shrink_size);

/* the index of the last occurrence of the specified element in this list, or -1 if this list does not contain the element */
DWORD arrayFloat_indexOf (ARRAY_FLOAT *arr, float val);
DWORD arrayFloat_lastIndexOf (ARRAY_FLOAT *arr, float val);

#endif