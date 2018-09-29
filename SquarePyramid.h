/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.h
 */
#ifndef SquarePyramidH
#define SquarePyramidH

#include "Position.h"
#include "Symmetry.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
typedef char TPlace;

///////////////////////////////////////////////////////////////////////////////
extern int spHeight;
extern int spX;
extern int spXY;
extern int spXYZ;
// a whole second space to hold the play count
extern int spXYZ2;

///////////////////////////////////////////////////////////////////////////////
#define MARGIN 1
#define SPS(i) ((i) * spXYZ2 * sizeof(TPlace))
#define SP_NEW(i) ((TPlace*)malloc(SPS(i)))
#define SP_EXTEND(sps, i) ((TPlace*)realloc(sps, SPS(i)))
#define SP_XYZ(x, y, z) (((MARGIN + z) * spXY + (MARGIN + y) * spX + (MARGIN + x)) * sizeof(TPlace))
#define SP_POS(p) SP_XYZ((p)[eX], (p)[eY], (p)[eZ])
#define SP_GET(p, sp) ((sp)[SP_POS(p)])
#define SP_SET(sp, c, p) ((sp)[SP_POS(p)] = (c))
#define SP_SET_PC_N(sp, c, n, p) { (sp)[SP_POS(p)] = (c); ((sp) + spXYZ)[SP_POS(p)] = (n); } 
#define SP_GET_N(sp, p) (((sp) + spXYZ)[SP_POS(p)]) 

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
void spTestVolume();
int spFind(TPosition*, TPlace, const TPlace*, TSetOfReflectionPlanes);
void spTestSequence(TSetOfReflectionPlanes);
TSet spEqualRotate(const TPlace*, const TPlace*, int byz);
TSet spEqualReflect(const TPlace*, const TPlace*, int byz);
typedef TSet (*FEqualSym)(const TPlace*, const TPlace*, int);

#endif
