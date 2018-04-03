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

#define SHAPE eUpright
#define PAGE_WIDTH 72

///////////////////////////////////////////////////////////////////////////////
int walk(EPresence pc, const char* path, EOrientation or, TPosition pos, PSquarePyramid sp) {
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
TSetOfOrientations getSkip(const TPosition* p, PSquarePyramid sp) {
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
int search(EPresence pc, TSetOfPresences used, TPosition* pos, const char* steps, PSquarePyramid sp, TSetOfOrientations skip) {
   int solutions = 0;
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
            TSquarePyramid newsp;
            spCopy(newsp, sp);
            SP_SET(newsp, pc, pos);
            walk(pc, pieces[pc][path], or, *pos, newsp);
            TSetOfPresences newused = SET_WITH(used, pc);
            char* newsteps = catStep(steps, stepToString(pc, pos, path, or));
//            if (!solIsUniqueSymmetric(newused, newsteps, newsp)) {
//               displayWide(eUpright, PAGE_WIDTH, 0);
//               display1(eUpright, newsp);
//               continue;
//            }
            TPosition newpos;
            spFind(&newpos, eAbsent, newsp);
            TSetOfOrientations newskip = 0;
            if (skip) {
               newskip = getSkip(&newpos, newsp);
            }
            int togo = 0;
            int fork = 0;
            int next_solutions = 0;
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
            if (fork || !togo) {
               if (solAddUniqueSymmetric(newused, newsteps, newsp)
                && !togo) {
                  next_solutions = 1;
               }
            }
            solutions += next_solutions;
            free(newsteps);
         }
      }
   }
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
void testPieceOrientations(EPresence pc, const char* path) {
   if (!pc || pc >= ePresences) {
      pc = eOrange;
   }
   printf("%c %s %s%s", GLYPH(pc), presToString(pc), path, EOL);
   printf("Orientation    Symmetry%s", EOL);
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
      TSquarePyramid sp;
      spClear(sp); //spInit(sp);
      SP_SET(sp, pc, &pos);
      walk(pc, path, or, pos, sp);
      displayWideRowRange(eCube, PAGE_WIDTH, 8, 49, sp);
   }
   displayWideRowRange(eCube, PAGE_WIDTH, 8, 49, 0);
}

///////////////////////////////////////////////////////////////////////////////
void testPathSymmetry(EOrientation or) {
   printf("%s%s", orToString(or), EOL);
   TPosition top = {0, 0, 0};
   TPosition mid = {2, 2, 4};
   TPosition pos = (or < 8) ? mid : top;
   int pc;
   for (pc = eGrey; pc < ePresences; ++pc) {
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         TSquarePyramid sp;
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
}

///////////////////////////////////////////////////////////////////////////////
void testSpFind() {
   char buf[POS_BUF_SIZE];
   TSquarePyramid sp;
   spInit(sp);
   TPosition pos = {0};
   int i;
   for (i = 0; i < 55; ++i) {
      spFind(&pos, eAbsent, sp);
      printf("%s%s", posToString(buf, &pos), EOL);
      SP_SET(sp, i + 'A' - 'a' + 1, &pos);
      display1(eUpright, sp);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
   int solutions = 0;
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;
   for (pc = eGrey; pc < ePresences; ++pc) {
      printf("%s (%c)%s",  presToString(pc), GLYPH(pc), EOL);
      solInit();
      TSetOfPresences used = 0;
      TSquarePyramid sp;
      spInit(sp);
      int sol = search(pc, used, &pos, "", sp, getSkip(&pos, sp));
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, 0);
      printf("%s sol %d%s",  presToString(pc), sol, EOL);
      int i;
      for (i = 1; i < ePresences; ++i) {
         printf("pieces %d%s", i, EOL);
         solDisplayByCount(i, SHAPE, PAGE_WIDTH);
      }
   }
   return solutions;
}
