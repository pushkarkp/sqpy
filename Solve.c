/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Repeat.h"
#include "PathState.h"
#include "Orientation.h"
#include "Move.h"
#include "SetOf.h"
#include "Display.h"
#include <stdio.h>

#define SHAPE eUpright
#define PAGE_WIDTH 80

///////////////////////////////////////////////////////////////////////////////
int walk(EPresence pc, const char* path, EOrientation or, TPosition pos, PSquarePyramid sp) {
//printf("walk %s\r\n", presToString(pc));
   TPathState ps;
   psInit(&ps);
   for (; *path; ++path) {
//printf("*path %c %s\r\n", *path, path);
      path = psRead(&pos, path, &ps);
//char buf1[16];
//char buf2[16];
//printf("pos %s or %s\r\n", posToString(buf1, &pos), orToString(buf2, or));
      const TMove* pmove;
      int count = charToMove(&pmove, *path, or);
//printf("move %d x %s\r\n", count, posToString(buf1, pmove));
      int i;
      for (i = 0; i < count; ++i) {
         step(&pos, pmove);
         if (spGet(&pos, sp) != eAbsent) {
            return 0;
         }
         if (pc != eAbsent) {
            spSet(sp, pc, &pos);
         }
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, TSetOfPresences used, TPosition* pos, PSquarePyramid sp, TSetOfOrientations skip) {
   int solutions = 0;
//char buf[16];
//printf("%s %s\r\n", presToString(pc), posToString(buf, pos));
   TSetOfPresences newused = SET_WITH(used, pc);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      if (used == 0) {
         path = 2;
         solInit();
      }
      EOrientation or;
      for (or = 0; or < ORIENTATIONS; ++or) {
         if (skip) {
            if (SET_HAS(skip, or)) {
//printf("%d skip %s\r\n", setCount(used), orToString(or));
               continue;
            }
char bufPos[16];
printf("%d use %s %s %s %s\r\n", setCount(used), orToString(or), presToString(pc), pieces[pc][path], posToString(bufPos, pos));
         }
         const TOrient* por = &orients[or];
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], or, *pos, sp)) {
            TSquarePyramid newsp;
            spCopy(newsp, sp);
            spSet(newsp, pc, pos);
            walk(pc, pieces[pc][path], or, *pos, newsp);
            if (!solIsUniqueSymmetric(newused, newsp)) {
               displayWide(eUpright, PAGE_WIDTH, 0);
               display1(eUpright, newsp);
               printf("duplicate%s", EOL);
               continue;
            }
            TPosition newpos;
            spFind(&newpos, eAbsent, newsp);
            TSetOfOrientations newskip = 0;
            if (skip) {
               newskip = rotateSkip(spEqualRotate(newsp, newsp)) 
                       | reflectSkip(spEqualReflect(newsp, newsp));
               if (newskip) {
                  displayWide(eUpright, PAGE_WIDTH, 0);
                  display1(eUpright, newsp);
                  printf("symmetrical%s", EOL);
               }
            }
/*char buf1[16];
char buf2[32];
char buf3[16];
printf("%s, %s: %s (%c) %s: %s\r\n", posToString(buf1, pos), orToString(buf2, por), presToString(pc), GLYPH(pc), pieces[pc][path], sopToString(buf3, newused));
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
                  int s = search(next, newused, &newpos, newsp, newskip);
                  if (s && next_solutions) {
                     fork = 1;
                  }
                  next_solutions += s;
               }
            }
            if (!togo || fork) {
               solAddUniqueSymmetric(newused, newsp);
               if (!togo) {
                  next_solutions = 1;
               }
            }
            solutions += next_solutions;
         }
      }
   }
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
void testSym() {
   int pc = eOrange;
   int path = 1;
   printf("path %s\r\n", pieces[pc][path]);
   TPosition pos = {2, 2, 2};
   TSetOfOrientations skip90 = rotateSkip(e90);
   TSetOfOrientations skip180 = rotateSkip(e180);
   EOrientation or;
   for (or = 0; or < ORIENTATIONS; ++or) {
      printf("%s (%s%s%sskip)\r\n", orToString(or), (SET_HAS(skip90, or) || SET_HAS(skip180, or)) ? "" : "no ", SET_HAS(skip90, or) ? "90 " : "", SET_HAS(skip180, or) ? "180 " : "");
      const TOrient* por = &orients[or];
      TSquarePyramid sp;
      spClear(sp);
      spSet(sp, pc, &pos);
      walk(pc, pieces[pc][path], or, pos, sp);
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
            spSet(sp, pc, &pos);
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
   spSet(sp, pc, &pos);
   walk(pc, pieces[pc][path], or, pos, sp);
   spFind(&pos, eAbsent, sp);
   for (pc = eGrey; pc < ePresences; ++pc) {
      if (!SET_HAS(used, pc)) {
         solutions += search(pc, used, &pos, sp, skip);
      }
   }
   displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("solutions %d\r\n", solutions);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
void testSpFind() {
   char buf[16];
   TSquarePyramid sp;
   spInit(sp);
   TPosition pos = {0};
   int i;
   for (i = 0; i < 55; ++i) {
      spFind(&pos, eAbsent, sp);
      printf("%s\r\n", posToString(buf, &pos));
      spSet(sp, i + 1, &pos);
      display1(eUpright, sp);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
   int all = SET_WITHOUT(SET_ALL(ePresences), eAbsent);
   int solutions = 0;
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;
//   for (pc = eGrey; pc < ePresences; ++pc) {
      TSquarePyramid sp;
      spInit(sp);
      TSetOfOrientations skip = rotateSkip(spEqualRotate(sp, sp)) 
                              | reflectSkip(spEqualReflect(sp, sp));
      TSetOfPresences used = 0;
      int sol = search(pc, used, &pos, sp, skip);
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, NULL);
      printf("%s sol %d unique %d\r\n\r\n", 
             presToString(pc), sol, solGetCount(all));
//   }
   int i;
   for (i = 1; i < ePresences; ++i) {
      solDisplayBySize(i, SHAPE, PAGE_WIDTH);
   }
   printf("solutions %d\r\n", solutions);
   return solutions;
}
