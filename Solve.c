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

//#define CMP_SKIP
//#define CMP_SKIP_SEP

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
//  displayWide(eUpright, PAGE_WIDTH, 0);
//  display1(eUpright, newsp);
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
//char buf[POS_BUF_SIZE];
//printf("%s %s\r\n", presToString(pc), posToString(buf, pos));
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      if (SET_WITHOUT(used, ePresences) == 0) {
printf("%s%s", presToString(pc), EOL);
         path = 2;
#ifndef CMP_SKIP
         solInit();
#endif
      }
      EOrientation or;
      for (or = 0; or < ORIENTATIONS; ++or) {
         if (skip) {
            char* step = stepToString(pc, pos, path, or);
printf("%s: %s %s %s %s%s", steps, step, presToString(pc), (SET_HAS(skip, or) ? "skip" : "chek"), orToString(or), EOL);
            free(step);
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
/*char buf1[POS_BUF_SIZE];
char buf2[ORIENT_BUF_SIZE];
char buf3[SOP_BUF_SIZE];
printf("%s, %s: %s (%c) %s: %s\r\n", posToString(buf1, pos), orientToString(buf2, por), presToString(pc), GLYPH(pc), pieces[pc][path], sopToString(buf3, newused));
//display1(SHAPE, newsp);
//printf("%s\r\n", rotationToString(sym));
displayWide(SHAPE, PAGE_WIDTH, newsp);
int done = 0;
EPresence u;
for (u = eGrey; u < ePresences; ++u) {
   if (SET_HAS(newused, u)) {
      ++done;
   }
}
if (done > 1) {
   continue;
}
*/
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
            if (fork) {
               solAddUniqueSymmetric(newused, newsteps, newsp);
            }
            if (!togo) {
               if (solAddUniqueSymmetric(newused, newsteps, newsp)) {
//               if (solAddUniqueSymmetric(SET_WITHOUT(newused, ePresences), newsteps, newsp)) {
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
   for (or = 0; or < ORIENTATIONS; ++or) {
      const char* isSkip = (SET_HAS(skip90, or) || SET_HAS(skip180, or) 
                         || SET_HAS(skip100, or) || SET_HAS(skip010, or) 
                         || SET_HAS(skip110, or) || SET_HAS(skip1T0, or)) ? "" : "no ";
      printf("%s (%s%s%s%s%s%s%sskip)\r\n", orToString(or), isSkip, 
               SET_HAS(skip90, or) ? "90 " : "",
               SET_HAS(skip180, or) ? "180 " : "",
               SET_HAS(skip100, or) ? "100 " : "",
               SET_HAS(skip010, or) ? "010 " : "",
               SET_HAS(skip110, or) ? "110 " : "",
               SET_HAS(skip1T0, or) ? "1T0 " : "");
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
void testSpSymmetry() {
   EOrientation or = GET_ORIENTATION(e110, eX, ePlus, ePlus);
   TPosition pos = {0, 0, 0};
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
            printf("%s %s rotation %s reflection %s\r\n", 
                   presToString(pc), pieces[pc][path], 
                   rotationToString(spEqualRotate(sp, sp)), 
                   sorpToString(buf, spEqualReflect(sp, sp)));
            displayWide(SHAPE, PAGE_WIDTH, sp);
         }
      }
      displayWide(SHAPE, PAGE_WIDTH, NULL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solve1() {
   int solutions = 0;
   TSetOfOrientations skip = rotateSkip(e90) 
                           | reflectSkip(e100Reflection | e010Reflection | e110Reflection | e1T0Reflection);
   int pc = eLightBlue;
   TSetOfPresences used = 0;
   used = SET_WITH(used, pc);
   int path = 2;
   EOrientation or = GET_ORIENTATION(e110, eX, ePlus, ePlus);
   TPosition pos = {0, 0, 0};
   TSquarePyramid sp;
   spInit(sp);
   SP_SET(sp, pc, &pos);
   walk(pc, pieces[pc][path], or, pos, sp);
   spFind(&pos, eAbsent, sp);
   for (pc = eGrey; pc < ePresences; ++pc) {
      if (!SET_HAS(used, pc)) {
         solutions += search(pc, used, &pos, "", sp, skip);
      }
   }
   displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("solutions %d\r\n", solutions);
   return solutions;
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
      printf("%s\r\n", posToString(buf, &pos));
      SP_SET(sp, i + 1, &pos);
      display1(eUpright, sp);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
/*   solInit();
   TSquarePyramid ssp;
   spInit(ssp);
   solAddUniqueSymmetric(1, "steps", ssp);
   solDisplay(1, SHAPE, PAGE_WIDTH);
   //spEnumerate(ssp);
   solAddUniqueSymmetric(1, "more steps", ssp);
   solDisplay(1, SHAPE, PAGE_WIDTH);
   return 0;*/
   const TSetOfPresences all = SET_WITHOUT(SET_ALL(ePresences), eAbsent);
   int solutions = 0;
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;
#ifdef CMP_SKIP
   solInit();
   int m;
   for (m = 0; m < 2; ++m) {
#else
//   for (pc = eGrey; pc < ePresences; ++pc) {
#endif
      TSetOfPresences used = 0;
      TSquarePyramid sp;
      spInit(sp);
      TSetOfOrientations skip = rotateSkip(spEqualRotate(sp, sp))
                              | reflectSkip(spEqualReflect(sp, sp));
#ifdef CMP_SKIP
      if (m) {
#ifdef CMP_SKIP_SEP
         used = SET_WITH(used, ePresences);
#endif
         skip = 0;
      }
#endif
      int sol = search(pc, used, &pos, "", sp, skip);
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, 0);
      printf("%s sol %d unique %d\r\n", 
             presToString(pc), sol, solGetCount(all | used));
      int i;
      for (i = 1; i < ePresences; ++i) {
printf("pieces %d%s", i, EOL);
         solDisplayByCount(i, SHAPE, PAGE_WIDTH);
      }
   //}
#ifdef CMP_SKIP_SEP
   TSetOfPresences more = SET_WITH(all, ePresences);
   TSetOfPresences less = all;
   if (solGetCount(less) > solGetCount(more)) {
printf("skip > all\r\n");
      TSetOfPresences tmp = more; 
      more = less;
      less = tmp;
   }
   int size = solGetCount(more);
   int s;
   for (s = 0; s < size; ++s) {
      PSquarePyramid sp = solGet(more, s);
      if (solIsUnique(less, sp)
       && solIsUniqueSymmetric(less, sp)) {
         displayWide(SHAPE, PAGE_WIDTH, sp);
      }
   }
   displayWide(SHAPE, PAGE_WIDTH, NULL);
#endif
   printf("solutions %d%s", solutions, EOL);
   return solutions;
}
