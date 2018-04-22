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
#define MARGIN 1
typedef char TPlace;

extern int spHeight;
extern int spX;
extern int spXY;
extern int spXYZ;

///////////////////////////////////////////////////////////////////////////////
#define SPS(i) ((i) * spXYZ * sizeof(TPlace))
#define SP_NEW(i) ((TPlace*)malloc(SPS(i)))
#define SP_EXTEND(i, sps) ((TPlace*)realloc(sps, SPS(i)))
#define SP_XYZ(x, y, z) (((MARGIN + z) * spXY + (MARGIN + y) * spX + (MARGIN + x)) * sizeof(TPlace))
#define SP_GET(p, sp) ((sp)[SP_XYZ((p)[eX], (p)[eY], (p)[eZ])])
#define SP_SET(sp, c, p) ((sp)[SP_XYZ((p)[eX], (p)[eY], (p)[eZ])] = (c))

void spSetHeight(int);
int spGetOffset(int x, int y, int z);
void spClear(TPlace*);
void spInit(TPlace*);
void spEnumerate(TPlace*);
TPlace* spCopy(TPlace*, const TPlace*);
int spEqual(const TPlace*, const TPlace*);
char* spRowToString(char*, int y, int z, const TPlace*);
char* spWholeRowToString(char*, int, int, const TPlace*);
void spFind(TPosition*, TPlace, const TPlace*);
ERotation spEqualRotate(const TPlace*, const TPlace*);
TSetOfReflectionPlanes spEqualReflect(const TPlace*, const TPlace*);

#endif
