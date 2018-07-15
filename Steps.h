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
#include "SquarePyramid.h"

///////////////////////////////////////////////////////////////////////////////
char* stepToString(EPresence, const TPosition*, const char* path, EOrientation);
char* catStep(const char* steps, char* step);
void catSteps(char**, const char* strrot, const char* strsorp, const char*);
char* stepsCopy(const char*);
int stepsPieceCount(const char*);
const char* stepsLast(const char*);
int stepOk(const char*);
int stepsOk(const char*);
TPlace* parseSteps(const char*);
#endif
