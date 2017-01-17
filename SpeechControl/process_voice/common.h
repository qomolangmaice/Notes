#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long long  uint64;

typedef signed char  int8;
typedef signed short int16;
typedef signed int   int32;
typedef signed long long int64;

#define ITEM_OF(array)	    (int)(sizeof(array) / sizeof(array[0]))

#endif
