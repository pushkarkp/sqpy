/**
 * Copyright 2018 Pushkar Piggott
 *
 * SetOf.h
 */
#ifndef SetOfH
#define SetOfH

///////////////////////////////////////////////////////////////////////////////
typedef int TSet;
typedef const char* (*FSetElementToString)(int);
typedef int (*FSetElementToChar)(int);
int setCount(TSet);
int setGetMax(TSet);
int setGetSingle(TSet);
char* setToString(TSet, FSetElementToString, const char*);
char* setToChars(TSet, FSetElementToChar, const char*);
#define SET_ALL 0xffffffff
#define SET_MAX_SIZE (sizeof(TSet) * 8)
#define SET_LAST 0x80000000
#define SET_ALL_OF(count) ((1 << (count)) - 1)
#define SET_WITH(set, el) ((set) | (1 << (el)))
#define SET_WITHOUT(set, el) ((set) & ~(1 << (el)))
#define SET_HAS(set, el) ((set) & (1 << (el)))
#define SET_NOT(set, count) (~(set) & SET_ALL_OF(count))

#endif
