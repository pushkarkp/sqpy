/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "PathState.h"
#include "Repeat.h"
#include "Display.h"
#include <stdio.h>

#define SHAPE eUpright
#define PAGE_WIDTH 80

///////////////////////////////////////////////////////////////////////////////
int walk(EPresence pc, const char* path, const TOrientation* or, TPosition pos, PSquarePyramid sp) {
//printf("walk %s\r\n", presenceToString(pc));
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
int search(EPresence pc, const TSetOfPresences* used, TPosition* pos, PSquarePyramid sp) {
   int solutions = 0;
   int top = setOfPresencesAll(0, used);
//char buf[16];
//printf("%s %s\r\n", presenceToString(pc), posToString(buf, pos));
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      if (top) {
         solReset();
      }
      ERotation ePathSymmetry = e0;
      TOrientation or = {0, 0, {0, 0}};
      TOrientation* por = &or;
      for (por = &or; por; por = orNext(por)) {
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], por, *pos, sp)) {
            TSquarePyramid newsp;
            spCopy(newsp, sp);
            spSet(newsp, pc, pos);
            walk(pc, pieces[pc][path], por, *pos, newsp);
            ERotation sym = spSymmetry(newsp);
            if (sym != e0) {
               if (ePathSymmetry != e0) {
                  continue;
               } else {
                  ePathSymmetry = sym;
               }
            }
            TPosition newpos;
            spFind(&newpos, eAbsent, sym, newsp);
/*char buf[16];
//printf("%s %s\r\n", rotationToString(sym), posToString(buf, &newpos));
char buf1[16];
char buf2[32];
char buf3[16];
printf("%s, %s: %s (%c) %s: %s %s\r\n", posToString(buf1, pos), orToString(buf2, por), presenceToString(pc), glyph[pc], pieces[pc][path], setOfPiecesToString(buf3, &newused), rotationToString(sym));
//display1(SHAPE, newsp);
//printf("%s\r\n", rotationToString(sym));
displayWide(SHAPE, PAGE_WIDTH, newsp);
int done = 0;
EPresence u;
for (u = eGrey; u < ePresences; ++u) {
   if (newused.presence[u]) {
      ++done;
   }
}
if (done > 1) {
   continue;
}
*/
            TSetOfPresences newused = *used;
            newused.presence[pc] = 1;
            int togo = 0;
            int fork = 0;
            int path_solutions = 0;
            EPresence next;
            for (next = eGrey; next < ePresences; ++next) {
               if (!newused.presence[next]) {
                  ++togo;
                  int s = search(next, &newused, &newpos, newsp);
                  if (s && path_solutions) {
                     fork = 1;
                  }
                  path_solutions += s;
               }
            }
            if (!togo || fork) {
               if (!togo) {
                  solAddUniqueSymmetric(newsp);
                  path_solutions = 1;
               }
//               displayWide(SHAPE, PAGE_WIDTH, newsp);
            }
            solutions += path_solutions;
         }
      }
   }
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
void testSpSymmetry() {
   TOrientation or = {e110, eX, {ePlus, ePlus}};
   TPosition pos = {0, 0, 0};
   int pc;
   for (pc = eGrey; pc < ePresences; ++pc) {
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         TSquarePyramid sp;
         spInit(sp);
         if (walk(eAbsent, pieces[pc][path], &or, pos, sp)) {
            spSet(sp, pc, &pos);
            walk(pc, pieces[pc][path], &or, pos, sp);
            printf("%s %s %s\r\n", presenceToString(pc), pieces[pc][path], rotationToString(spSymmetry(sp)));
            displayWide(SHAPE, PAGE_WIDTH, sp);
         }
      }
      displayWide(SHAPE, PAGE_WIDTH, NULL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solve1() {
   int solutions = 0;
   int pc = eLightBlue;
   TSetOfPresences used = {0};
   used.presence[pc] = 1;
   int path = 2;
   TOrientation or = {e110, eX, {ePlus, ePlus}};
   TPosition pos = {0, 0, 0};
   TSquarePyramid sp;
   spInit(sp);
   spSet(sp, pc, &pos);
   walk(pc, pieces[pc][path], &or, pos, sp);
   ERotation sym = spSymmetry(sp);
   spFind(&pos, eAbsent, sym, sp);
   for (pc = eGrey; pc < ePresences; ++pc) {
      if (!used.presence[pc]) {
         solutions += search(pc, &used, &pos, sp);
      }
   }
   displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("solutions %d\r\n", solutions);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
   int solutions = 0;
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;;
   //for (pc = eGrey; pc < ePresences; ++pc) {
      TSquarePyramid sp;
      spInit(sp);
      TSetOfPresences used = {0};
      int sol = search(pc, &used, &pos, sp);
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("%s sol %d unique %d\r\n\r\n", presenceToString(pc), sol, solGetCount());
//   }
printf("solutions %d\r\n", solutions);
   return solutions;
}
