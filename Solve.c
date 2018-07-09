/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Repeat.h"
#include "Piece.h"
#include "Position.h"
#include "Steps.h"
#include "SetOf.h"
#include "Topics.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int useOnce = 0;

///////////////////////////////////////////////////////////////////////////////
int countUse(int* use) {
   int count = 0;
   int i;
   for (i = 0; i < pieceCount; ++i) {
      count += use[i];
   }
   return count;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations getSkip(const TPosition* p, TPlace* sp) {
   TSetOfOrientations skip = 0;
   ERotation rot = 0;
   if (ON_AXIS(p)) {
      rot = spEqualRotate(sp, sp);
      skip |= rotateSkip(rot);
   }
   TSetOfReflectionPlanes sorp = onPlanes(p);
   if (sorp) {
      sorp &= spEqualReflect(sp, sp);
      skip |= reflectSkip(sorp);
   }
   if (skip && IS_TOPIC(eTopicSymmetries)) {
      displayWide(ePyramid, 0);
      char posBuf[POS_BUF_SIZE];
      printf("Symmetries for %s (%s axis)%s", 
             posToString(posBuf, p), ON_AXIS(p) ? "on" : "off", EOL);
      printf("Rotations %s%s",
             rotationToString(rot), EOL);
      char* strplanes = setToString(onPlanes(p), reflectionPlaneToString);
      char* strsorp = setToString(sorp, reflectionPlaneToString);
      printf("Reflections %s: %s%s", strplanes, strsorp, EOL); 
      free(strplanes);
      free(strsorp);
      char* fmt = "Skip %s%s";
      char* strsoor = 0;
      if (setCount(skip) > eOrientations / 2) {
         fmt = "Check %s%s";
         strsoor = setToString(SET_NOT(skip, eOrientations), orToString);
      } else {
         strsoor = setToString(skip, orToString);
      }
      printf(fmt, strsoor, EOL);
      free(strsoor);
      display1(ePyramid, sp);
   }
   return skip;
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, const int* used, const TPosition* pos, const char* steps, TPlace* sp, TSetOfOrientations skip) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      EOrientation or;
      for (or = 0; or < eOrientations; ++or) {
         if (skip && SET_HAS(skip, or)) {
//printf("continue (skip)\r\n");
            continue;
         }
         if (!SET_HAS(repeat[pc][path], or)
          && pcWalk(eAbsent, pieces[pc][path], or, pos, sp) == eAbsent) {
            spCopy(newsp, sp);
            SP_SET(newsp, pc, pos);
            pcWalk(pc, pieces[pc][path], or, pos, newsp);
            int* newused = pcDupInstanceCounts(used);
            ++newused[pc];
            TSet sop = pcInstanceCountSet(newused);
            const char* newstep = stepToString(pc, pos, pieces[pc][path], or);
            char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
            if (!solIsUniqueSymmetric(sop, newsteps, newsp)) {
printf("continue (duplicate)\r\n");
               continue;
            }
            if (IS_TOPIC(eTopicProgress)) {
               if (IS_TOPIC(eTopicSteps)) {
                  printf("%s%s", newsteps, EOL);
               }
               displayWide(ePyramid, newsp);
            }
            TPosition newpos[eDimensions];
            spFind(newpos, eAbsent, newsp);
            if (newpos[eX] == -1) {
               if (solAddUniqueSymmetric(sop, newsteps, newsp)) {
                  solutions = 1;
               }
            } else {
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               TSetOfOrientations newskip = 0;
               if (skip) {
                  newskip = getSkip(newpos, newsp);
               }
               EPresence next;
               for (next = eFirstPiece; next < pieceCount; ++next) {
                  if (newused[next] < pieceMaxInstances[next]) {
                     ++togo;
                     int s = search(next, newused, newpos, newsteps, newsp, newskip);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || (!togo && useOnce)) {
                  if (solAddUniqueSymmetric(pcSumInstanceCounts(newused), newsteps, newsp)
                   && !fork) {
                     next_solutions = 1;
                  }
               }
               solutions += next_solutions;
            }
            free(newused);
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int once) {
   useOnce = once;
   int solutions = 0;
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {0, 0, 0};
   EPresence end = pc + 1;
   if (pc == eAbsent) {
      pc = eFirstPiece;
      end = pieceCount;
   }
   for (; pc < end; ++pc) {
      solInit();
      TSet used = 0;
      spInit(sp);
      int sol = search(pc, pieceZeroInstances, pos, "", sp, getSkip(pos, sp));
      solutions += sol;
      displayWide(ePyramid, 0);
      int max = solMaxPieceCount();
      printf("%d solutions starting with %c %d pieces%s", sol,  GLYPH(pc), max, EOL);
      int n;
      for (n = 1; n < pieceCount; ++n) {
         int count = solCountForPieceCount(n);
         if (count > 0) {
            printf("%d with %d pieces%s", count, n, EOL);
            solDisplayByPieceCount(n, ePyramid);
         }
      }
   }
   free(sp);
   return solutions;
}
