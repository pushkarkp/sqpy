/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.h
 */
#ifndef SolveH
#define SolveH

#include "Orientation.h"
#include "Piece.h"
#include "Topics.h"

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int useOnce, TSetOfTopics);
void testOrientations(EPresence, TPath, TSetOfOrientations);

#endif
