/**
 * Copyright 2018 Pushkar Piggott
 *
 * Steps.c
 */

#include "Steps.h"
#include "Piece.h"
#include "Alloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define STEP_SIZE 7

///////////////////////////////////////////////////////////////////////////////
// returns size in multiples of (STEP_SIZE + pathMaxLength)
int getUnitSize(int len) {
   int f = len / (STEP_SIZE + pathMaxLength) 
         + (len % (STEP_SIZE + pathMaxLength) ? 1 : 0);
   return f * (STEP_SIZE + pathMaxLength);
}

///////////////////////////////////////////////////////////////////////////////
char* stepToString(EPresence pc, const TPosition* pos, int path, EOrientation or) {
   char* step = (char*)malloc(STEP_SIZE + pathMaxLength);
   sprintf(step, 
           "%c%1d%1d%1d%s%02d", 
           GLYPH(pc), pos->d[eX], pos->d[eY], pos->d[eZ], pieces[pc][path], or);
   return step;
}

///////////////////////////////////////////////////////////////////////////////
char* catStep(const char* steps, char* step) {
   int lensteps = strlen(steps);
   if (!lensteps) {
      return step;
   }
   // add 2 for ' ' and terminator
   char* newsteps = (char*)malloc(getUnitSize(lensteps + 2 + strlen(step)));
   strcpy(newsteps, steps);
   newsteps[lensteps] = ' ';
   strcpy(newsteps + lensteps + 1, step);
   free(step);
   return newsteps;
}

///////////////////////////////////////////////////////////////////////////////
void catSteps(char** steps, ERotation rot, TSetOfReflectionPlanes sorp, const char* newsteps) {
   int lensteps = strlen(*steps);
   const char* strrot = "";
   int lenrot = 0;
   if (rot) {
      strrot = rotationToString(rot);
      lenrot = strlen(strrot) + 1;
   }
   char sorpBuf[SORP_BUF_SIZE];
   const char* strsorp = "";
   int lensorp = 0;
   if (sorp) {
      strsorp = sorpToString(sorpBuf, sorp);
      lensorp = strlen(strsorp) + 1;
   }
   // add 3 for "; " and terminator
   *steps = (char*)realloc(*steps, getUnitSize(lensteps + 3 + lenrot + lensorp + strlen(newsteps)));
   char* addsteps = *steps + lensteps;
   *addsteps++ = ';';
   *addsteps++ = ' ';
   if (rot) {
      strcpy(addsteps, strrot);
      addsteps += lenrot;
      *(addsteps - 1) = ' ';
   }
   if (sorp) {
      strcpy(addsteps, strsorp);
      addsteps += lensorp;
      *(addsteps - 1) = ' ';
   }
   strcpy(addsteps, newsteps);
}

///////////////////////////////////////////////////////////////////////////////
char* stepsCopy(const char* steps) {
   char* newsteps = (char*)malloc(getUnitSize(strlen(steps)));
   strcpy(newsteps, steps);
   return newsteps;
}
