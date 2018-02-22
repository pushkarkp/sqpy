/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
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
int search(EPresence pc, TSetOfPresences used, TPosition* pos, PSquarePyramid sp, TSquarePyramid* sym) {
   int solutions = 0;
//char buf[16];
//printf("%s %s\r\n", presenceToString(pc), posToString(buf, pos));
   TSetOfPresences newused = SOP_WITH(used, pc);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      if (used == 0) {
//         path = 2;
         solInit();
      }
      TOrientation or = {0, 0, {0, 0}};
      TOrientation* por = &or;
      for (por = &or; por; por = orNext(por)) {
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], por, *pos, sp)) {
            TSquarePyramid newsp;
            spCopy(newsp, sp);
            spSet(newsp, pc, pos);
            walk(pc, pieces[pc][path], por, *pos, newsp);
            if (solIsUniqueSymmetric(newused, newsp)) {
               TPosition newpos;
               spFind(&newpos, eAbsent, newsp);
/*char buf1[16];
char buf2[32];
char buf3[16];
printf("%s, %s: %s (%c) %s: %s\r\n", posToString(buf1, pos), orToString(buf2, por), presenceToString(pc), glyph[pc], pieces[pc][path], sopToString(buf3, newused));
//display1(SHAPE, newsp);
//printf("%s\r\n", rotationToString(sym));
displayWide(SHAPE, PAGE_WIDTH, newsp);
int done = 0;
EPresence u;
for (u = eGrey; u < ePresences; ++u) {
   if (SOP_HAS(newused, u)) {
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
                  if (!SOP_HAS(newused, next)) {
                     ++togo;
                     int s = search(next, newused, &newpos, newsp, sym);
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
   TSquarePyramid sym[ePresences];
   int pc = eLightBlue;
   TSetOfPresences used = 0;
   used = SOP_WITH(used, pc);
   int path = 2;
   TOrientation or = {e110, eX, {ePlus, ePlus}};
   TPosition pos = {0, 0, 0};
   TSquarePyramid sp;
   spInit(sp);
   spSet(sp, pc, &pos);
   walk(pc, pieces[pc][path], &or, pos, sp);
   spFind(&pos, eAbsent, sp);
   for (pc = eGrey; pc < ePresences; ++pc) {
      if (!SOP_HAS(used, pc)) {
         solutions += search(pc, used, &pos, sp, sym);
      }
   }
   displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("solutions %d\r\n", solutions);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
   int solutions = 0;
   TSquarePyramid sym[ePresences];
   TPosition pos = {0, 0, 0};
   int pc = eLightBlue;
   for (pc = eGrey; pc < ePresences; ++pc) {
      TSquarePyramid sp;
      spInit(sp);
      TSetOfPresences used = {0};
      int sol = search(pc, used, &pos, sp, sym);
      solutions += sol;
      displayWide(SHAPE, PAGE_WIDTH, NULL);
printf("%s sol %d unique %d\r\n\r\n", presenceToString(pc), sol, solGetCount(sopAll()));
   }
printf("solutions %d\r\n", solutions);
   return solutions;
}
