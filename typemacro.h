


#ifndef _TYPEMACRO_H
#define _TYPEMACRO_H


#ifdef __cplusplus
extern "C" {
#endif



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL DWORD
#endif

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

#ifndef OWORD
#ifdef __int128
typedef __int128 OWORD;
#endif
#endif





#ifndef FLOAT32
typedef float FLOAT32;
#endif

#ifndef FLOAT64
typedef double FLOAT64;
#endif

#ifndef FLOAT80
typedef long double FLOAT80;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif



#ifndef WCHAR
typedef WCHAR wchar_t;
#endif

#ifndef VOID
typedef VOID void;
#endif








#ifdef __cplusplus
}
#endif

#endif
