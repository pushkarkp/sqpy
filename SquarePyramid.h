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
#define SP_GET(pat, sp) \
   ((sp)[MARGIN + (pat)->d[eZ]][MARGIN + (pat)->d[eY]][MARGIN + (pat)->d[eX]])
#define SP_SET(sp, c, pat) \
   ((sp)[MARGIN + (pat)->d[eZ]][MARGIN + (pat)->d[eY]][MARGIN + (pat)->d[eX]] = (c))

void spClear(TSquarePyramid);
void spInit(TSquarePyramid);
void spEnumerate(TSquarePyramid);
PSquarePyramid spCopy(TSquarePyramid, const TSquarePyramid);
int spEqual(const TSquarePyramid, const TSquarePyramid);
char* spRowToString(char*, int y, int z, const TSquarePyramid);
char* spWholeRowToString(char*, int, int, const TSquarePyramid);
void spFind(TPosition*, char, const TSquarePyramid);
ERotation spEqualRotate(const TSquarePyramid, const TSquarePyramid);
TSetOfReflectionPlanes spEqualReflect(const TSquarePyramid, const TSquarePyramid);

#endif
