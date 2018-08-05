/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.h
 */
#ifndef RepeatH
#define RepeatH

#include "Position.h"
#include "Orientation.h"
#include "Symmetry.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
// Whether a piece's path at an orientation on the plane 
// repeats a previous path orientation.
extern TSet** repeat;
void findRepeat();
TSetOfOrientations rotateSkip(int pc, int path, TSetOfRotations);
TSetOfOrientations reflectSkip(int pc, int path, TSetOfReflectionPlanes);

#endif
