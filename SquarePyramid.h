/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.h
 */
#ifndef SquarePyramidH
#define SquarePyramidH

#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {eAsymmetric, e180, e90} ESymmetry;
const char* symToString(ESymmetry); 
#define HEIGHT 5
#define MARGIN 1
#define SIZE (HEIGHT + 2 * MARGIN)
typedef char TSquarePyramid[SIZE][SIZE][SIZE];
typedef char (*PSquarePyramid)[SIZE][SIZE];

///////////////////////////////////////////////////////////////////////////////
void spClear(TSquarePyramid);
void spInit(TSquarePyramid);
void spEnumerate(TSquarePyramid);
PSquarePyramid spCopy(TSquarePyramid, TSquarePyramid);
int spEqual(TSquarePyramid, TSquarePyramid);
ESymmetry spSymmetry(TSquarePyramid);
char* spRowToString(char*, int y, int z, TSquarePyramid, const char* glyph);
char* spWholeRowToString(char*, int, int, TSquarePyramid, const char* glyph);
void sqFind(TPosition*, char, ESymmetry, TSquarePyramid);
char spGet(TPosition*, TSquarePyramid);
int spSet(TSquarePyramid, char, TPosition*);

#endif
