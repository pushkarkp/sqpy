/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.h
 */
#ifndef SquarePyramidH
#define SquarePyramidH

#include "Position.h"
#include "Symmetry.h"

///////////////////////////////////////////////////////////////////////////////
#define HEIGHT 5
#define MARGIN 1
#define SP_SIZE (HEIGHT + 2 * MARGIN)
typedef char TSquarePyramid[SP_SIZE][SP_SIZE][SP_SIZE];
typedef char (*PSquarePyramid)[SP_SIZE][SP_SIZE];

///////////////////////////////////////////////////////////////////////////////
void spClear(TSquarePyramid);
void spInit(TSquarePyramid);
void spEnumerate(TSquarePyramid);
PSquarePyramid spCopy(TSquarePyramid, TSquarePyramid);
int spEqual(TSquarePyramid, TSquarePyramid);
char spGet(TPosition*, TSquarePyramid);
void spSet(TSquarePyramid, char, TPosition*);
char* spRowToString(char*, int y, int z, TSquarePyramid);
char* spWholeRowToString(char*, int, int, TSquarePyramid);
void spFind(TPosition*, char, TSquarePyramid);
ERotation spEqualRotate(TSquarePyramid, TSquarePyramid);
TSetOfReflectionPlanes spEqualReflect(TSquarePyramid, TSquarePyramid);

#endif
