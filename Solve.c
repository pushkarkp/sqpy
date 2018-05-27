/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Steps.h"
#include "Repeat.h"
#include "Piece.h"
#include "Position.h"
#include "SetOf.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int useOnce = 0;
TSet topics = 0;

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
TSetOfOrientations getSkip(int pc, const TPosition* p, TPlace* sp) {
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
   if (skip && SET_HAS(topics, eDisplaySymmetries)) {
      char posBuf[POS_BUF_SIZE];
      printf("Symmetries for %c at %s (%s axis)%s", 
             GLYPH(pc), posToString(posBuf, p), ON_AXIS(p) ? "on" : "off", EOL);
      printf("Rotations %s%s",
             rotationToString(rot), EOL);
      char* strplanes = setToString(onPlanes(p), reflectionPlaneToString);
      char* strsorp = setToString(sorp, reflectionPlaneToString);
      printf("Reflections %s: %s%s", strplanes, strsorp, EOL); 
      free(strplanes);
      free(strsorp);
      char* strsoor = setToString(skip, orToString);
      printf("Skip %s%s", strsoor, EOL);
      free(strsoor);
   }
   return skip;
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, int* use, const TPosition* pos, const char* steps, TPlace* sp, TSetOfOrientations skip) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      EOrientation or;
      for (or = 0; or < eOrientations; ++or) {
         if (skip && SET_HAS(skip, or)) {
            continue;
         }
         const TOrient* por = &orients[or];
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && pcWalk(eAbsent, pieces[pc][path], or, pos, sp)) {
            spCopy(newsp, sp);
            SP_SET(newsp, pc, pos);
            pcWalk(pc, pieces[pc][path], or, pos, newsp);
            int* newuse = pcDupInstanceCounts(use);
            --newuse[pc];
            char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
            if (!solIsUniqueSymmetric(pcSumInstanceCounts(newuse), newsteps, newsp)) {
               continue;
            }
            if (SET_HAS(topics, eDisplaySteps)) {
               printf("%s%s", newsteps, EOL);
            }
            displayWide(ePyramid, newsp);
            TPosition newpos[eDimensions];
            spFind(newpos, eAbsent, newsp);
            if (newpos[eX] == -1) {
               if (solAddUniqueSymmetric(pcSumInstanceCounts(newuse), newsteps, newsp)) {
                  solutions = 1;
               }
            } else {
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               TSetOfOrientations newskip = 0;
               if (skip) {
                  newskip = getSkip(pc, newpos, newsp);
               }
               EPresence next;
               for (next = eFirstPiece; next < pieceCount; ++next) {
                  if (newuse[next] > 0) {
                     ++togo;
                     int s = search(next, newuse, newpos, newsteps, newsp, newskip);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || (!togo && useOnce)) {
                  if (solAddUniqueSymmetric(pcSumInstanceCounts(newuse), newsteps, newsp)) {
                     next_solutions = !fork;
                  }
               }
               solutions += next_solutions;
            }
            free(newuse);
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int once, TSet sot) {
   useOnce = once;
   topics = sot;
   int solutions = 0;
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {0, 0, 0};
   EPresence end = pc + 1;
   if (pc == eAbsent) {
      pc = eFirstPiece;
      end = pieceCount;
   }
   for (; pc < end; ++pc) {
      solInit(topics);
      TSet used = 0;
      spInit(sp);
      int sol = search(pc, pieceMaxInstances, pos, "", sp, 0);//getSkip(pc, pos, sp));
      solutions += sol;
      displayWide(ePyramid, 0);
      printf("%d solutions starting with %c%s", sol,  GLYPH(pc), EOL);
      int i;
      for (i = eFirstPiece; i < pieceCount; ++i) {
         int count = solCountForCount(i);
         if (count > 0) {
            printf("%d with %d pieces%s", count, i, EOL);
            solDisplayByCount(i, ePyramid);
         }
      }
   }
   free(sp);
   return solutions;
}
