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
int search(EPresence pc, const TSetOfPieces* used, TPosition* pos, PSquarePyramid sp) {
   int solutions = 0;
char buf1[16];
printf("%s %s\r\n", presenceToString(pc), posToString(buf1, pos));
   TOrientation or = {e110, eY, {ePlus, ePlus}};//{0, 0, {0, 0}};
   TOrientation* por = &or;
   for (por = &or; por; por = orNext(por)) {
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], por, *pos, sp)) {
            TSquarePyramid newsp;
            spCopy(newsp, sp);
            spSet(newsp, pc, pos);
            walk(pc, pieces[pc][path], por, *pos, newsp);
            TPosition newpos;
            sqFind(&newpos, eAbsent, spSymmetry(newsp), newsp);
            TSetOfPieces newused = *used;
            newused.piece[pc] = 1;
//char buf1[16];
//char buf2[32];
//char buf3[16];
//printf("%s, %s: %s (%c) %s: %s\r\n", posToString(buf1, pos), orToString(buf2, por), presenceToString(pc), glyph[pc], pieces[pc][path], setOfPiecesToString(buf3, &newused));
display1(eUpright, newsp);
//            displayWide(eUpright, PAGE_WIDTH, newsp);
            int togo = 0;
            int fork = 0;
            int path_solutions = 0;
            EPresence next;
            for (next = eGrey; next < ePresences; ++next) {
               if (!newused.piece[next]) {
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
                  path_solutions = 1;
               }
//               displayWide(eUpright, PAGE_WIDTH, newsp);
            }
            solutions += path_solutions;
         }
      }
   }
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve() {
   TPosition pos = {0, 0, 0};
   int pc = eWhite;
   TSquarePyramid sp;
   spInit(sp);
   spEnumerate(sp);
   do {
      spSet(sp, pc, &pos);
display1(eUpright, sp);
      sqFind(&pos, pc, e180, sp);
char buf[16];
   } while (pos.d[eX] != -1);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
int solve1() {
   int solutions = 0;
   TPosition pos = {0, 0, 0};
   int pc;
   for (pc = eGrey; pc < ePresences; ++pc) {
      TSquarePyramid sp;
      spInit(sp);
      TSetOfPieces used = {0};
      int sol = search(eWhite, &used, &pos, sp);
printf("%s sol %d\r\n\r\n", presenceToString(pc), sol);
      solutions += sol;
   }
printf("solutions %d\r\n", solutions);
   return solutions;
}
