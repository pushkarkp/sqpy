/**
 * Copyright 2018 Pushkar Piggott
 *
 * Steps.h
 */
#ifndef StepsH
#define StepsH

#include "Presence.h"
#include "Position.h"
#include "Orientation.h"
#include "Symmetry.h"

///////////////////////////////////////////////////////////////////////////////
char* stepToString(EPresence, const TPosition*, const char* path, EOrientation);
char* catStep(const char* steps, char* step);
void catSteps(char** steps, ERotation, TSetOfReflectionPlanes, const char* newsteps);
char* stepsCopy(const char*);

#endif
