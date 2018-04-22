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
typedef int TMove;
const char* moveToString(char*, const TMove*);

///////////////////////////////////////////////////////////////////////////////
#define MOVE_BUF_SIZE 16
int charToMove(const TMove**, int, EOrientation);
void step(TPosition*, const TMove*);

#endif
