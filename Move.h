/**
 * Copyright 2018 Pushkar Piggott
 *
 * Move.h
 */
#ifndef MoveH
#define MoveH

#include "Position.h"
#include "Orientation.h"

///////////////////////////////////////////////////////////////////////////////
typedef struct {
   int d[eDimensions];
} TMove;
const char* moveToString(char*, const TMove*);
typedef TMove TMoves[ePlanes][e2dDimensions][eSigns];

///////////////////////////////////////////////////////////////////////////////
extern const TMoves moves;

///////////////////////////////////////////////////////////////////////////////
int charToMove(const TMove**, int, EOrientation);
void step(TPosition*, const TMove*);

#endif
