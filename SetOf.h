/**
 * Copyright 2018 Pushkar Piggott
 *
 * SetOf.h
 */
#ifndef SetOfH
#define SetOfH

///////////////////////////////////////////////////////////////////////////////
int setCount(int);
#define SET_ALL(count) ((1 << (count)) - 1)
#define SET_WITH(sop, pres) ((sop) | (1 << (pres)))
#define SET_WITHOUT(sop, pres) ((sop) & ~(1 << (pres)))
#define SET_HAS(sop, pres) ((sop) & ((1 << pres)))

#endif
