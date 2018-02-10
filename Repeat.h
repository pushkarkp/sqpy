/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.h
 */
#ifndef RepeatH
#define RepeatH

#include "Piece.h"
#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
// Whether a piece path at an orientation repeats a previous path orientation.
extern char repeat[ePresences][MAX_PATHS][e2dDimensions][eSigns][eSigns];
void findRepeat();

#endif
