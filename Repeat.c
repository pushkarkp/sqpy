/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Orientation.h"
#include "Solve.h"
#include "Display.h"
#include <stdio.h>
#include <stdlib.h>

#define PAGE_WIDTH 65
#define AS_INDEX(x) (2 * x + 1)

///////////////////////////////////////////////////////////////////////////////
char repeat[ePresences][MAX_PATHS][e2dDimensions][eSigns][eSigns] = {0};

///////////////////////////////////////////////////////////////////////////////
void findRepeat() {
   int maxSps = 16;
   TPlace* sps = SP_NEW(maxSps);
   int pc;
   for (pc = 0; pc < ePresences; ++pc) {
      EPlane plane;
      for (plane = e001; plane < ePlanes; ++plane) {
         // use every other sp to allow overflow 
         int distinct = 0;
         int path = 0;
         for (path = 0; pieces[pc][path]; ++path) {
            EOrientation or;
            for (or = 0; or < eOrientations; ++or) {
               if (orients[or].plane != plane) {
                  continue;
               }
               TPosition pos[eDimensions] = {2, 2, 2};
               while (AS_INDEX(distinct) + 1 >= maxSps) {
                  maxSps *= 2;
                  sps = SP_EXTEND(maxSps, sps);
               }
               spClear(sps + SPS(AS_INDEX(distinct) - 1));
               spClear(sps + SPS(AS_INDEX(distinct)));
               spClear(sps + SPS(AS_INDEX(distinct) + 1));
               SP_SET(sps + SPS(AS_INDEX(distinct)), pc, pos);
               walk(pc, pieces[pc][path], or, pos, sps + spXYZ * AS_INDEX(distinct));
               int eq;
               for (eq = distinct - 1; eq >= 0; --eq) {
                  if (spEqual(sps + SPS(AS_INDEX(distinct)), 
                              sps + SPS(AS_INDEX(eq)))
                   && spEqual(sps + SPS(AS_INDEX(distinct) - 1), 
                              sps + SPS(AS_INDEX(eq) - 1))
                   && spEqual(sps + SPS(AS_INDEX(distinct) + 1), 
                              sps + SPS(AS_INDEX(eq) + 1))) {
                     break;
                  }
               }
/*char buf2[32];
printf("%s: %s %s distinct %d eq %d", presenceToString(pc), orToString(buf2, por), pieces[pc][path], distinct, eq);
if (por->plane) {
   printf(" repeat %d%s", repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]], EOL);
   displayWide(eCube, PAGE_WIDTH, sps[AS_INDEX(distinct)]);
} else {
   print(EOL);
   displayWideRowRange(eCube, PAGE_WIDTH, 25, 33, sps[AS_INDEX(distinct)]);
}
if (por->plane
 && (eq < 0) == repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]) {
   printf("!!%s", EOL);
   display1(eCube, sps[AS_INDEX(distinct)]);
}*/
               if (eq < 0) {
                  ++distinct;
               } else {
                  const TOrient* por = &orients[or];
                  repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]] = 1;
               }
            }
         }
//         displayWideRowRange(eCube, PAGE_WIDTH, 25, 33, 0);
      }
   }
   free(sps);
}
