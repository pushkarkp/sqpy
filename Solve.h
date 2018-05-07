/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.h
 */
#ifndef SolveH
#define SolveH

#include "Orientation.h"
#include "Presence.h"

///////////////////////////////////////////////////////////////////////////////
int solve(int pc, int useOnce);

void testSpFind();
void testPathSymmetry(EOrientation);
void testPieceOrientations(EPresence, const char* path);

#endif
