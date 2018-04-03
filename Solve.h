/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.h
 */
#ifndef SolveH
#define SolveH

#include "Piece.h"
#include "Position.h"
#include "Orientation.h"
#include "SquarePyramid.h"

///////////////////////////////////////////////////////////////////////////////
int solve();
int walk(EPresence, const char*, EOrientation, TPosition, PSquarePyramid);

void testSpFind();
void testPathSymmetry(EOrientation);
void testPieceOrientations(EPresence, const char* path);

#endif
