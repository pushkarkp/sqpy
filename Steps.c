/**
 * Copyright 2018 Pushkar Piggott
 *
 * Steps.c
 */

#include "Steps.h"
#include "Piece.h"
#include "Path.h"
#include "SetOf.h"
#include "Display.h"
#include "Topics.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STEP_SIZE (4 + OR_BUF_SIZE)

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
           "%c%1d%1d%1d%s%s", 
           GLYPH(pc), pos[eX], pos[eY], pos[eZ], orToString(or), path);
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

///////////////////////////////////////////////////////////////////////////////
char** eachStep(char* steps) {
   int n = stepsPieceCount(steps);
   char** step = (char**)malloc((n + 1) * sizeof(char*));
   int s = 0;
   step[s++] = steps;
   int i;
   for (i = 1; steps[i]; ++i) {
      if (steps[i] == ' ') {
         steps[i] = 0;
      } else if (steps[i - 1] == 0) {
         step[s++] = steps + i;
      }
   }
   step[s++] = 0;
   return step;
}

///////////////////////////////////////////////////////////////////////////////
int stepOk(const char* step) {
   return isalpha(step[0])
       && isdigit(step[1])
       && isdigit(step[2])
       && isdigit(step[3])
       && orientOk(step + 4)
       && pathOk(step + (orientIsShort(step + 4) ? 5 : OR_BUF_SIZE + 3));
}

///////////////////////////////////////////////////////////////////////////////
int stepReport(const char* step, const char* prefix) {
   if (!isalpha(step[0])) {
      reportErr(1, prefix, step, 0, " - letter expected");
      return 0;
   }
   int i;
   for (i = 1; i < 4; ++i) {
      if (!isdigit(step[i])) {
         reportErr(1, prefix, step, i, " - digit expected");
         return 0;
      }
   }
   return orientReport(step, 4)
       && pathReport(step, (orientIsShort(step + 4) ? 5 : OR_BUF_SIZE + 3));
}

///////////////////////////////////////////////////////////////////////////////
int stepsOk(const char* given) {
   char* steps = strdup(given);
   char** step = eachStep(steps);
   int i;
   for (i = 0; step[i]; ++i) {
      if (!stepOk(step[i])) {
         return 0;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
TPlace* parseSteps(const char* given) {
   char* steps = strdup(given);
   char** step = eachStep(steps);
   TPlace* sp = SP_NEW(1);
   spInit(sp);
   // show any collision
   displayTopicsAll();
   int i;
   for (i = 0; step[i]; ++i) {
      EPresence pc = PIECE(step[i][0]);
      TPosition pos[eDimensions] = {step[i][1] - '0', step[i][2] - '0', step[i][3] - '0'};
      EOrientation or = stringToOr(step[i] + 4);
      TPath path = step[i] + 4 + ((orientIsShort(step[i] + 4) ? 1 : OR_BUF_SIZE - 1));
      if (eAbsent != SP_GET(pos, sp)
       || eAbsent != pcWalk(pc, 0, path, or, pos, sp)) {
         free(sp);
         sp = 0;
         break;
      }
      SP_SET(sp, pc, pos);
   }
   displayTopicsRevert();
   free(steps);
   free(step);
   return sp;
}
