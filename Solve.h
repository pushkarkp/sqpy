/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.h
 */
#ifndef SolveH
#define SolveH

#include "Piece.h"
#include "Position.h"
#include "SquarePyramid.h"

///////////////////////////////////////////////////////////////////////////////
int solve();
int walk(EPresence, const char*, const TOrientation*, TPosition, PSquarePyramid);

#endif
