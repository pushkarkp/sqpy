/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.h
 */
#ifndef SquarePyramidH
#define SquarePyramidH

#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {e0, e180, e90} ERotation;
const char* rotationToString(ERotation); 
typedef enum {eNoReflection, e100Reflection, e010Reflection, e110Reflection = 4, e1T0Reflection = 8} EReflectionPlane;
const char* reflectionToString(char*, int); 
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
ERotation spSymmetry(TSquarePyramid);
char* spRowToString(char*, int y, int z, TSquarePyramid, const char* glyph);
char* spWholeRowToString(char*, int, int, TSquarePyramid, const char* glyph);
void spFind(TPosition*, char, TSquarePyramid);
ERotation spEqualRotation(TSquarePyramid, TSquarePyramid);
int spEqualReflection(TSquarePyramid, TSquarePyramid);

#endif
