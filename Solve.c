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

static int fill = 0;

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
TSetOfOrientations getSkip(EPresence pc, int path, const TPosition* p, TPlace* sp, 
                           TSetOfRotations sorn, TSetOfReflectionPlanes sorp) {
   TSetOfOrientations skip = 0;
   if (sorn) {
      skip |= rotateSkip(pc, path, sorn);
   }
   if (sorp) {
      skip |= reflectSkip(pc, path, sorp);
   }
   if (skip && IS_TOPIC(eTopicSymmetries)) {
      displayWide(ePyramid, 0);
      char posBuf[POS_BUF_SIZE];
      printf("Symmetries for %s (%s axis)%s", 
             posToString(posBuf, p), ON_AXIS(p) ? "on" : "off", EOL);
      char* strsorn = setToString(sorn, rotationToString);
      printf("Rotations %s%s", strsorn, EOL);
      free(strsorn);
      char* strsorp = setToChars(sorp, reflectionPlaneToChar);
      printf("Reflections: %s%s", strsorp, EOL); 
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
int search(EPresence pc, const int* used, const TPosition* pos, const char* steps, TPlace* sp, 
           TSetOfRotations sorn, TSetOfReflectionPlanes sorp) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      TSetOfOrientations skip = getSkip(pc, path, pos, sp, sorn, sorp);
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
            TSet sopc = SET_WITH(pcInstanceCountSet(used), pc);
            char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
            if (!solIsUniqueSymmetric(sopc, newsteps, newsp)) {
               if (IS_TOPIC(eTopicProgress)) {
                  printf("skip duplicate %s%s", newsteps, EOL);
               }
               free(newsteps);
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
               if (solAddUniqueSymmetric(sopc, newsteps, newsp)) {
                  solutions = 1;
               }
            } else {
               if (sorn) {
                  sorn = 0;
                  if (ON_AXIS(newpos)) {
                     sorn = spEqualRotate(newsp, newsp);
                  }
               }
               if (sorp) {
                  sorp = 0;
                  TSetOfReflectionPlanes s = onReflectionPlanes(newpos);
                  if (s) {
                     sorp = (s | spEqualReflect(newsp, newsp));
                  }
               }
               int* newused = pcDupInstanceCounts(used);
               ++newused[pc];
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               EPresence next;
               for (next = eFirstPiece; next < pieceCount; ++next) {
                  if (fill || newused[next] < pieceMaxInstances[next]) {
                     ++togo;
                     int s = search(next, newused, newpos, newsteps, newsp, sorn, sorp);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || togo == 0) {
                  if (solAddUniqueSymmetric(sopc, newsteps, newsp)
                   && !fork) {
                     next_solutions = 1;
                  }
               }
               solutions += next_solutions;
               free(newused);
            }
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int f,
          TSetOfRotations sorn, TSetOfReflectionPlanes sorp) {
   fill = f;
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
      int sol = search(pc, pieceZeroInstances, pos, "", sp, sorn, sorp);
      solutions += sol;
      displayWide(ePyramid, 0);
      int max = solMaxPieceCount();
      printf("%d solutions starting with %c %d pieces%s", sol,  GLYPH(pc), max, EOL);
      int n;
      for (n = 1; n <= max; ++n) {
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
