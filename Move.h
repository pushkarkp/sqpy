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
#define MOVE_BUF_SIZE 16
const char* moveToString(char*, const TMove*);

///////////////////////////////////////////////////////////////////////////////
int moveFromChar(const TMove**, int, EOrientation);
void moveStep(TPosition*, const TMove*);

#endif
