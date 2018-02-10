/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Solve.h"
#include "Display.h"
#include <stdio.h>

#define SHAPE eUpright
#define PAGE_WIDTH 80
#define AS_INDEX(x) (2 * x + 1)

char repeat[ePresences][MAX_PATHS][e2dDimensions][eSigns][eSigns] = {0};

///////////////////////////////////////////////////////////////////////////////
void findRepeat() {
   int pc;
   for (pc = eGrey; pc < ePresences; ++pc) {
      EPlane plane;
      for (plane = e001; plane < ePlanes; ++plane) {
         // use every other sp to allow overflow
         TSquarePyramid sps[2 * PLANE_ORIENTATIONS * MAX_PATHS + 1];
         int distinct = 0;
         int path = 0;
         for (path = 0; pieces[pc][path]; ++path) {
            TOrientation or = {plane, 0, {0, 0}};
            TOrientation* por = &or;
            for (por = &or; por; por = orNext(por)) {
               if (por->plane != plane) {
                  break;
               }
               TPosition pos = {2, 2, 2};
               spClear(sps[AS_INDEX(distinct)]);
               spClear(sps[AS_INDEX(distinct) - 1]);
               spClear(sps[AS_INDEX(distinct) + 1]);
               spSet(sps[AS_INDEX(distinct)], pc, &pos);
               walk(pc, pieces[pc][path], &or, pos, sps[AS_INDEX(distinct)]);
               int eq;
               for (eq = distinct - 1; eq >= 0; --eq) {
                  if (spEqual(sps[AS_INDEX(distinct)], sps[AS_INDEX(eq)])
                   && spEqual(sps[AS_INDEX(distinct) - 1], sps[AS_INDEX(eq) - 1])
                   && spEqual(sps[AS_INDEX(distinct) + 1], sps[AS_INDEX(eq) + 1])) {
                     break;
                  }
               }
/*               char buf2[32];
               printf("%s: %s %s distinct %d eq %d", presenceToString(pc), orToString(buf2, por), pieces[pc][path], distinct, eq);
               if (por->plane) {
                  printf(" repeat %d\r\n", repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]);
                  displayWide(eCube, PAGE_WIDTH, sps[AS_INDEX(distinct)]);
               } else {
                  printf("\r\n");
                  displayWideRowRange(eCube, PAGE_WIDTH, 25, 33, sps[AS_INDEX(distinct)]);
               }
*/               if (por->plane
                && (eq < 0) == repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]) {
//                  printf("!! \r\n");
                  display1(eCube, sps[AS_INDEX(distinct)]);
               }
               if (eq < 0) {
                  ++distinct;
               } else {
                  repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]] = 1;
               }
            }
         }
//         displayWideRowRange(eCube, PAGE_WIDTH, 25, 33, 0);
      }
   }
}
