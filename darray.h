
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

typedef struct _ARRAY_BLOCK {
	float *elements;
} ARRAY_BLOCK;

typedef struct _ARRAY {
	ARRAY_BLOCK *blocks;
	DWORD element_size;
	DWORD block_size;
	DWORD capacity;		/* maximum capacity (ARRAY_BLOCK_CAPACITY * block_size) */
} ARRAY, *PARRAY;

ARRAY *array_new ();
void array_destroy (ARRAY *arr);
void array_add (ARRAY *arr, float val);
DWORD array_size (ARRAY *arr);
float array_get (ARRAY *arr, DWORD index);
bool array_set (ARRAY *arr, DWORD index, float val);
void array_remove (ARRAY *arr, DWORD index);
void array_clear (ARRAY *arr);
void array_iterate (ARRAY *arr, void (*callback)(DWORD index, float val));
bool array_remove_range (ARRAY *arr, DWORD fromIndex, DWORD toIndex);
float array_last (ARRAY *arr);




