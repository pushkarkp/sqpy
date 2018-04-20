/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Steps.h"
#include "Repeat.h"
#include "PathState.h"
#include "Orientation.h"
#include "Move.h"
#include "SetOf.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SHAPE ePyramid
#define PAGE_WIDTH 65

int useOnce = 0;

///////////////////////////////////////////////////////////////////////////////
int walk(EPresence pc, const char* path, EOrientation or, TPosition pos, TPlace* sp) {
   TPosition ps[ePositionStores];
   PS_INIT(ps);
   for (; *path; ++path) {
      path = psRead(&pos, path, ps);
      const TMove* pmove;
      int count = charToMove(&pmove, *path, or);
      int i;
      for (i = 0; i < count; ++i) {
         step(&pos, pmove);
         if (SP_GET(&pos, sp) != eAbsent) {
            return 0;
         }
         if (pc != eAbsent) {
            SP_SET(sp, pc, &pos);
         }
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations getSkip(const TPosition* p, TPlace* sp) {
   TSetOfOrientations skip = 0;
   ERotation rot = 0;
   if (ON_AXIS(*p)) {
      rot = spEqualRotate(sp, sp);
      skip |= rotateSkip(rot);
   }
   TSetOfReflectionPlanes sorp = onPlanes(p);
   if (sorp) {
      sorp &= spEqualReflect(sp, sp);
      skip |= reflectSkip(sorp);
   }
   if (skip) {
      char posBuf[POS_BUF_SIZE];
      char sorpBuf1[SORP_BUF_SIZE];
      char sorpBuf2[SORP_BUF_SIZE];
      char soorBuf[SOOR_BUF_SIZE];
      printf("symmetry(s) %s%s %s, %s: %s skip %s%s", 
             posToString(posBuf, p),
             ON_AXIS(*p) ? " on axis" : "",
             rotationToString(rot),
             sorpToString(sorpBuf1, onPlanes(p)),
             sorpToString(sorpBuf2, sorp),
             soorToString(soorBuf, skip),
             EOL);
   }
   return skip;
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, TSetOfPresences used, TPosition* pos, const char* steps, TPlace* sp, TSetOfOrientations skip) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      EOrientation or;
      for (or = 0; or < eOrientations; ++or) {
         if (skip) {
//char* step = stepToString(pc, pos, path, or);
//printf("%s: %s %s %s %s%s", steps, step, presToString(pc), (SET_HAS(skip, or) ? "skip" : "chek"), orToString(or), EOL);
//free(step);
            if (SET_HAS(skip, or)) {
               continue;
            }
         }
         const TOrient* por = &orients[or];
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], or, *pos, sp)) {
            spCopy(newsp, sp);
            SP_SET(newsp, pc, pos);
            walk(pc, pieces[pc][path], or, *pos, newsp);
            TSetOfPresences newused = SET_WITH(used, pc);
            char* newsteps = catStep(steps, stepToString(pc, pos, path, or));
//            if (!solIsUniqueSymmetric(newused, newsteps, newsp)) {
//               displayWide(ePyramid, PAGE_WIDTH, 0);
//               display1(ePyramid, newsp);
//               continue;
//            }
            TPosition newpos;
            spFind(&newpos, eAbsent, newsp);
            if (newpos.d[eX] == -1) {
               if (solAddUniqueSymmetric(newused, newsteps, newsp)) {
                  solutions = 1;
               }
            } else {
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               TSetOfOrientations newskip = 0;
               if (skip) {
                  newskip = getSkip(&newpos, newsp);
               }
               EPresence next;
               for (next = eGrey; next < ePresences; ++next) {
                  if (!SET_HAS(newused, next)) {
                     ++togo;
                     int s = search(next, newused, &newpos, newsteps, newsp, newskip);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || (!togo && useOnce)) {
                  if (solAddUniqueSymmetric(newused, newsteps, newsp)) {
                     next_solutions = 1;
                  }
               }
               solutions += next_solutions;
            }
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
void testPieceOrientations(EPresence pc, const char* path) {
   if (!pc || pc >= ePresences) {
      pc = eOrange;
   }
   printf("%c %s %s%s", GLYPH(pc), presToString(pc), path, EOL);
   printf("Orientation    Symmetry%s", EOL);
   TPlace* sp = SP_NEW(1);
   TPosition pos = {2, 2, 2};
   TSetOfOrientations skip90 = rotateSkip(e90);
   TSetOfOrientations skip180 = rotateSkip(e180);
   TSetOfOrientations skip100 = reflectSkip(SET_WITH(0, e100Reflection));
   TSetOfOrientations skip010 = reflectSkip(SET_WITH(0, e010Reflection));
   TSetOfOrientations skip110 = reflectSkip(SET_WITH(0, e110Reflection));
   TSetOfOrientations skip1T0 = reflectSkip(SET_WITH(0, e1T0Reflection));
   EOrientation or;
   for (or = 0; or < eOrientations; ++or) {
      const char* isSkip = (SET_HAS(skip90, or) || SET_HAS(skip180, or) 
                         || SET_HAS(skip100, or) || SET_HAS(skip010, or) 
                         || SET_HAS(skip110, or) || SET_HAS(skip1T0, or)) ? "" : "no ";
      printf("%s (%s%s%s%s%s%s%sskip)%s", orToString(or), isSkip, 
               SET_HAS(skip90, or) ? "90 " : "",
               SET_HAS(skip180, or) ? "180 " : "",
               SET_HAS(skip100, or) ? "100 " : "",
               SET_HAS(skip010, or) ? "010 " : "",
               SET_HAS(skip110, or) ? "110 " : "",
               SET_HAS(skip1T0, or) ? "1T0 " : "",
               EOL);
      const TOrient* por = &orients[or];
      spClear(sp); //spInit(sp);
      SP_SET(sp, pc, &pos);
      walk(pc, path, or, pos, sp);
      displayWideRowRange(eCube, PAGE_WIDTH, 8, 49, sp);
   }
   displayWideRowRange(eCube, PAGE_WIDTH, 8, 49, 0);
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
void testPathSymmetry(EOrientation or) {
   printf("%s%s", orToString(or), EOL);
   TPlace* sp = SP_NEW(1);
   TPosition top = {0, 0, 0};
   TPosition mid = {2, 2, 4};
   TPosition pos = (or < 8) ? mid : top;
   int pc;
   for (pc = eGrey; pc < ePresences; ++pc) {
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         spInit(sp);
         if (walk(eAbsent, pieces[pc][path], or, pos, sp)) {
            SP_SET(sp, pc, &pos);
            walk(pc, pieces[pc][path], or, pos, sp);
            char buf[16];
            printf("%s %s rotation %s reflection %s%s", 
                   presToString(pc), pieces[pc][path], 
                   rotationToString(spEqualRotate(sp, sp)), 
                   sorpToString(buf, spEqualReflect(sp, sp)),
                   EOL);
            displayWide(SHAPE, PAGE_WIDTH, sp);
         }
      }
      displayWide(SHAPE, PAGE_WIDTH, NULL);
   }
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
void testSpFind() {
   char buf[POS_BUF_SIZE];
   TPlace* sp = SP_NEW(1);
   spInit(sp);
   TPosition pos = {0};
   int i;
   for (i = 0; i < 56; ++i) {
      spFind(&pos, eAbsent, sp);
      printf("%s%s", posToString(buf, &pos), EOL);
      SP_SET(sp, i + 'A' - 'a' + 1, &pos);
      displayWide(ePyramid, 65, sp);
   }
   displayWide(ePyramid, 65, 0);
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
int solve(int once) {
   useOnce = once;
   int solutions = 0;
   TPlace* sp = SP_NEW(1);
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;
   for (pc = eGrey; pc < ePresences; ++pc) {
      printf("%s (%c)%s",  presToString(pc), GLYPH(pc), EOL);
      solInit();
      TSetOfPresences used = 0;
      spInit(sp);
      int sol = search(pc, used, &pos, "", sp, getSkip(&pos, sp));
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, 0);
      printf("%s sol %d%s",  presToString(pc), sol, EOL);
      int i;
      for (i = 1; i < ePresences; ++i) {
         int count = solCountForCount(i);
         if (count > 0) {
            printf("%d pieces: %d%s", i, count, EOL);
            solDisplayByCount(i, SHAPE, PAGE_WIDTH);
         }
      }
   }
   free(sp);
   return solutions;
}
