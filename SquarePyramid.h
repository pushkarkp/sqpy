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
typedef char TPlace;

///////////////////////////////////////////////////////////////////////////////
extern int spHeight;
extern int spX;
extern int spXY;
extern int spXYZ;

///////////////////////////////////////////////////////////////////////////////
#define MARGIN 1
#define SPS(i) ((i) * spXYZ * sizeof(TPlace))
#define SP_NEW(i) ((TPlace*)malloc(SPS(i)))
#define SP_EXTEND(sps, i) ((TPlace*)realloc(sps, SPS(i)))
#define SP_XYZ(x, y, z) (((MARGIN + z) * spXY + (MARGIN + y) * spX + (MARGIN + x)) * sizeof(TPlace))
#define SP_POS(p) SP_XYZ((p)[eX], (p)[eY], (p)[eZ])
#define SP_GET(p, sp) ((sp)[SP_POS(p)])
#define SP_SET(sp, c, p) ((sp)[SP_POS(p)] = (c))

void spSetHeight(int);
void spClear(TPlace*);
void spInit(TPlace*);
void spInitCube(TPlace*);
void spEnumerate(TPlace*);
TPlace* spCopy(TPlace*, const TPlace*);
int spEqual(const TPlace*, const TPlace*);
char* spRowToString(char*, int y, int z, const TPlace*);
char* spCubeRowToString(char*, int, int, const TPlace*);
char* spWholeRowToString(char*, int, int, const TPlace*);
void spTestPyramid();
void spFind(TPosition*, TPlace, const TPlace*);
void spTestOrder();
ERotation spEqualRotate(const TPlace*, const TPlace*);
TSetOfReflectionPlanes spEqualReflect(const TPlace*, const TPlace*);

#endif
