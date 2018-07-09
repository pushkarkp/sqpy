/**
 * Copyright 2018 Pushkar Piggott
 *
 * Steps.c
 */

#include "Steps.h"
#include "Piece.h"
#include "SetOf.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define STEP_SIZE 7

///////////////////////////////////////////////////////////////////////////////
// returns size in multiples of (STEP_SIZE + maxPathLength)
int getUnitSize(int len) {
   int f = len / (STEP_SIZE + maxPathLength) 
         + (len % (STEP_SIZE + maxPathLength) ? 1 : 0);
   return f * (STEP_SIZE + maxPathLength);
}

///////////////////////////////////////////////////////////////////////////////
char* stepToString(EPresence pc, const TPosition* pos, const char* path, EOrientation or) {
   char* step = (char*)malloc(STEP_SIZE + maxPathLength);
   sprintf(step, 
           "%c%1d%1d%1d%s%02d", 
           GLYPH(pc), pos[eX], pos[eY], pos[eZ], path, or);
   return step;
}

///////////////////////////////////////////////////////////////////////////////
char* catStep(const char* steps, char* step) {
   if (!step) {
      static int i = 0;
      ++i;
   }
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
void catSteps(char** steps, const char* strrot, const char* strsorp, const char* newsteps) {
   int lensteps = strlen(*steps);
   int lenrot = strlen(strrot) + 1;
   int lensorp = strlen(strsorp) + 1;
   // add 3 for "; " and terminator
   *steps = (char*)realloc(*steps, getUnitSize(lensteps + 3 + lenrot + lensorp + strlen(newsteps)));
   char* addsteps = *steps + lensteps;
   *addsteps++ = ';';
   *addsteps++ = ' ';
   if (strrot[0]) {
      strcpy(addsteps, strrot);
      addsteps += lenrot;
      *(addsteps - 1) = ' ';
   }
   if (strsorp[0]) {
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

///////////////////////////////////////////////////////////////////////////////
int stepsPieceCount(const char* steps) {
   int n = 1;
   int i;
   for (i = 0; steps[i]; ++i) {
      if (steps[i] == ' ') {
         ++n;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
const char* stepsLast(const char* steps) {
   const char* last = steps;
   int i;
   for (i = 0; steps[i]; ++i) {
      if (steps[i] == ' ') {
         last = steps + i + 1;
      }
   }
   return last;
}
